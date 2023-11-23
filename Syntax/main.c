#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_WORD_SIZE 100


char word[MAX_WORD_SIZE];


int read_file(FILE* fp,char word[MAX_WORD_SIZE])
{
    int c, i = 0;
    while ((c = fgetc(fp)) != EOF)
    {

        if (isalpha(c))
        {
            word[i++] = c;
        }
        else
        {
            if (i > 0)
            {
                word[i] = '\0';
                i = 0;
                return ((int)1);
            }
        }
    }

    fclose(fp);
}

void match(char* expected_token,char* lookahead) {


    if (strcmp(lookahead, expected_token) == 0) {
        printf("Matcher %s\n", lookahead);
    } else {
        printf("Erreur de syntax: %s | %s\n", expected_token, lookahead);
        exit(1);
    }
}
void system_goal()
{

    FILE *fp;
    fp = fopen("resultat.txt", "r");
    read_file(fp,word);
    program(word,fp);
    read_file(fp,word);
    match("SCANEOF",word);
}

void program(char* word,FILE *fp)
{
    match("BEGIN",word);
    read_file(fp,word);
    inst_list(word,fp);
    read_file(fp,word);
    match("END",word);
}

void inst_list(char* word,FILE *fp)
{

     if (strcmp("IDENTIFIER", word) == 0)
     {
         inst(word,1,fp);
         read_file(fp,word);

     }
     if (strcmp("READ", word) == 0)
     {
         inst(word,2,fp);
         read_file(fp,word);

     }
     if (strcmp("WRITE", word) == 0)
     {
         inst(word,3,fp);
         read_file(fp,word);
     }
     else
     {
         printf("Ereure de de sysntaxe à la ligne %s:\n",word);

     }
}





void inst(char* word,int a,FILE *fp) {
    switch (a) {
        case 1:
            match("IDENTIFIER", word);
            read_file(fp,word);
            match("ASSIGNOP", word);
            //expression();
            match("SEMICOLON", word);
            break;
        case 2:
            match("READ", word);
            read_file(fp,word);
            match("LPAREN", word);
            read_file(fp,word);
            id_list(word,fp);
            match("RPAREN", word);
            read_file(fp,word);
            match("SEMICOLON", word);
            if(strcmp("SEMICOLON", word) == 0)
            {
                read_file(fp,word);
                inst_list(word,fp);
            }
            break;
        case 3:
            match("WRITE", word);
            read_file(fp,word);
            match("LPAREN", word);
//            expr_list();
            match("RPAREN", word);
            match("SEMICOLON", word);
            break;
        default:
            printf("Ereure de de sysntaxe le 'instruction  %s:\n",word);

            break;
    }
}

void id_list(char* word,FILE *fp) {
    match("IDENTIFIER", word);
    read_file(fp,word);
    while (strcmp("COMMA", word) == 0) {
           match("COMMA", word);
        read_file(fp,word);
        match("IDENTIFIER", word);
        read_file(fp,word);
    }
}
//
//void expression(void)
//{
//
//    for(t = next_token(); t = PLUSOP || t == MINSOP; t =next_token())
//    {
//        add_opp();
//        if (strcmp("ID", word) == 0) prim(word,1,fp);
//     if (strcmp("READ", word) == 0) prim(word,2,fp);
//     if (strcmp("WRITE", word) == 0) prim(word,3,fp);
//     else
//     {
//         printf("Ereure de de sysntaxe le 'instruction ne peut pas commencer par %s:\n",word);
//
//     }
//
//    }
//
//}
//void expr_list(char* word)
//{
//    expression();
//    while(next_token()==COMMA)
//    {
//        match(COMMA,word);
//        expression();
//    }
//
//}
//void add_opp(char* word) {
//    token t = next_token();
//    if (t == PLUSOP || t == MINSOP) {
//        match(token_to_string(t), word);
//    } else {
//        syntax_error(t);
//    }
//}
//
//void prim(char* word) {
//    token tok = next_token();
//    switch (tok) {
//        case LPAREN:
//            match("LPAREN", word);
//            expression();
//            match("RPAREN", word);
//            break;
//        case ID:
//            match("ID", word);
//            break;
//        case INTLITERAL:
//            match("INTLITERAL", word);
//            break;
//        default:
//            syntax_error(tok);
//            break;
//    }
//}
//
//



void main()
{
    system_goal();
}
