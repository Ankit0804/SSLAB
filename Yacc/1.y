%{

/*1. Develop a YACC program to recognize a valid arithmetic
expression that uses operator +, -, *, /
*/

#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror(char *s);
%}

%token NUM ID NL
%left '+' '-'
%left '*' '/'
%%

S :	E NL {printf("Valid expression!\n"); exit(0);};
E :	  E '+' E
	| E '-' E
    | '-' E
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
	exit(0);
}

int main()
{
	printf("Enter expression: ");
	yyparse();
	return 0;
}	