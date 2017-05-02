//4. Develop a YACC program to recognize the strings of the
//form a^mb^n, where m>=0, n>=0.

%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror(char *s);
%}

%token A B NL
%%

S : C D NL {printf("Valid string!\n"); exit(0);}

C : A C |
  
D : B D |
  ;
%%

int yyerror(char *s)
{
	printf("Invalid string!\n");
	exit(0);
}

int main()
{
	printf("Enter the string: ");
	yyparse();
	return 0;
}
