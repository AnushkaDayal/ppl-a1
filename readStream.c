/* GROUP 49
2018A7PS0151P - MUHTASHIM RAFIQI
2018A7PS0208P - ANANYA VARMA
2018A7PS0168P - RISHIRAJ ACCHARYA
2017B1A70902P - ANUSHKA DAYAL
 */

#include "readStream.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define keywordLength 12

char* keywords[] = {"program", "declare", "list", "of", "variables", "integer", "boolean", "real", "array", "jagged", "size", "values"};

TokenList* populate_token_list(char* keywords[])
{
    TokenList *t = (TokenList*) malloc(sizeof(TokenList));
    t ->c = '\0';
    t ->end = 0;
    int i=0;
    for(; i < 26; i++)
        t ->arr[i] = NULL;
    for(i = 0; i < keywordLength; i++)
        add_token(t, keywords[i]);
    return t;
}

char* find_token(TokenList *tl, char *str)
{
    char ch = str[0];
    TokenList *temp_list = tl;
    int k;
    
    if(ch=='(') return "L_BRAC";
    else if (ch==')') return "R_BRAC";
    else if (ch=='[') return "LSQ_BRAC";
    else if (ch==']') return "RSQ_BRAC";
    else if (ch== '{') return "LCURL_BRAC";
    else if (ch=='}') return "RCURL_BRAC";
    else if (ch==':') return "COLON";
    else if (ch==';') return "SEMICOLON";
    else if (ch=='+') return "ADD";
    else if (ch=='-') return "SUB";
    else if (ch=='*') return "PROD";
    else if (ch=='/') return "DIVIDE";
    else if (ch== '=') return "EQUALS";
    else if (ch== '&') return "AND";
    else if (ch== '|') return "OR";
    else if (ch== '.') return "DOTS";

    if(strcmp(str, "R1") == 0)
        return "R1";
    ch = ch - '0';
    
    if(ch >= 0 && ch <= 9) return "NUM_LITERAL";
   
    int i=0;
    while(str[i] != '\0')
    {
        if((str[i] < 'a') || (str[i] > 'z') || temp_list ->arr[str[i] - 'a'] == NULL)
            return "ID";
        temp_list = temp_list ->arr[str[i] - 'a'];
        i++;
    }
    
    if(!(temp_list ->end))
        return "ID";
    char *ret = (char*) malloc(sizeof(char) * strlen(str));
    for(k = 0; str[k] != '\0'; k++)
        ret[k] = str[k] - 32;
    ret[k] = '\0';
    return ret;
}

void add_token(TokenList *tl, char *str)
{
    TokenList *temp_list = tl;
   
    int i=0;
    while(str[i] != '\0')
    {
        char ch = str[i] - 'a';
        if(!(temp_list ->arr[ch]))
        {
            TokenList *temp= (TokenList*) malloc(sizeof(TokenList));
            for(int j = 0; j < 26; j++)
                temp ->arr[j] = NULL;
            temp ->end = 0;
            temp ->c = ch;
            
            temp_list ->arr[ch] = temp;
            temp_list = temp;
        }
        else
            temp_list = temp_list ->arr[ch];
        i++;
    }
    temp_list ->end = 1;
}


tokenStream* tokeniseSourcecode(char *fname, tokenStream *s)
{

    FILE *ptr= fopen(fname,"r");
    if(ptr == NULL)
    {
        printf("Can't open file\n");
        return NULL;
    }
    int line_number=1;

    TokenList *t = populate_token_list(keywords);
    s= (tokenStream*) malloc(sizeof(tokenStream));
    tokenStream *head= s;
    head ->prev = NULL;

    while(!feof(ptr))
    {
        char data[1000];
        fgets(data, 1000, ptr);
        if(strcmp(data, "\n") == 0)
        {
            line_number++;
            continue;
        }
        int len = strlen(data);
        if((data[len - 1] == '\n') || (data[len - 1] == '\r'))
            data[len - 1] = '\0';
        
        char* lexeme = strtok(data, " ");
        while (lexeme != NULL)
        {
            while((*lexeme == '\t' || *lexeme == ' ' || *lexeme == '\r'))
                lexeme++;
            if((*lexeme != '\0') && (*lexeme != '\r'))
            {
                tokenStream *temp= (tokenStream*) malloc(sizeof(tokenStream));
                strcpy(temp->lexeme,lexeme);
                char* token;
                temp->line_no=line_number;
                token= find_token(t,lexeme);
                strcpy(temp->token,token);
                s->next=temp;
                temp ->prev = s;
                s=s->next;
            }
            lexeme = strtok(NULL, " ");
        }
        line_number++;
    }
    s->next=NULL;
    fclose(ptr);
    ptr = NULL;
    return head->next;
}
