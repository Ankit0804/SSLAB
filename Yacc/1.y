%{
#include<stdio.h>
int yylex();
int yyerror(char *s);
%}

%token NUM ID NL
%left '+' '-'
%left '*' '/'

%%
S :	E NL {printf("Valid expression!\n"); return 0;};
    | ID "=" E NL {printf("Valid expression!\n"); return 0;};
E :	  E '+' E
	| E '-' E
	| E '*' E
	| E '/' E
	| '(' E ')'
	| ID
	| NUM
    ;
%%

int yyerror(char *s)
{
	printf("Invalid expression!\n");
	return 1;
}

int main()
{
	printf("Enter expression: ");
	yyparse();
	return 0;
}
