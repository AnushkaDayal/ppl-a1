/* GROUP 49
2018A7PS0151P - MUHTASHIM RAFIQI
2018A7PS0208P - ANANYA VARMA
2018A7PS0168P - RISHIRAJ ACCHARYA
2017B1A70902P - ANUSHKA DAYAL
 */


typedef struct TreeNode
{
    int terminal;
    int level;
    int line_number;
    char *token;
    char *lexeme;
    char *type_expression;
    struct TreeNode *parent;
    struct TreeNode *left_child;
    struct TreeNode *right_sibling;
    
}TreeNode;

typedef struct Stack
{
    int top;
    TreeNode* element[1000];
}Stack;

Stack* initialize_stack(TreeNode *root);
TreeNode* initialize_tree();
void push(Stack *s, TreeNode *t);
TreeNode* pop(Stack *s);

