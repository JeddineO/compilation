%{
#include <stdio.h>
%}
mot1  .*a.*e.*i.*o.*u.*
mot2  a(a|b)*|(a|b)*a
mot3     ^(ab|ba|(ab|ba)*(a|b))$
mot4  ^(b*ab*)*$ 
%%
{mot1} { printf("mot 1  valide: %s\n", yytext); } ;
{mot2} { printf("mot 2  valide: %s\n", yytext); } ;
{mot3} { printf("mot 3  valide: %s\n", yytext); } ;
{mot4} { printf("mot 4  valide: %s\n", yytext); } ;
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