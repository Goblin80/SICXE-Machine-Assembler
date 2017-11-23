typedef struct
{
    char mnemonic[sSIZE];
    short format,
    	  opcode;
}instruction;

typedef struct
{
    short loc,
    	  opwidth,
          flag;
    instruction operator;
    char label[6 + 1],
         operand[sSIZE],
         objectcode[sSIZE];
}operation;

typedef struct
{
    char symbol[6 + 1];
    int value; // 1 word
    short type; // 2 bytes
}symbolEntry;

typedef struct
{
    symbolEntry entry[100]; // should be more
    short len;
}symbolTable;

typedef struct
{
	short len,
		  lines,
          literalCount;
	operation op[pSIZE];
    symbolTable st;
}program;