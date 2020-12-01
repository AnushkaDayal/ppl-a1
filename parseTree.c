/* GROUP 49
2018A7PS0151P - MUHTASHIM RAFIQI
2018A7PS0208P - ANANYA VARMA
2018A7PS0168P - RISHIRAJ ACCHARYA
2017B1A70902P - ANUSHKA DAYAL
 */


#include "parseTree.h"
#include<stdio.h>
#include<stdlib.h>

Stack* initialize_stack(TreeNode *root)
{
    Stack *s = (Stack*) malloc(sizeof(Stack));
    s ->element[0] = root;
    s ->top = 0;
    return s;
}

void push(Stack *s, TreeNode *t)
{
    s ->element[++s ->top] = t;
}

TreeNode* pop(Stack *s)
{
    int temp = s ->top--;
    return s ->element[temp];
}

TreeNode* initialize_tree()
{
    TreeNode *root = (TreeNode*) malloc(sizeof(TreeNode));
    root ->terminal = 0;
    root ->level = 0;
    root ->parent = NULL;
    root ->left_child = NULL;
    root ->right_sibling = NULL;
    root ->token = "<program>";
    root ->lexeme = NULL;
    
    return root;
}

