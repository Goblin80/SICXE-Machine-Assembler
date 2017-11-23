#include <stdio.h>
#include <string.h>

#define pSIZE 1024
#define sSIZE 30
#define rSIZE 0x1E
#define isStrEq(a, b) !strcmp(a, b)

#include "lib/declareDATA.h"
#include "lib/xtraSTR.h"
#include "lib/MDAFs.h"
#include "lib/formatPARSER.h"
#include "lib/passSICXE.h"


void printSymbolTable(program *p)
{
    symbolEntry *e = p->st.entry;
    for(int i = 0; i < 17; i++) printf("%c", 215); printf("\n");

    for(int i = 0; i < p->st.len; i++, e++)
        printf("%c %04X %c %-6s %c\n", 186, e->value, 186, e->symbol, 186);

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
    {
        pass0(p, t, s, " \t");

        if(isStrEq(t->operator.mnemonic, "LTORG")) // Make room for literals
        {
            t += p->literalCount;
            p->lines += p->literalCount;
            p->literalCount = 0;
        }
    }
    
    pass1(p);
    pass2(p);
}

int main()
{
    program p; // a new program with length zero
    memset(&p, 0, sizeof p); // playing safe
    // parseSRC(&p, "../test/sample.txt");
    parseSRC(&p, "../test/sample2.txt");

    // printSymbolTable(&p);
    printPObject(&p);
    // genHTE(&p);
     return 0;
}

