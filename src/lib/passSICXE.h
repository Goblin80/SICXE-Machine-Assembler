#define tBufferSIZE 256

int pass0(operation *out, char in[], const char d[]) // split
{
    char a[3][sSIZE];
    memset(a, 0, sizeof a);
    int tok_count = 0, k;
    for(char *t = strtok(in, d); t; t = strtok(NULL, d))
        strcpy(a[tok_count++], strCapitalize(t));

    if(tok_count)
    {
        if((k = isAsmFunc(a[0]) == -1 && getOpcode(a[0]) == -1))
            strcpy(out->label, a[0]);
    
        strcpy(out->operator.mnemonic, a[k]);
        strcpy(out->operand, a[k + 1]);
    }
    return tok_count;
}

void pass1(program *p) // LOC
{
    char *s = p->op[0].operator.mnemonic, *t;
    short LOCCTR = 0, l = p->lines, *w;
    if(isStrEq(s, "START"))
        sscanf(p->op[0].operand, "%hx", &LOCCTR);
    
    for(operation *c = p->op; l--; c++)
    {
        c->loc = LOCCTR;
        s = c->operator.mnemonic;
        t = c->operand;
        w = &c->opwidth;
        
        if(isStrEq(s, "BYTE"))
            switch(t[0])
            {
                case 'X':
                    *w = (strlen(t) - 2) / 2;
                    break;
                case 'C':
                    *w = strlen(t) - 3;
                    break;
            }
        else if(isStrEq(s, "RESW")) *w = 3 * strInt(t);
        else if(isStrEq(s, "RESB")) *w = strInt(t);
        else if(isStrEq(s, "WORD")) // *w = 3;
        {
            short commas = 1;
            for(short i = 0; t[i] != '\0'; i++)
                if(t[i] == ',') commas++;
            *w = 3 * commas;
        }
        else if(isAsmFunc(s) == 1) *w = c->loc = 0;
        else c->operator.format = *w = getOpsize(s),
             c->operator.opcode = getOpcode(s);

        if(!isStrEq(c->label, "")) // append to the symbol table
        {
            strcpy(p->st.entry[p->st.len].symbol, c->label);
            p->st.entry[p->st.len++].value = LOCCTR;
        }
        LOCCTR += *w;
    }
    p->len = LOCCTR - p->op[0].loc;
}


void pass2(program *p)
{
    char *s, *t, buffer[tBufferSIZE], charBuffer[sSIZE], a[sSIZE];
    short b, l = p->lines, PC, BASE, TA;
    PC = BASE = 0;
    for(operation *c = p->op; l--; c++)
    {
        PC += c->opwidth;
        s = c->operator.mnemonic;
        t = c->operand;

        if(c->opwidth == c->operator.format)
            switch(c->opwidth)
            {
                case 0: //AsmFunc
                    if(isStrEq(s, "BASE"))
                        BASE = getLabelLocation(p, t);
                    break;
                case 1:
                    parseFormat1(p, c);
                    break;
                case 2:
                    parseFormat2(p, c);
                    break;
                case 3:
                    parseFormat3(p, c, PC, BASE);
                    break;
                case 4:
                    parseFormat4(p, c);
                    break;
                default:
                    //something went wrong
                    break;
            }
        else
        {
            if(isStrEq(s, "BYTE"))
            switch(t[0]) // maybe i should pack this
            {
                case 'X':
                    sscanf(t, "X'%hx'", &b);
                    sprintf(c->objectcode, "%02X", b); //cheap fix
                    break;
                case 'C':                    
                    sscanf(t, "C'%[^']'", a);
                    for(int j = 0; a[j] != '\0'; j++)
                    {
                        sprintf(charBuffer, "%02X", a[j]);
                        strcat(c->objectcode, charBuffer);
                    }
                    break;
            }
            else if(isStrEq(s, "WORD"))
            {
                sscanf(t, "%hd", &b);
                sprintf(c->objectcode, "%03X", b);
            }
        }


    }
}

void hRECORD(program *p)
{
    operation *s = p->op;
    char out[tBufferSIZE];
    sprintf(out, "H^%-6s^%06X^%06X", s->label, s->loc, p->len);
    printf("%s\n", out);
}

void tRECORD(program *p)
{
    operation *s = p->op;
    char tBuffer[tBufferSIZE], out[tBufferSIZE];
    short tBufferLoc = s->loc, tBufferLen = 0, l;

    memset(tBuffer, 0, sizeof tBuffer);
    while(!isStrEq(s++->operator.mnemonic, "END"))
    {
        if(isStrEq(s->operator.mnemonic, "BASE")) continue; //cheap fix, very cheap
        l = strlen(s->objectcode) / 2;
        // l = s->opwidth;
        if((tBufferLen + l > rSIZE || !l) && tBufferLen) //rSIZE is 0x1E
        {
            sprintf(out, "T^%06X^%02X%s", tBufferLoc, tBufferLen, tBuffer);
            memset(tBuffer, tBufferLen = 0, tBufferSIZE); //size of what the pointer points... doesn't work
            printf("%s\n", out);
        }

        if(!l) continue;
        if(!tBufferLen) tBufferLoc = s->loc;
        strcat(tBuffer, "^");
        strcat(tBuffer, s->objectcode);
        tBufferLen += l;
    }
}

void eRECORD(program *p)
{
    char out[tBufferSIZE];
    sprintf(out, "E^%06X", p->op[0].loc);
    printf("%s\n", out);
}

void mRECORD(program *p)
{
    operation *s = p->op;
    char out[tBufferSIZE];
    while(!isStrEq(s++->operator.mnemonic, "END"))
        if(s->operator.format == 4 && s->operand[0] != '#')
        {
            sprintf(out, "M^%06X^05", s->loc + 1);
            printf("%s\n", out);
        }
}

void genHTE(program *p) //generate HTE Record
{
    hRECORD(p);
    tRECORD(p);
    mRECORD(p);
    eRECORD(p);
}
