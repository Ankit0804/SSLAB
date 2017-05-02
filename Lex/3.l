%{
    //identifiers and operators
    #include <stdio.h>    
    enum ops{PLUS,MINUS,MUL,DIV};
    int flags[4]={0,0,0,0};
    int num_operators = 0,num_identifiers = 0;
    char stack[100];
    int top=-1;

    void validate();
%}

%%

[_a-zA-Z][_a-zA-Z0-9]* {num_identifiers++;printf("Indentfier: %s\n",yytext);}
[0-9]+ {num_identifiers++;printf("Constant: %s\n",yytext);}
[+] {flags[PLUS]++;}
[-] {flags[MINUS]++;}
[/] {flags[DIV]++;}
[*] {flags[MUL]++;}
[(] {stack[++top]='(';}
[)] {
        if(stack[top] == '(')
        {
            top--;
        }
    }
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
    if(num_identifiers==num_operators+1 && top)
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