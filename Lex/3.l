%{
    //identifiers and operators
    #include <stdio.h>    
    enum ops{PLUS,MINUS,MUL,DIV};
    int flags[4]={0,0,0,0};
    int num_operators = 0,num_identifiers = 0;
    char stack[100];
    int top=-1;
    int opflag=0;
    void validate();
%}

%s operand_active

%%


<operand_active>[-]?[_a-zA-Z][_a-zA-Z0-9]* {num_identifiers++;printf("Indentfier: %s\n",yytext);opflag=0; BEGIN(0);}
[_a-zA-Z][_a-zA-Z0-9]* {num_identifiers++;printf("Indentfier: %s\n",yytext);}
<operand_active>[-]?[0-9]+ {num_identifiers++;printf("Constant: %s\n",yytext); opflag=0;BEGIN(0);}
[0-9]+ {num_identifiers++;printf("Constant: %s\n",yytext);}

[+] {flags[PLUS]++; opflag=1;BEGIN(operand_active);}
[-] {flags[MINUS]++; opflag=1;BEGIN(operand_active);}
[/] {flags[DIV]++; opflag=1;BEGIN(operand_active);}
[*] {flags[MUL]++; opflag=1;BEGIN(operand_active);}
[ \t] {;}
[(] {stack[++top]='(';}
[)] {
        if(stack[top] == '(')
        {
            top--;
        }
    }
<operand_active>[;] {validate(); BEGIN(0);}
[;] {validate();}


%%

int main()
{
    printf("Enter an expression(eg a-b;):");
    yylex();    
    return 0;
}

int yywrap()
{
    return 1;
}

void validate()
{
    num_operators = flags[PLUS]+flags[MINUS]+flags[DIV]+flags[MUL];   
    if(num_identifiers==num_operators+1 && top && !opflag)
    {
        printf("\nExpression is valid and contains the following operators: ");
        if(flags[PLUS]) printf("+\t");
        if(flags[MINUS]) printf("-\t");
        if(flags[DIV]) printf("/\t");
        if(flags[MUL]) printf("*\t");
    }
    else
    {
        printf("\nInvalid Expression");
    }
    flags[0] = flags[1] = flags[2] = flags[3]=0;
    num_operators = num_identifiers = 0;
    top = -1;
}