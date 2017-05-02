%{
    //characters,words,spaces,lines
    #include <stdio.h>
    int chars=0,words=0,lines=0,spaces=0;    
%}

%%

[^ \t\n]+ { chars+=yyleng;words++;}
[\n] { lines++; }
[ \t]+ {spaces+=yyleng;}



%%

int main()
{
    yyin = fopen("data.txt","r");
    yylex();
    printf("\nCharacters: %d",chars);
    printf("\nWords: %d",words);
    printf("\nLines: %d",lines);
    printf("\nSpaces: %d",spaces);
}