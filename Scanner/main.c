#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


int i=0;

typedef enum
{
    BEGIN, END, READ, WRITE, ID, SCANEOF, INTLETERAL, LPAREN, RPAREN,
    SEMICOLON, COMMA, PLUSOP, ASSIGNOP, MINUSOP, EOL, ERROR, FLOATLETERAL
} token;
// Prototyping =================================================================
FILE *Open_File(char *, char *);
token check_reserved(char *);
void lexical_error(int, int );
token Scanner(FILE *, int *);
void Transforme_Token(FILE *, token );
void Lex();
char *token_get_name(token );
token to_token(char *);
int match(FILE *, token );
token next_token(FILE *);
void syntax_error(token,int );
void system_goal(FILE *);
void program(FILE *);
void inst_List(FILE *);
void inst(FILE *);
void id_List(FILE *);
void expr(FILE *);
void expr_List(FILE *);
void add_op(FILE *);
void prim(FILE *);
void Syn();
// =============================================================================
FILE *Open_File(char *filename, char *mode)
{
    FILE *file = fopen(filename,mode);
    if (!file)
    {
        printf("\nErreur lors de l'ouverture de fichier %s",filename);
        exit(0);
    }
    return ((FILE *)file);
}
// =============================================================================
token check_reserved(char lexem[8])
{
    char *KeyWord[4] = {"BEGIN", "END", "READ", "WRITE"};
    int i;
    for (i = 0; i < 4; i++)
        if (stricmp(lexem,KeyWord[i]) == 0) break;
    switch (i)
    {
    case 0:
        return BEGIN;
    case 1:
        return END;
    case 2:
        return READ;
    case 3:
        return WRITE;
    default :
        return ID;
    }
}
// =============================================================================
void lexical_error(int in_char, int line_n)
{
    printf("\nLexical error at line %d : %c",line_n,(char)in_char);
}
// =============================================================================
token Scanner(FILE *file, int *err_char)
{
    int in_char, c, i;
    char lexem[8]; // To check reserved words
    if (!file) exit(0);
    while ((in_char = fgetc(file)) != EOF)
    {
        if (isspace(in_char))
        {
            if (in_char == (int)'\n') return EOL;
            continue;
        }
        if (isalpha(in_char))
        {
            // ID ::= LETTER | ID LETTER | ID DIGIT | ID UNDERSCORE
            lexem[0] = (char)in_char;
            for (i = 1; i < 7; i++)
            {
                // Check reserved words
                c = fgetc(file);
                if ((isalnum(c) == 0) && (c != '_')) break;
                lexem[i] = (char)c;
            }
            if (i < 7)
            {
                lexem[i] = '\0';
                if (!feof(file)) fseek(file,-1,SEEK_CUR);
                return check_reserved(lexem);
            }
            while ((isalnum(c)) || (c == '_')) c = fgetc(file);
            if (!feof(file)) fseek(file,-1,SEEK_CUR);
            return ID;
        }
        if (isdigit(in_char))
        {
            // INTLETERAL ::= DIGIT | INTLETERAL DIGIT
            while (isdigit(c = fgetc(file)));
            if (c == '.')
            {
                while (isdigit(c = fgetc(file)));
                if (c == 'E' || c == 'e')
                {
                    c = fgetc(file);
                    if (c == '+' || c == '-')
                    {
                        if (!isdigit(c = fgetc(file)))
                        {
                            if (!feof(file)) fseek(file,-3,SEEK_CUR);
                            else fseek(file,-2,SEEK_CUR);
                        }
                        else
                        {
                            while (isdigit(c = fgetc(file)));
                            if (!feof(file)) fseek(file,-1,SEEK_CUR);
                        }
                    }
                    else if (isdigit(c))
                    {
                        while (isdigit(fgetc(file)));
                        if (!feof(file)) fseek(file,-1,SEEK_CUR);
                    }
                    else if (!feof(file)) fseek(file,-2,SEEK_CUR);
                    else fseek(file,-1,SEEK_CUR);
                }
                return FLOATLETERAL;
            }
            if (!feof(file)) fseek(file,-1,SEEK_CUR);
            return INTLETERAL;
        }
        if (in_char == '.')
        {
            c = fgetc(file);
            if (!isdigit(c))
            {
                *err_char = c;
                if (!feof(file)) fseek(file,-1,SEEK_CUR);
                return ERROR;
            }
            while (isdigit(c = fgetc(file)));
            if (c == 'E' || c == 'e')
            {
                c = fgetc(file);
                if (c == '+' || c == '-')
                {
                    if (!isdigit(c = fgetc(file)))
                    {
                        if (!feof(file)) fseek(file,-3,SEEK_CUR);
                        else fseek(file,-2,SEEK_CUR);
                    }
                    else
                    {
                        while (isdigit(c = fgetc(file)));
                        if (!feof(file)) fseek(file,-1,SEEK_CUR);
                    }
                }
                else if (isdigit(c))
                {
                    while (isdigit(fgetc(file)));
                    if (!feof(file)) fseek(file,-1,SEEK_CUR);
                }
                else if (!feof(file)) fseek(file,-2,SEEK_CUR);
                else fseek(file,-1,SEEK_CUR);
            }
            return FLOATLETERAL;
        }
        if (in_char == '(') return LPAREN;
        if (in_char == ')') return RPAREN;
        if (in_char == ';') return SEMICOLON;
        if (in_char == ',') return COMMA;
        if (in_char == '+') return PLUSOP;
        if (in_char == ':')
        {
            // look for :=
            c = fgetc(file);
            if (c == '=') return ASSIGNOP;
            if (!feof(file)) fseek(file,-1,SEEK_CUR);
            *err_char = in_char;
            return ERROR;
        }
        if (in_char == '-')
        {
            // is it --, comment start
            c = fgetc(file);
            if (c == '-')
            {
                // Ignore comment
                while ((c = fgetc(file)) != '\n');
                return EOL;
            }
            if (!feof(file)) fseek(file,-1,SEEK_CUR);
            return MINUSOP;
        }
        *err_char = in_char;
        return ERROR;
    } // fin while
    return SCANEOF;
}
// =============================================================================
void Transforme_Token(FILE *file, token tk)
{
    if (!file) exit(0);
    switch (tk)
    {
    case BEGIN:
        fprintf(file,"BEGIN ");
        break;
    case END:
        fprintf(file,"END ");
        break;
    case READ:
        fprintf(file,"READ ");
        break;
    case WRITE:
        fprintf(file,"WRITE ");
        break;
    case ID:
        fprintf(file,"ID ");
        break;
    case INTLETERAL:
        fprintf(file,"INTLETERAL ");
        break;
    case FLOATLETERAL:
        fprintf(file,"FLOATLETERAL ");
        break;
    case ASSIGNOP:
        fprintf(file,"ASSIGNOP ");
        break;
    case LPAREN:
        fprintf(file,"LPAREN ");
        break;
    case RPAREN:
        fprintf(file,"RPAREN ");
        break;
    case SEMICOLON:
        fprintf(file,"SEMICOLON ");
        break;
    case COMMA:
        fprintf(file,"COMMA ");
        break;
    case PLUSOP:
        fprintf(file,"PLUSOP ");
        break;
    case MINUSOP:
        fprintf(file,"MINUSOP ");
        break;
    case SCANEOF:
        fprintf(file,"SCANEOF ");
        break;
    default :
        printf("\nSomething is wrong");
    }
}
// =============================================================================
void Lex()
{
    FILE *Sourcefile = Open_File("langage.txt","r");
    FILE *CodeULfile = Open_File("resultat.txt","w");
    int line_n = 1, err_char;
    token tk;
    do
    {
        tk = Scanner(Sourcefile,&err_char);
        if (tk == EOL)
        {
            i++;
            //printf("%d",i);
            line_n++;
            fprintf(CodeULfile,"\n");
        }
        else if (tk == ERROR) lexical_error(err_char,line_n);
        else Transforme_Token(CodeULfile,tk);
    }
    while (tk != SCANEOF);
    fclose(Sourcefile);
    fclose(CodeULfile);
}
// =============================================================================
char *token_get_name(token tk)
{
    switch (tk)
    {
    case BEGIN:
        return ((char *)"BEGIN");
    case END:
        return ((char *)"END");
    case READ:
        return ((char *)"READ");
    case WRITE:
        return ((char *)"WRITE");
    case ID:
        return ((char *)"ID");
    case INTLETERAL:
        return ((char *)"INTLETERAL");
    case FLOATLETERAL:
        return ((char *)"FLOATLETERAL");
    case ASSIGNOP:
        return ((char *)"ASSIGNOP");
    case LPAREN:
        return ((char *)"LPAREN");
    case RPAREN:
        return ((char *)"RPAREN");
    case SEMICOLON:
        return ((char *)"SEMICOLON");
    case COMMA:
        return ((char *)"COMMA");
    case PLUSOP:
        return ((char *)"PLUSOP");
    case MINUSOP:
        return ((char *)"MINUSOP");
    case SCANEOF:
        return ((char *)"SCANEOF");
    default :
        printf("\nSomething is wrong");
        exit(0);
    }
}
// =============================================================================
token to_token(char *tok)
{
    if (!strcmp(tok,"BEGIN")) return BEGIN;
    if (!strcmp(tok,"END")) return END;
    if (!strcmp(tok,"READ")) return READ;
    if (!strcmp(tok,"WRITE")) return WRITE;
    if (!strcmp(tok,"ID")) return ID;
    if (!strcmp(tok,"INTLETERAL")) return INTLETERAL;
    if (!strcmp(tok,"FLOATLETERAL")) return FLOATLETERAL;
    if (!strcmp(tok,"ASSIGNOP")) return ASSIGNOP;
    if (!strcmp(tok,"LPAREN")) return LPAREN;
    if (!strcmp(tok,"RPAREN")) return RPAREN;
    if (!strcmp(tok,"SEMICOLON")) return SEMICOLON;
    if (!strcmp(tok,"COMMA")) return COMMA;
    if (!strcmp(tok,"PLUSOP")) return PLUSOP;
    if (!strcmp(tok,"MINUSOP")) return MINUSOP;
    if (!strcmp(tok,"SCANEOF")) return SCANEOF;
    printf("\nSomething is wrong!");
    exit(0);
}
// =============================================================================
int match(FILE *file, token tk)
{
    char t[18];
    int l = ftell(file);
    if (fscanf(file,"%18[^ ] ",t) != EOF)
    {
        if (!strcmp(token_get_name(tk),t)) return 1;
    }
    fseek(file,l,SEEK_SET);
    return 0;
}
// =============================================================================
token next_token(FILE *file)
{
    char t[18];
    int l = ftell(file);
    if (fscanf(file,"%18[^ ] ",t) != EOF)
    {
        fseek(file,l,SEEK_SET);
        return to_token(t);
    }
    return SCANEOF;
}
// =============================================================================
void syntax_error(token tk, int err_t)
{
    switch (err_t)
    {
    case 0:
        printf("\nSyntax error line %d: \"%s\" not found",i,token_get_name(tk));
        break;
    case 1:
        printf("\nSyntax error before \"%s\": {ID, READ or WRITE} not found",
        token_get_name(tk));
        break;
    case 2:
        printf("\nSyntax error before \"%s\": {PLUSOP or MINUSOP} not found",
               token_get_name(tk));
        break;
    case 3:
        printf("\nSyntax error before \"%s\": {ID, LPARENT, INTLETERAL or FLOATLETERAL} not found",
               token_get_name(tk));
        break;

    }
}
// =============================================================================
void system_goal(FILE *file)
{
    // <systemgoal> -> <program> SCANEOF
    program(file);
    if (!match(file, SCANEOF)) syntax_error(SCANEOF, 0);
}
// =============================================================================
void program(FILE *file)
{
    // <program> -> begin <inst_List> end
    if (!match(file, BEGIN)) syntax_error(BEGIN, 0);
    inst_List(file);
    if (!match(file, END)) syntax_error(END, 0);
}
// =============================================================================
void inst_List(FILE *file)
{
    // <inst_List> -> <inst> {<inst>}
    token tk;
    inst(file);
    while (1)
    {
        switch (tk = next_token(file))
        {
        case ID:
        case READ:
            i++;
        case WRITE:
            i++;
            inst(file);
            break;
        default:
            while(tk!=ID || tk!=READ || tk!=WRITE)
            {
                 tk = next_token(file);
            }
        }


    }
}
// =============================================================================
void inst(FILE *file)
{
    // <inst> -> id := <expr>; | read(<id_List>); | write(<expr_List>);
    token tk = next_token(file);
    switch (tk)
    {
    case ID:
        match(file, ID);
        if (!match(file, ASSIGNOP)) syntax_error(ASSIGNOP, 0);
        expr(file);
        if (!match(file, SEMICOLON)) syntax_error( SEMICOLON, 0);
        break;
    case READ:
        match(file, READ);
        if (!match(file, LPAREN)) syntax_error(LPAREN, 0);
        id_List(file);
        if (!match(file, RPAREN)) syntax_error(RPAREN, 0);
        if (!match(file, SEMICOLON)) syntax_error(SEMICOLON, 0);
        break;
    case WRITE:
        match(file, WRITE);
        if (!match(file, LPAREN)) syntax_error(LPAREN, 0);
        expr_List(file);
        if (!match(file, RPAREN)) syntax_error(RPAREN, 0);
        if (!match(file, SEMICOLON)) syntax_error(SEMICOLON, 0);
        break;
    default:
        syntax_error(tk, 1);
    }
}
// =============================================================================
void id_List(FILE *file)
{
    // <id_List> -> id {, id}
    token tk;
    if (!match(file, ID)) syntax_error(ID, 0);
    while ((tk = next_token(file)) == COMMA)
    {
        match(file,COMMA);
        if (!match(file, ID)) syntax_error(ID, 0);
    }
}
// =============================================================================
void expr(FILE *file)
{
    // <expr> -> <prim> {<add_op> <prim>}
    token tk;
    prim(file);
    for (tk = next_token(file); (tk == PLUSOP) || (tk == MINUSOP); tk = next_token(file))
    {
        add_op(file);
        prim(file);
    }
}
// =============================================================================
void expr_List(FILE *file)
{
    // <expr_List> -> <expr> {, <expr>}
    token tk;
    expr(file);
    while ((tk = next_token(file)) == COMMA)
    {
        match(file, COMMA);
        expr(file);
    }
}
// =============================================================================
void add_op(FILE *file)
{
    // <add_op> -> plusop | minusop
    token tk = next_token(file);
    if ((tk == PLUSOP) || (tk == MINUSOP)) match(file,tk);
    else syntax_error(tk, 2);
}
// =============================================================================
void prim(FILE *file)
{
    // <prim> -> id | (<expr>) | intleteral | floatleteral
    token tk = next_token(file);
    switch (tk)
    {
    case ID:
        match(file, ID);
        break;
    case LPAREN:
        match(file, LPAREN);
        expr(file);
        if (!match(file,RPAREN)) syntax_error(RPAREN, 0);
        break;
    case INTLETERAL:
        match(file, INTLETERAL);
        break;
    case FLOATLETERAL:
        match(file, FLOATLETERAL);
        break;
    default:
        syntax_error(tk, 3);
    }
}
// =============================================================================
void Syn()
{
    FILE *file = Open_File("resultat.txt","r");
    system_goal(file);
    fclose(file);
}


void main()
{
    int choix;
    printf("\nCompilation");
    do
    {
        printf("\n0. Arreter le programme");
        printf("\n1. Analyse lexical");
        printf("\n2. Analyse syntaxique");
        printf("\n--> : ");
        scanf("%d",&choix);
        switch (choix)
        {
        case 0:
            break;
        case 1:
            Lex();
            break;
        case 2:
            Syn();
            break;
        default:
            printf("\nChoix non valide");
        }
    }
    while (choix);
}
