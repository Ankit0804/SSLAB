%{
    //identifiers from file
    #include <stdio.h>    
    int num_identifiers = 0;    
%}

%%

[_a-zA-Z][_a-zA-Z0-9]* {num_identifiers++;printf("Indentfier: %s\n",yytext);}
[.]* {;}
\n {;}

%%

int main()
{
    yyin = fopen("test.c","r");
    yylex();   
    printf("Number of identifiers: %d",num_identifiers); 
    return 0;
}

int yywrap()
{
    return 1;
}

