%{//3. Develop a YACC program to evaluate an arithmetic expression
//involving operators +, -, * and /.
%}

%{
#include "y.tab.c"
extern int yylval;
%}

%%
[0-9]+			{yylval = atoi(yytext); return NUM;}
\n			return NL;
.			return *yytext;
%%

int yywrap()
{
	return 1;
}
