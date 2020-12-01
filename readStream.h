/* GROUP 49
2018A7PS0151P - MUHTASHIM RAFIQI
2018A7PS0208P - ANANYA VARMA
2018A7PS0168P - RISHIRAJ ACCHARYA
2017B1A70902P - ANUSHKA DAYAL
 */

typedef struct TokenList
{
    char c;
    int end;
    struct TokenList* arr[26];
} TokenList;

typedef struct tokenStream
{
    int line_no;
    char lexeme[25],token[25];
    struct tokenStream *next;
    struct tokenStream *prev;
} tokenStream;

TokenList* populate_token_list(char* keywords[]);
void add_token(TokenList *t, char *s);
char* find_token(TokenList *t, char *s);
tokenStream* tokeniseSourcecode(char *fname, tokenStream *s);
