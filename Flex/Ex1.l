%{
#include <stdio.h>
%}
%%
"&"           {printf("symbole : %s\n",yytext);}
"["           {printf("symbole : %s \n",yytext);}
"]"           {printf("symbole : %s \n",yytext);}
")" 	      {printf("symbole : %s \n",yytext);}
"(" 	      {printf("symbole : %s \n",yytext);}
"}" 	      {printf("symbole : %s \n",yytext);}
"{"           {printf("symbole : %s \n",yytext);}
"-" 	      {printf("operateur : %s\n",yytext);}
"+" 	      {printf("operateur : %s\n",yytext);}
"=" 	      {printf("operateur : %s\n",yytext);}
"fscanf"      {printf("mots_clefs : %s\n",yytext);}
"scanf"       {printf("mots_clefs : %s\n",yytext);}
"fprintf"     {printf("mots_clefs : %s\n",yytext);}
"printf"      {printf("mots_clefs : %s\n",yytext);}
"do"          {printf("mots_clefs : %s\n",yytext);}
"while"       {printf("mots_clefs : %s\n",yytext);}
"switch"      {printf("mots_clefs : %s\n",yytext);}
"else if"     {printf("mots_clefs : %s\n",yytext);}
"else"        {printf("mots_clefs : %s\n",yytext);}
"if"          {printf("mots_clefs : %s\n",yytext);}
"for"         {printf("mots_clefs : %s\n",yytext);}
[0-9]+(\.[0-9]+)|(E(\+|\-)?[0-9]+)[ \n\t] {printf("Float: %s  ",yytext);}
[0-9]+(\.[0-9]+)(E(\+|\-)?[0-9]+)[ \n\t] {printf("Float : %s  ",yytext);}
[0-9]+[ \n\t]         { printf("Nombre entier : %s\n", yytext); }
%%
int yywrap()
{
    return 1;
}
int main(int argc, char** argv)
{
    FILE* fichier = NULL;
    if (argc == 2 && (fichier = fopen(argv[1], "r")) != NULL) {
        yyin = fichier;
    } else {
        printf("Erreur : aucun fichier d'entrée spécifié.\n");
        return 1;
    }
    yylex();
    fclose(fichier);
    return 0;
}
