void parseFormat1(program *p, operation *c)
{
    sprintf(c->objectcode, "%02X", c->operator.opcode);
}

void parseFormat2(program *p, operation *c)
{
    char s[sSIZE];
    strcpy(s, c->operand);
    unsigned short reg = 0, i = 4;
    for(char *t = strtok(s, ","); t; t = strtok(NULL, ","), i = 0)
        reg |= getRegisterNumeric(t) << i;
    sprintf(c->objectcode, "%02X%02X", c->operator.opcode, reg);
}

void parseFormat3(program *p, operation *c, short PC, short BASE)
{
    char *s, *t; //HANDLE Indexed
    short TA;
    s = c->operator.mnemonic;
    t = c->operand;

    struct {unsigned short value:12;}disp;

    if(isStrEq(s, "RSUB")) // only exception;
    {
        sprintf(c->objectcode, "%03X%03X", c->operator.opcode << 4 | 3 << 4, 0);
        return;
    }

    TA = getLabelLocation(p, t);
    switch(t[0])
    {
        case '#':
            t++;
            c->flag |= 1 << 4; //set i
            if(t[0] < 'A')
            {
                sscanf(t, "%3X", &TA);
                sprintf(c->objectcode, "%03X%03X", c->operator.opcode << 4 | c->flag, TA);
                return;
            }
            break;
        case '@':
            c->flag |= 1 << 5; //set n
            break;
        default:
            c->flag |= 3 << 4; //set n, i
            break;
    }

    for(int i = 0; i < strlen(t); i++)
        if(t[i] == ',')
            c->flag |= 1 << 3;

    if(TA - PC < 1<<11)
    {
        disp.value = TA - PC;
        c->flag |= 1 << 1; //set p
    }
    else if(TA - BASE < 1<<12)
    {
        disp.value = TA - BASE;
        c->flag |= 1 << 2; //set b
    }
    sprintf(c->objectcode, "%03X%03X", c->operator.opcode << 4 | c->flag, disp.value);
}

void parseFormat4(program *p, operation *c)
{
    char *s = c->operator.mnemonic + 1 , //skips '+'
         *t = c->operand;
    unsigned short TA;

    c->flag |= 1; // set e
    TA = getLabelLocation(p, t);

    if(t[0] == '#')
    {
        c->flag |= 1 << 4; //set i
        t++; // skips '#'
        if(t[0] < 'A')
            sscanf(t, "%05X", &TA);
    }
    sprintf(c->objectcode, "%03X%05X", c->operator.opcode << 4 | c->flag, TA);
}