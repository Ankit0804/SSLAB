%{//1. Develop a YACC program to recognize a valid arithmetic
//expression that uses operator +, -, *, /
%}

%{
#include "y.tab.c"
%}

%%
[0-9]+			return NUM;
[a-zA-Z_][a-zA-Z0-9_]*	return ID;
\n			return NL;
.			return *yytext;
%%

int yywrap()
{
	return 1;
}
