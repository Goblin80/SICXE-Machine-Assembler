typedef struct
{
    char mnemonic[sSIZE];
    short format,
    	  opcode;
}instruction;

typedef struct
{
    short loc,
    	  opwidth;
    instruction operator;
    char label[sSIZE],
         operand[sSIZE],
         objectcode[sSIZE];
}operation;

typedef struct
{
	short len,
		  lines;
	operation op[pSIZE];
}program;