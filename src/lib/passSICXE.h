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

void pass1(program *p) //LOC
{
    char *s = p->op[0].operator.mnemonic, *t;
    short LOCCTR = 0, i = 0, *w;
    if(isStrEq(s, "START"))
		sscanf(p->op[i].operand, "%hx", &LOCCTR);
    
    for(operation *c = p->op; i < p->lines; i++, c++)
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
        else if(isAsmFunc(s) == 1) *w = 0, c->loc = 0;
        else *w = getOpsize(s);

        LOCCTR += *w;
    }
    p->len = LOCCTR - p->op[0].loc;
}