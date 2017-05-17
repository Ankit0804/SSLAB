%{//4. Develop a YACC program to recognize the strings of the
//form a^mb^n, where m>=0, n>=0.
%}

%{
#include "y.tab.c"
%}

%%
a	return A;
b	return B;
\n	return NL;
.	return yytext[0];
%% 

int yywrap()
{
	return 1;
}
