%{
    //comments
    #include <stdio.h>
    int count=0,lines=0;
%}

%x blockcomment

%%

"/*"   {count++; BEGIN(blockcomment);}
<blockcomment>[\n] {count++;lines++;}
<blockcomment>\*\/ {BEGIN(INITIAL);}
<blockcomment>. {;}
\/\/.*    {count++;}
\n { lines++; }
.   {;}



%%

int main()
{
    yyin = fopen("test.c","r");
    yylex();
    printf("\nCommented Lines: %d of %d\n",count,lines);        
}