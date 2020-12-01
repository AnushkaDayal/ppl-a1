/* GROUP 49
2018A7PS0151P - MUHTASHIM RAFIQI
2018A7PS0208P - ANANYA VARMA
2018A7PS0168P - RISHIRAJ ACCHARYA
2017B1A70902P - ANUSHKA DAYAL
 */

#include "populate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Grammar_length 49
#define Non_terminal_length 22

char* non_terminals[] = {"<dec_sml>", "<dec_sm>", "<dec_pm>", "<dec_array>", "<dec_jagged>", "<v_list>", "<dtype>",
"<range>", "<row_list>", "<bracket>", "<value>", "<val>", "<assign_sml>", "<variable>", "<or_opn>", "<and_opn>", "<add_opn>", "<mult_opn>",
"<var1>", "<rangelist>", "<rows>", "<jag_row>"};


Grammar* find_non_terminal(Grammar *g, char *c)
{
    Grammar *gram = g;
    int i=0;
    while(i<Grammar_length)
    {
        if(strcmp(gram[i].LHS, c) == 0)
            return &gram[i];
        i++;
    }
    return NULL;
}

Grammar* populate(char* fname)
{

    FILE *ptr= fopen(fname,"r");
    if(!ptr)
    {
        printf("File cannot be accessed\n");
        return NULL;
    }
    int count= 49;
    int i=0;
    
    Grammar* ans= (Grammar* ) malloc(sizeof(Grammar)*count);
    
    while(!feof(ptr))
    {
        char data[1000];
        fscanf(ptr, "%[^\n]\n", data);

        char* lexeme = strtok(data, " ");
        strcpy(ans[i].LHS,lexeme);
        ans[i].head=NULL;
        lexeme = strtok(NULL, " ");
        Node* head= (Node* )malloc(sizeof(Node));
        head->next=NULL;
        Node* s=head;

        while (lexeme != NULL)
        {
            Node* temp= (Node* ) malloc(sizeof(Node));
            strcpy(temp->RHS,lexeme);
            temp->next=NULL;
            s->next=temp;
            s=s->next;
            lexeme = strtok(NULL, " ");
        }
        
        ans[i].head= head->next;
        free(head);
        i++;
    }
    fclose(ptr);
    ptr = NULL;
    return ans;
}



non_terminal* initialize_nt()
{
    non_terminal *nt = (non_terminal*) malloc (sizeof(non_terminal));
    nt ->val = -1;
    for(int i = 0; i < 26; i++)
        nt ->arr[i] = NULL;
    for(int i = 0; i < Non_terminal_length; i++)
        add_nt(nt, non_terminals[i], i);
    return nt;
}

void add_nt(non_terminal *nt, char* c, int n)
{
    int l = strlen(c);
    non_terminal *p = nt, *temp = NULL;
    int i=0;
    while(i<l)
    {
        int ch = c[i] - 'a';
        while(i < l && (ch < 0 || ch > 25))
        {
            i++;
            ch = c[i] - 'a';
        }
        if(i >= l)
            break;
        if(p ->arr[ch] == NULL)
        {
            temp = (non_terminal*) malloc (sizeof(non_terminal));
            for(int j = 0; j < 26; j++)
                temp ->arr[j] = NULL;
            temp ->val = -1;
            temp ->c = c[i];
            p ->arr[ch] = temp;
            p = temp;
            temp = NULL;
        }
        else
            p = p ->arr[ch];
        i++;
    }
    p ->val = n;
}

int val_nt(non_terminal *nt, char *c)
{
    int l = strlen(c);
    non_terminal *p = nt, *temp = NULL;
    int i=0;
    while(i<l)
    
    {
        int ch = c[i] - 'a';
        while(i < l && (ch < 0 || ch > 25))
        {
            i++;
            ch = c[i] - 'a';
        }
        if(i >= l)
            break;
        if(p ->arr[ch] == NULL)
            return -1;
        p = p ->arr[ch];
        
        i++;
    }
    return p ->val;
}
