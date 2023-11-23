#include <stdio.h>
#include <stdlib.h>

char* lire_caractere_par_caractere(char* fichier, int position_debut)
{
    FILE* fp = fopen(fichier, "r");

    fseek(fp, position_debut, SEEK_SET); // déplace le curseur à la position de départ
    int i = 0;
    char c = fgetc(fp); // lit un caractère à la fois

    return c; // renvoie le buffer
}

char c;
int state = 0, start = 0 ;
int lexical_value ;
char mot[10];
int j=0,line=1;


void fail()
{
    switch(start)
    {
        case 0: start = 6;break;
        case 6: start = 12;break;
        case 12:start = 16;break;
        case 16:start = 18;break;
        case 21:start = -1;break;
        default : start=-1;

    }
}

void next_token()
{
    switch(start)
    {
        case 0:
            if(c == 's')
            {

                start = 1;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;

        case 1:
            if(c == 'i')
            {
                start = 2;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();
            }
            break;

        case 2:
            if(c==' ' || c=='\t' || c=='\n' || c==EOF)
            {
                printf("%s : est un mot valide\n", mot);
                for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
                start=0;
                state++;
            }
            else if(c=='n')
            {
                 start=3;
                 mot[j++] = c;
                state++;
            }

            else
                start=16;
            break;

        case 3:
            if(c == 'o')
            {
                start = 4;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();
            }
            break;

        case 4:
            if(c == 'n')
            {
                start = 5;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;

        case 5:
            if(c==' ' || c=='\t' || c=='\n' || c==EOF)
            {
                printf("%s : est un mot valide\n", mot);
                for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
                start=0;
                state++;
            }
            else
            {
                start=16;
            }
            break;

        case 6:
            if(c == 'a')
            {
                start = 7;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;
        case 7:
            if(c == 'l')
            {

                start = 8;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;
        case 8:
            if(c == 'o')
            {

                start = 9;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;
        case 9:
            if(c == 'r')
            {

                start = 10;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;
        case 10:
            if(c == 's')
            {

                start = 11;
                mot[j++] = c;
                state++;
            }
            else
            {
                start=16;

            }
            break;
        case 11:
            if(c==' ' || c=='\t' || c=='\n' || c==EOF)
            {
                printf("%s : est un mot valide\n", mot);
                for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
                start=0;
                state++;
            }
            else
            {
                fail();
            }
            break;
        case 12:
            if(c == '<')
            {

                start = 13;
                mot[j++] = c;
                state++;
            }else if(c == '>')
            {

                start = 14;
                mot[j++] = c;
                state++;
            }else if(c == '=')
            {

                start = 15;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;
            case 13:
            if(c == '>' || c == '=')
            {

                start = 15;
                mot[j++] = c;
                state++;
            }else if(c==' ' || c=='\t' || c=='\n' || c==EOF)
            {
                printf("%s : est un operateur\n", mot);
                for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
                start=0;
                state++;
            }
            else
            {
                fail();

            }
            break;
            case 14:
            if(c == '=')
            {

                start = 15;
                mot[j++] = c;
                state++;
            }else if(c==' ' || c=='\t' || c=='\n' || c==EOF)
            {
                printf("%s : est un operateur\n", mot);
                for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
                start=0;
                state++;
            }
            else
            {
                fail();

            }
            break;
            case 15:
            if(c==' ' || c=='\t' || c=='\n' || c==EOF)
            {
                printf("%s : est un operateur\n", mot);
                for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
                start=0;
                state++;
            }
            else
            {
                fail();
            }
            break;

            case 16:
            if(isalpha(c))
            {

                start = 17;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;
            case 17:
            if(isalpha(c) || isdigit(c))
            {
                start = 17;
                mot[j++] = c;
                state++;
            }else if(c==' ' || c=='\t' || c=='\n' || c==EOF)
            {
                printf("%s : est un identificateur\n", mot);
                for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
                start=0;
                state++;
            }
            else
            {
                fail();

            }
            break;

            case 18:
            if(isdigit(c))
            {
                start = 19;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;
            case 19:
            if(isdigit(c))
            {
                start = 19;
                mot[j++] = c;
                state++;
            }else if(c==' ' || c=='\t' || c=='\n' || c==EOF)
            {
                printf("%s : est un entier\n", mot);
                for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
                start=0;
                state++;
            }else if(c=='.')
            {
                start = 20;
                mot[j++] = c;
                state++;
            }else if(c=='E' || c=='e')
            {
                start = 21;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;
            case 20:
            if(isdigit(c))
            {
                start = 22;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;
            case 21:
            if(isdigit(c))
            {
                start = 23;
                mot[j++] = c;
                state++;
            }else if(c=='+' || c=='-')
            {
                start = 23;
                mot[j++] = c;
                state++;
            }
            else
            {
                fail();

            }
            break;
            case 22:
            if(isdigit(c))
            {
                start = 22;
                mot[j++] = c;
                state++;
            }else if(c=='E' || c=='e')
            {
                start = 21;
                mot[j++] = c;
                state++;
            }else if(c==' ' || c=='\t' || c=='\n' || c==EOF)
            {
                printf("%s : est un float\n", mot);
                for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
                start=0;
                state++;
            }
            else
            {
                fail();

            }
            break;
            case 23:
            if(isdigit(c))
            {
                start = 23;
                mot[j++] = c;
                state++;
            }else if(c==' ' || c=='\t' || c=='\n' || c==EOF)
            {
                printf("%s : est un float\n", mot);
                for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
                start=0;
                state++;
            }
            else
            {
                fail();

            }
            break;

        case -1:
            if(c=='\n')
                    line++;
            if(c!=' ' && c!='\n')
            {
            printf("Erreur dans la ligne %d\n",line);
            while(c!=' ' && c!='\n')
            {
                c = lire_caractere_par_caractere("Fichier.txt", state);
                state++;

            }
            }else
            {
                state++;

            }


            for(int i=0;i<10;i++)mot[i]='\0';
                j=0;
            start=0;

            break;
    }

}

int main()
{
    while(c != EOF)
    {

        c = lire_caractere_par_caractere("Fichier.txt", state);

        next_token();

    }



}
