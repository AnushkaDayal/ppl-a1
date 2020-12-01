/* GROUP 49
2018A7PS0151P - MUHTASHIM RAFIQI
2018A7PS0208P - ANANYA VARMA
2018A7PS0168P - RISHIRAJ ACCHARYA
2017B1A70902P - ANUSHKA DAYAL
 */

typedef struct Node
{
    char RHS[20];
    struct Node* next;
} Node;

typedef struct Grammar
{
    char LHS[20];
    Node* head;

} Grammar;

typedef struct n
{
    char c;
    struct n* arr[26];
    int val;
}non_terminal;


Grammar* populate(char* fname);
Grammar* find_non_terminal(Grammar *g, char *c);
non_terminal* initialize_nt();
void add_nt(non_terminal *nt, char* c, int n);
int val_nt(non_terminal *nt, char *c);
