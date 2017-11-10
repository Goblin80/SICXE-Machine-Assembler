#include <stdio.h>
#include <string.h>

#define pSIZE 1024
#define sSIZE 30
#define rSIZE 0x1E
#define isStrEq(a, b) !strcmp(a, b)

#include "lib/declareDATA.h"
#include "lib/xtraSTR.h"
#include "lib/MDAFs.h"
#include "lib/passSICXE.h"


void printSymbolTable(program *p)
{
    operation *t = p->op;

    for(int i = 0; i < 17; i++) printf("%c", 215); printf("\n");

    for(int i = 0;i < p->lines; i++, t++)
        if(!isStrEq(t->label, ""))
            printf("%c %04X %c %-6s %c\n", 186, t->loc, 186, t->label, 186);
    for (int i = 0; i < 17; i++) printf("%c", 215);
}

void printPObject(program *p)
{
    operation *t = p->op;
    for(int i = 0;i < p->lines; i++, t++)
        printf("%04X\t%-10s%-10s%-10s%-10s\n",
                t->loc, t->label, t->operator.mnemonic, t->operand, t->objectcode);

}

void parseSRC(program *p, char fname[])
{
    char s[sSIZE];
    freopen(fname, "r", stdin);

    for(operation *t = p->op; scanf("%[^\n]%*c", s) != EOF; t++, p->lines++)
        pass0(t, s, " \t");
    
    pass1(p);
}

int main()
{
    program p = {0, 0}; // a new program with length zero
    parseSRC(&p, "../test/sample.txt");

    // printf("%d %x \n", p.lines, p.len);
    // printSymbolTable(&p);
    printPObject(&p);

     return 0;
}

