%{//2. Develop a YACC program to recognize a valid variable, which
//starts with a letter, followed by any number of letters or digits
%}

%{
#include "y.tab.c"
%}

%%
[0-9]+			return NUM;
[a-zA-Z]		return LET;
\n			return NL;
.			return yytext[0];
%%

int yywrap()
{
	return 1;
}
