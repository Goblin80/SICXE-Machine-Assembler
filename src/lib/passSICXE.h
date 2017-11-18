#define tBufferSIZE 256

void pass0(operation *out, char in[], const char d[]) // split
{
	char a[3][sSIZE];
	memset(a, 0, sizeof a);
	int i = 0, k;
    for(char *t = strtok(in, d); t; t = strtok(NULL, d))
        strcpy(a[i++], strCapitalize(t));
        
    if((k = isAsmFunc(a[0]) == -1 && getOpcode(a[0]) == -1))
        strcpy(out->label, a[0]);
    
    strcpy(out->operator.mnemonic, a[k]);
    strcpy(out->operand, a[k + 1]);
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
                sscanf(t, "%d", &b);
                sprintf(c->objectcode, "%03X", b);
            }
        }


    }
}