%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aps.tab.h"

int charat = 1;
int lineat = 1;
%}

ident           [a-z]+
nombre 		[0-9]+
comm            #.*?\n

%%

"True"    {  charat += 4; return TOKEN_TRUE;  }
"False"   {  charat += 5; return TOKEN_FALSE; }

"not"     {  charat += 3; return TOKEN_NOT;   }
"and"     {  charat += 3; return TOKEN_AND;   }
"or"      {  charat += 2; return TOKEN_OR;    }

"eq"      {  charat += 2; return TOKEN_EQ;  }
"lt"      {  charat += 2; return TOKEN_LT;  }

"add"     {  charat += 3; return TOKEN_ADD; }
"sub"     {  charat += 3; return TOKEN_SUB; }
"div"     {  charat += 3; return TOKEN_DIV; }
"mul"     {  charat += 3; return TOKEN_MUL; }
"mod"     {  charat += 3; return TOKEN_MOD; }

"void"    {  charat += 4; return TOKEN_VOID; }
"bool"    {  charat += 4; return TOKEN_BOOL; }
"int"     {  charat += 3; return TOKEN_INT;  }

{ident}     { charat += strlen(yytext); yylval.str    = strdup(yytext);  return TOKEN_IDENT; }
{nombre}    { charat += strlen(yytext); yylval.entier = atoi(yytext);   return TOKEN_NOMBRE; }
{comm}    { lineat++; }  /* commentaires à la PERL ignorés */

"WHILE"   {  charat += 5; return TOKEN_WHILE; }
"SET"     {  charat += 3; return TOKEN_SET;   }
"IF"      {  charat += 2; return TOKEN_IF;    }
"ECHO"    {  charat += 4; return TOKEN_ECHO;  }

"VAR"     {  charat += 3; return TOKEN_VAR;   }
"CONST"   {  charat += 5; return TOKEN_CONST; }

[ \t]     { charat++; } /* caractères d'espacement ignorés */
[\n]      { lineat++; charat = 1; }

.         { charat++; return yytext[0]; } /* les '[' et ']' renvoyés ici */

%%
