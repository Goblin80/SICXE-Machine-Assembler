int isAsmFunc(char x[sSIZE])
{
    char t[][sSIZE] = { "START", "END", "BYTE",
                        "WORD", "RESB", "RESW",
                        "BASE", "ORG" ,"EQU"};
    for(int i = 0; i < sizeof t / sSIZE; i++)
        if(isStrEq(t[i], x)) return 1;
    return -1;
}

instruction t[] = { {"FIX", 1, 0xC4}, {"FLOAT", 1, 0xC0}, {"HIO", 1, 0xF4},
                    {"NORM", 1, 0xC8}, {"SIO", 1, 0xF0}, {"TIO", 1, 0xF8},
                    {"ADDR", 2, 0x90}, {"CLEAR", 2, 0xB4}, {"COMPR", 2, 0xA0},
                    {"DIVR", 2, 0x9C}, {"MULR", 2, 0x98}, {"RMO", 2, 0xAC},
                    {"SHIFTL", 2, 0xA4}, {"SHIFTR", 2, 0xA8}, {"SUBR", 2, 0x94},
                    {"SVC", 2, 0xB0}, {"TIXR", 2, 0xB8}, {"ADD", 3, 0x18},
                    {"ADDF", 3, 0x58}, {"AND", 3, 0x40}, {"COMP", 3, 0x28},
                    {"COMPF", 3, 0x88}, {"DIV", 3, 0x24}, {"DIVF", 3, 0x64},
                    {"J", 3, 0x3C}, {"JEQ", 3, 0x30}, {"JGT", 3, 0x34},
                    {"JLT", 3, 0x38}, {"JSUB", 3, 0x48}, {"LDA", 3, 0x00},
                    {"LDB", 3, 0x68}, {"LDCH", 3, 0x50}, {"LDF", 3, 0x70},
                    {"LDL", 3, 0x08}, {"LDS", 3, 0x6C}, {"LDT", 3, 0x74},
                    {"LDX", 3, 0x04}, {"LPS", 3, 0xD0}, {"MUL", 3, 0x20},
                    {"MULF", 3, 0x60}, {"OR", 3, 0x44}, {"RD", 3, 0xD8},
                    {"RSUB", 3, 0x4C}, {"SSK", 3, 0xEC}, {"STA", 3, 0x0C},
                    {"STB", 3, 0x78}, {"STCH", 3, 0x54}, {"STF", 3, 0x80},
                    {"STI", 3, 0xD4}, {"STL", 3, 0x14}, {"STS", 3, 0x7C},
                    {"STSW", 3, 0xE8}, {"STT", 3, 0x84}, {"STX", 3, 0x10},
                    {"SUB", 3, 0x1C}, {"SUBF", 3, 0x5C}, {"TD", 3, 0xE0},
                    {"TIX", 3, 0x2C}, {"WD", 3, 0xDC} };

short getOpcode(char m[sSIZE])
{
    short f = 0;
    if(m[0] == '+') f++, m++;
    for(int i = 0;i < sizeof t / sizeof (instruction); i++)
        if(isStrEq(t[i].mnemonic, m))
            return t[i].opcode;
    return -1;
}

short getOpsize(char m[sSIZE])
{
    short f = 0;
    if(m[0] == '+') f++, m++;
    for(int i = 0;i < sizeof t / sizeof (instruction); i++)
        if(isStrEq(t[i].mnemonic, m))
            return t[i].format + f;
    return -1;
}

short getLabelLocation(program *p, char c[sSIZE])
{
    char *a, b[sSIZE];
    symbolEntry *e = p->st.entry;
    strcpy(b, c); strtok(b, ","); a = b;
    if(a[0] == '#' || a[0] == '@') a++;
    if(isStrEq(a, "")) return 0;

    for(int i = 0; i < p->st.len; i++, e++)
        if(isStrEq(e->symbol, a))
            return e->value;
    return -1;
}

short getRegisterNumeric(char r[])
{
    char registers[][3] = {"A", "X", "L", "B", "S", "T", "F", "?", "PC", "SW"};
    for(int i = 0; i < sizeof(registers) / sizeof(*registers); i++)
        if(isStrEq(r, registers[i]))
            return i;
    return 0;
}

void setFlag(short *f, char flag)
{
    char bits[] = {'e', 'p', 'b', 'x', 'i', 'n'};
    for(int i = 0; i < sizeof(bits); i++)
            if(flag == bits[i])
                *f |= 1 << i;
}