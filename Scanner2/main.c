#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 20

char token_buffer[MAX];

void clear_buffer()
{
    for (int i = 0; i < MAX; i++)
    {
        token_buffer[i] = '\0';
    }
}

void buffer_char(char c)
{
    int i;
    for (i = 0; token_buffer[i] != '\0'; i++);
    if (i < MAX - 1)
    {
        token_buffer[i] = c;
    }
}

void check_reserved()
{
    FILE *file;
    file = fopen("resultat.txt", "a");

    if (strcmp(token_buffer, "BEGIN") == 0)
    {
        fprintf(file, "BEGIN\t");
    }
    else if (strcmp(token_buffer, "READ") == 0)
    {
        fprintf(file, "READ\t");
    }
    else if (strcmp(token_buffer, "WRITE") == 0)
    {
        fprintf(file, "WRITE\t");
    }
    else if (strcmp(token_buffer, "END") == 0)
    {
        fprintf(file, "END\t");
    }
    else
    {
        fprintf(file, "IDENTIFIER\t");
    }

    fclose(file);
}

void lexical_error(char c)
{
    FILE *file;
    file = fopen("resultat.txt", "a");

    fprintf(file, "!!!!Erreur de syntaxe: %c", c);

    fclose(file);
}

void scanner()
{
    FILE *file, *file1;
    file = fopen("langage.txt", "r");
    file1 = fopen("resultat.txt", "w");
    fclose(file1);

    int in_char, c;
    clear_buffer();

    while ((in_char = fgetc(file)) != EOF)
    {
        if (!isalpha(in_char)) clear_buffer();

        //if (isspace(in_char)) continue;
        if(in_char=='\n')
            {
                FILE *file1;
                file1 = fopen("resultat.txt", "a");
                fprintf(file1, "\n");
                fclose(file1);
                printf("1");
            }

        else if (isalpha(in_char))
        {
            buffer_char(in_char);
            for (c = fgetc(file); isalnum(c) || c == '_'; c = fgetc(file))
            {
                buffer_char(c);
            }
            ungetc(c, file);
            check_reserved();
        }
        else if (isdigit(in_char))
        {
            c = fgetc(file);
            buffer_char(in_char);
            for (c = fgetc(file); isdigit(c); c = fgetc(file))
            {
                buffer_char(c);
            }
            if (c == '.')
            {
                for (c = fgetc(file); isdigit(c); c = fgetc(file))
                {
                    buffer_char(c);
                }
                if (c == 'e')
                {
                    for (c = fgetc(file); isdigit(c); c = fgetc(file))
                    {
                        buffer_char(c);
                    }
                    if (c == '+' || c == '-')
                    {
                        for (c = fgetc(file); isdigit(c); c = fgetc(file))
                        {
                            buffer_char(c);
                        }
                    }
                }
                if (c == ' ' || c == '+' || c == '-' || c == ';' || c == ')' || !feof(file))
                {
                    FILE *file1;
                    file1 = fopen("resultat.txt", "a");
                    fprintf(file1, "REALLITERAL\t");
                    fclose(file1);
                }
            }
            else if (c == ' ' || c == '+' || c == '-' || c == ';' || c == ')' || !feof(file))
            {
                FILE *file1;
                file1 = fopen("resultat.txt", "a");
                fprintf(file1, "INTLITERAL\t");
                fclose(file1);

            }

            ungetc(c, file);


        }

        else if (in_char == '(')
        {
            file1 = fopen("resultat.txt", "a");
            fprintf(file1,"LPAREN\t");
            fclose(file1);

        }
        else if (in_char == ')')
        {
            file1 = fopen("resultat.txt", "a");

            fprintf(file1,"RPAREN\t");
            fclose(file1);

        }
        else if (in_char == ';')
        {
            file1 = fopen("resultat.txt", "a");

            fprintf(file1,"SEMICOLON\t");
            fclose(file1);

        }
        else if (in_char == ',')
        {
            file1 = fopen("resultat.txt", "a");

            fprintf(file1,"COMMA\t");
            fclose(file1);

        }
        else if (in_char == '+')
        {
            file1 = fopen("resultat.txt", "a");

            fprintf(file1,"PLUSOP\t");
            fclose(file1);

        }
        else if (in_char == '-')
        {
            file1 = fopen("resultat.txt", "a");

            fprintf(file1,"MINUSOP\t");
            fclose(file1);

        }

        else if (in_char == ':')
        {
            c = fgetc(file);
            if (c == '=')
            {
                file1 = fopen("resultat.txt", "a");

                fprintf(file1,"ASSIGNOP\t");
                fclose(file1);

            }
            else
            {

                ungetc(c, file);
                lexical_error(in_char);
                printf("N'est pas definit dans le langage\t");
            }
        }
        else
        {
            //if(c=='.')while((c=fgetc(file))!=" ");
            while((c=fgetc(file))==' ');
            lexical_error(in_char);
            fprintf(file1,"N'est pas definit dans le langage\t");
        }

    }
    file1 = fopen("resultat.txt", "a");

    fprintf(file1,"SCANEOF\t");
    fclose(file1);


}

int main()
{
    scanner();
}

