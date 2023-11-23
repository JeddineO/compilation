%{
#include <stdio.h>
%}
comment \/\*(.|\n)*\*\/
%%
{comment} {printf("|%s|est un commentaire \n",yytext);}
.* {printf("|%s|est erreur \n",yytext);}
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