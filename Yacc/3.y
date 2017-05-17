%{
#include <stdio.h>
int yylex();
int yyerror(char *s);
%}

%token NUM NL
%left '+' '-'
%left '*' '/'
%%

S :	E NL {printf("Value: %d\n", $1); return 0;};
E :	  E '+' E	{$$=$1+$3;}
	| E '-' E	{$$=$1-$3;}
	| E '*' E	{$$=$1*$3;}
	| E '/' E	{if($3 == 0)
			{
				printf("Cannot divide by zero!\n");
				return 0;
			}
			else
				$$=$1/$3;
            }
	| '(' E ')'	{$$=$2;}
	| NUM		{$$=$1;}
	;
%%

int yyerror(char *s)
{
	printf("Invalid expression!\n");
	return 0;
}

int main()
{
	printf("Enter expression: ");
	yyparse();
	return 0;
}
