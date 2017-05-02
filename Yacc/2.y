//2. Develop a YACC program to recognize a valid variable, which
//starts with a letter, followed by any number of letters or digits

%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror(char *s);
%}

%token NUM LET NL
%%

S : LET A NL {printf("Valid variable!\n"); exit(0);};
A : LET A
  | NUM A
  | LET
  | NUM
  ;
%%

int yyerror(char *s)
{
	printf("Invalid variable!\n");
	exit(0);
}

int main()
{
	printf("Enter variable: ");
	yyparse();
	return 0;
}
