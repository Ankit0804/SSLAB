%{
	#include <stdio.h>
	int top = -1, numOperators = -1, numOperands = -1, valid = 1;
	char operators[100],operands[10][10], stack[100];
%}

letter	[a-zA-Z]
digit	[0-9]

%%
[({\[]			{ char* val = strdup(yytext); stack[++top] = val[0];}
"}"				{
					if(stack[top] != '{')
						{ valid = 0; return 1; }
					else
						top--;
				}
")"				{
					if(stack[top] != '(')
						{ valid = 0; return 1;  }
					else
						top--;
				}
"]"				{
					if(stack[top] != '[')
						{ valid = 0; return 1;  }
					else
						top--;
				}	
[+\-*/]			{char* val = strdup(yytext); operators[++numOperators] = val[0];}

{digit}+|{letter}({letter}|{digit})*			{ strcpy(operands[++numOperands], yytext);}	

%%

int main(int argc, char* argv[]){
	if(argc < 2)
		{
			printf("Invalid use!\n");
			return 0;
		}
	yyin = fopen(argv[1], "r");
	yylex();
	if(valid){
		//TODO: check if numOperands - numOperators = 1
		printf("Valid!\n");
		printf("Operators are: \n");
		for(int i = 0; i <= numOperators; i++){
			printf("%c\n",operators[i]);
		}
		printf("Operands are: \n");
		for(int i = 0; i <= numOperands; i++){
			printf("%s\n",operands[i]);
		}
	}else{
		printf("Invalid!\n");
	}
}

int yywrap(){
	return 1;
}