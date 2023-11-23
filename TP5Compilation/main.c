#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 100


char stack[STACK_SIZE];
int top = -1;


void affichage()
{
    for(int x=0;x<=top;x++)
    {
        printf("%c",stack[x]);
    }
}

void push(char c)
{
    if (top < STACK_SIZE - 1)
    {
        stack[++top] = c;
    }
    else
    {
        printf("Erreur: pile pleine\n");
        exit(1);
    }
}

char pop()
{
    if (top >= 0)
    {
        return stack[top--];
    }
    else
    {
        printf("Erreur: pile vide\n");
        exit(1);
    }
}

char peek()
{
    if (top >= 0)
    {
        return stack[top];
    }
    else
    {
        printf("Erreur: pile vide\n");
        exit(1);
    }
}

int is_terminal(char c)
{
    return (c == '(' || c == ')' || c == 'a' || c == 'b' || c == '+' || c == '*' || c == '$');
}

int get_row(char c)
{
    switch (c)
    {
        case 'R': return 0;
        case 'X': return 1;
        case 'A': return 2;
        case 'Y': return 3;
        case 'B': return 4;
        case 'Z': return 5;
        case 'C': return 6;
        default: return -1;
    }
}

int get_col(char c)
{
    switch (c)
    {
        case 'a': return 0;
        case 'b': return 1;
        case '*': return 2;
        case '+': return 3;
        case '(': return 4;
        case ')': return 5;
        case '$': return 6;
        default: return -1;
    }
}




char *table[7][7] =
{
    {"AX","AX",NULL, NULL,"AX", NULL, NULL},
    {NULL, NULL,NULL, "+AX", NULL, "ε", "ε"},
    {"BY", "BY",NULL,NULL,"BY",NULL , NULL},
    {"BY", "BY",NULL,"ε","BY","ε","ε"},
    {"CZ", "CZ",NULL, NULL, "CZ", NULL, NULL},
    {"ε","ε","*Z","ε","ε", "ε","ε"},
    {"a", "b",NULL, NULL, "(R)",NULL, NULL}
};
void parse(char *input)

{
    int len = strlen(input);
    int i = 0;


    input[len]='$';
    len++;
    input[len]='\0';
    printf("la chaine entree est : %s\n",input);
    push('$');
    push('R');



    while (i < len && top >= 0)
    {
    affichage();

        char x = peek();
        char c = input[i];

        if (x == c)
        {
            top--;
            i++;
        }
        else if (is_terminal(x))
        {
            printf("Erreur de syntaxe 1\n");
            return;
        }
        else
        {

            int row = get_row(x);
            int col = get_col(c);
            char *prod = table[row][col];


            if (prod == NULL)
            {

                printf("Erreur de syntaxe 2\n");
                return;
            }

            top--;

            if (strcmp(prod,"ε")!=0)
            {
                int len1 = strlen(prod);
                int j;

                for (j = len1 - 1; j >= 0; j--)
                {
                    push(prod[j]);
                }
                printf("\t | %s",prod);
            }else{
                printf("\t | Eps");
            }
        }
        printf("\t | %c",input[i]);
        printf("\n");
    }

    if (i == len && top == -1)
    {
        printf("\nChaine syntaxiquement correcte\n");
    }
    else
    {
        printf("Erreur de syntaxe3\n");
    }
}



int main()
{
char input[100];

printf("Entrez une chaine : ");
scanf("%s", input);

parse(input);

return 0;
}
