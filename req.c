/* GROUP 49
2018A7PS0151P - MUHTASHIM RAFIQI
2018A7PS0208P - ANANYA VARMA
2018A7PS0168P - RISHIRAJ ACCHARYA
2017B1A70902P - ANUSHKA DAYAL
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "populate.h"
#include "readStream.h"
#include "parseTree.h"

enum avail_type{stat=0,dynamic=1,not_applicable=2};
enum arr_type{primary=0,rectangular=1,jagged=2};

char* to_string(int n);
char* concat_str(char *str1, char *str2);
char *substr(char *string, int lo, int hi);

void createParseTree(Stack *s, Grammar *g, tokenStream *ts, non_terminal *nt);
void traverseParseTree(TreeNode *root, non_terminal *nt);
void printParseTree(TreeNode *root);
void push_stack(TreeNode *tree_node, Node *h, Stack *s);
char* get_type_expression(TreeNode *tree_node, non_terminal *nt);
Grammar* select_nt(Grammar *g, char *c, tokenStream *p, non_terminal *nt);

int last_rem = 0;
tokenStream *last = NULL;
int varlistsize = 0;
int dimension_cnt = 0, dimension_jagged = 0;

typedef union element
{
  int num;
  char id[20];
}element;

typedef struct prim_id
{
  char prim_type[10];
} prim_id;

typedef struct rec_array
{
  char prim_type[10];
  int dim;
  element* ranges;
  int* key;
    
} rec_array;

typedef struct jagged_array
{	

  int error;
  char prim_type[10];
  int dim;
  int low;
  int high;
  int* ranges;
}jagged_array;

typedef union type_expr
{
  prim_id p;
  rec_array r;
  jagged_array j;
}type_expr;

typedef struct rec_type{
  struct rec_type *next;
  char name[25];
  type_expr typeexpression;
  enum arr_type arrtype;
  enum avail_type rectype;
}rec_type;


//global pointer
rec_type *main_pointer=NULL;


char* to_string(int number)
{
    char *temp = (char*) malloc (4 * sizeof(char));
    temp[3] = '\0';
    for(int i = 2; i >= 0; i--)
    {
        temp[i] = (number % 10) + '0';
        number /= 10;
    }
    while(*temp == '0')
        temp++;
    if(*temp == '\0')
        return "0";
    return temp;
}

char* concat_str(char *str1, char *str2)
{
    char* temp = (char *) malloc (sizeof(char) * 300);
    strcpy(temp, str1);
    strcat(temp, str2);
    return temp;
}

char *substr(char *string, int lo, int hi)
{
   char *p;
   if(lo >= hi)
    return "";
   int c, length = hi - lo;
   p = malloc(length+1);

   for (c = lo; c < hi; c++)
   {
      p[c - lo] = string[c];
   }

   p[length] = '\0';

   return p;
}



void traverse_arr(TreeNode* root)
{
  TreeNode* init_node= root->left_child;
      while(strcmp(init_node->token,"ID")!=0)
      init_node = init_node->right_sibling;
    
  TreeNode* par_node = init_node;
      while(strcmp(par_node->token,"LSQ_BRAC")!=0)
      par_node = par_node->right_sibling;
	
  TreeNode* var_type= par_node;
      while(strcmp(var_type->token,"<dtype>")!=0)
      var_type = var_type->right_sibling;

  char req_type[10];
      var_type= var_type->left_child;
      strcpy(req_type,var_type->lexeme);

  int dimension=1;
  int key1[100] ;
  int type = 0;
  element ranges1[100];

  for(int i=0;i<100;i++)
  	{
  		ranges1[i].num = -1;
  		key1[i] = -1;
  	}

  	int curr =0;
  	par_node = par_node->right_sibling;

  	TreeNode* left_data = par_node->left_child;

  	if(strcmp(left_data->token,"ID")==0)
  	{
  		key1[curr] = 1;
  		strcpy(ranges1[curr].id,left_data->lexeme);
  		curr+=1;
  		type =1;
  	}
  	else
  	{
  		key1[curr] = 0;
  		ranges1[curr].num = atoi(left_data->lexeme);
  		curr+=1;
  	}
  	par_node = par_node->right_sibling->right_sibling;
  	TreeNode* right_data = par_node->left_child;

  	if(strcmp(right_data->token,"ID")==0)
  	{
  		key1[curr] = 1;
  		strcpy(ranges1[curr].id,right_data->lexeme);
  		curr+=1;
  		type =1;
  	}
  	else
  	{
  		key1[curr] = 0;
  		ranges1[curr].num = atoi(right_data->lexeme);
  		curr+=1;
  	}
  	par_node = par_node->right_sibling->right_sibling;


  	if(strcmp("<rows>",par_node->token)==0)
  	{
  		while(strcmp(par_node->left_child->token,"e")!=0)
  		{

  			TreeNode* succ = par_node->left_child->right_sibling;
  			TreeNode* left_data = succ->left_child;

  			if(strcmp(left_data->token,"ID")==0)
  			{
  				key1[curr] = 1;
  				strcpy(ranges1[curr].id,left_data->lexeme);
  				curr+=1;
  				type =1;
  			}
  			else
  			{
  				key1[curr] = 0;
  				ranges1[curr].num = atoi(left_data->lexeme);
  				curr+=1;
  			}
  			succ = succ->right_sibling->right_sibling;
  			TreeNode* right_data = succ->left_child;

  			if(strcmp(right_data->token,"ID")==0)
  			{
  				key1[curr] = 1;
  				strcpy(ranges1[curr].id,right_data->lexeme);
  				type =1;
                curr++;
  			}
  			else
  			{
  				key1[curr] = 0;
  				ranges1[curr].num = atoi(right_data->lexeme);
  				curr++;
  			}
  			par_node = succ->right_sibling->right_sibling;
            dimension ++;
  		}
  	}

  rec_type* data= (rec_type* )malloc(sizeof(rec_type) );
  (data->typeexpression).r.ranges = (element*)malloc(sizeof(element)*curr);
(data->typeexpression).r.key = (int*)malloc(sizeof(int)*curr);

      strcpy(data->name,init_node->lexeme);
      data->arrtype= rectangular;

  	if(type==0)
  		data->rectype = stat;
    else
  		data->rectype = dynamic;
  	
  		(data->typeexpression).r.dim = dimension;

  	for(int i=0;i<curr;i++)
  	{

		(data->typeexpression).r.ranges[i] = ranges1[i];

  		(data->typeexpression).r.key[i] = key1[i];
  	}
  	strcpy((data->typeexpression).r.prim_type,req_type);
        data->next=NULL;

  	main_pointer->next=data;
      main_pointer=main_pointer->next;

  	init_node = init_node->right_sibling;

  	if(strcmp(init_node->token,"<v_list>")==0)
  	{
  	while(strcmp(init_node->left_child->token,"e")!=0)
  	{
  		TreeNode * successor = init_node->left_child;

       data= (rec_type* )malloc(sizeof(rec_type) );
    (data->typeexpression).r.ranges = (element*)malloc(sizeof(element)*curr);
(data->typeexpression).r.key = (int*)malloc(sizeof(int)*curr);
          strcpy(data->name,successor->lexeme);
          data->arrtype= rectangular;

      	if(!type)
      	data->rectype = stat;
      	else
      	data->rectype = dynamic;
      	
        (data->typeexpression).r.dim = dimension;
      	for(int i=0;i<curr;i++)
      	{
      		(data->typeexpression).r.ranges[i] = ranges1[i];
      		(data->typeexpression).r.key[i] = key1[i];
      	}
      	strcpy((data->typeexpression).r.prim_type,req_type);
            data->next=NULL;

      	main_pointer->next=data;
          main_pointer=main_pointer->next;

  		init_node = successor->right_sibling;
  	}
  	}

}

void traverse_jagg(TreeNode* root)
{
  TreeNode * init_node= root->left_child;
while(strcmp(init_node->token,"ID"))
init_node = init_node->right_sibling;

TreeNode * par_node = init_node;
while(strcmp(par_node->token,"LSQ_BRAC"))
par_node = par_node->right_sibling;

TreeNode* var_type= par_node;
  while(strcmp(var_type->token,"<dtype>"))
  var_type = var_type->right_sibling;

TreeNode* r_node = var_type;
  while(strcmp(r_node->token,"<row_list>"))
  r_node = r_node->right_sibling;

char req_type[10];
  var_type= var_type->left_child;
  strcpy(req_type,var_type->lexeme);

int dimension=1, lo =0,hi=0;
lo = atoi(par_node->right_sibling->left_child->lexeme);
hi = atoi(par_node->right_sibling->right_sibling->right_sibling->left_child->lexeme);

par_node = par_node->right_sibling->right_sibling->right_sibling->right_sibling->right_sibling;

while(strcmp(par_node->left_child->token,"e")!=0)
{
  TreeNode* succ = par_node->left_child;
  dimension ++;
  par_node = succ->right_sibling->right_sibling;

}

int ranges1[200];
	
    for(int i =0;i<200;)
	{
		ranges1[i] = -1;
        i++;
	}
	int curr=0;

rec_type data;
strcpy((data.typeexpression).j.prim_type,req_type);

data.arrtype=jagged;

data.rectype=not_applicable;
(data.typeexpression).j.low=lo;
(data.typeexpression).j.high=hi;
data.typeexpression.j.dim=dimension;
data.next=NULL;

while(strcmp(r_node->left_child->token,"<jag_row>")==0)
{
    
  TreeNode * succ = r_node->left_child;
  TreeNode * grand_child = r_node->left_child->left_child;
  while(strcmp(grand_child->token,"NUM_LITERAL"))
  {
    grand_child = grand_child->right_sibling;
  }

  grand_child = grand_child->right_sibling;
  int t_size=0;
  while(strcmp(grand_child->token,"NUM_LITERAL"))
  {
    grand_child = grand_child->right_sibling;

  }

  t_size = atoi(grand_child->lexeme);

  while(strcmp(grand_child->token,"<value>"))
  {
    grand_child = grand_child->right_sibling;
  }

  if(dimension==2)
  {
    int cnt1 = 0;
    ranges1[curr]=t_size;
    curr++;
    int error_line= grand_child->left_child->left_child->line_number;


    while(grand_child->left_child->right_sibling!=NULL)
    {
      TreeNode * val_ptr = grand_child->left_child;

      if(strcmp(val_ptr->left_child->token,"e")==0)
      {
		  printf("ERROR: semicolon followed by semicolon at line %d \n",error_line);
		  data.typeexpression.j.error=1;
	  }


      if(strcmp(val_ptr->left_child->token,"NUM_LITERAL")==0 && strcmp(val_ptr->left_child->right_sibling->left_child->token,"NUM_LITERAL")==0)
      {
		  printf("ERROR: Assigned Size exceeding limits at line %d 1\n",error_line );
		  data.typeexpression.j.error=1;
	  }

      cnt1++;
      grand_child= grand_child->left_child->right_sibling->right_sibling;
    }

    TreeNode * val_ptr= grand_child->left_child;
    if(strcmp(val_ptr->left_child->token,"e")==0)
    {
		printf("ERROR: semicolon followed by semicolon at line %d \n",error_line);
		data.typeexpression.j.error=1;
	}
    cnt1++;

    if(cnt1!=t_size)
    {
		printf("ERROR: Unequal Sizes at line %d\n",error_line );
		data.typeexpression.j.error=1;
	}

  }

  if(dimension==3)
  {
	int cnt1 = 0;
	ranges1[curr]=t_size;

    	curr++;
	
    int error_line= grand_child->left_child->left_child->line_number;
	TreeNode * t_next  = grand_child;


	while((grand_child->left_child->right_sibling))
    	{
		TreeNode * val_ptr = grand_child->left_child;
		if(!(strcmp(val_ptr->left_child->token,"e")))
		{
      			printf("ERROR: semicolon followed by semicolon at line %d \n",error_line);
	  		data.typeexpression.j.error=1;

      			ranges1[curr] = 0;
      			cnt1++;
                curr++;
    		}
		else
		{
			int count =0;
			while(strcmp(val_ptr->left_child->token,"e"))
			{
				val_ptr = val_ptr->left_child->right_sibling;
                count++;
			}
			ranges1[curr] = count;

			curr+=1;
			cnt1+=1;
			
		}
		t_next  = grand_child->left_child->right_sibling->right_sibling;

		grand_child= grand_child->left_child->right_sibling->right_sibling;
    	}

    if(	t_next ->left_child->right_sibling==NULL)
	{
		int count = 0;
		TreeNode * ab = t_next->left_child;
		if(strcmp(ab->left_child->token,"e")==0)
		{
			printf("There is an error at the last position");
		}
		else
		{
			while(strcmp(ab->left_child->token,"e")!=0)
			{
				count+=1;
				ab = ab->left_child->right_sibling;
			}
			ranges1[curr] = count;

			curr+=1;
		}
	}

 cnt1++;

    if(cnt1!=t_size)
    {
		printf("ERROR: Unequal Sizes at line %d\n",error_line );
		data.typeexpression.j.error=1;
	}

  }

  r_node = r_node->left_child->right_sibling;

}

data.typeexpression.j.ranges= (int*)malloc(sizeof(int)*curr);

for(int i=0;i<curr;i++)
data.typeexpression.j.ranges[i]=ranges1[i];

rec_type* fixed_ptr= (rec_type*)malloc(sizeof(rec_type));

*(fixed_ptr)=data;
strcpy(fixed_ptr->name,init_node->lexeme);
 

main_pointer->next=fixed_ptr;
  main_pointer=main_pointer->next;

  	init_node = init_node->right_sibling;
	
  if(!(strcmp(init_node->token,"<v_list>")))
  {
  while(strcmp(init_node->left_child->token,"e"))
  {
    TreeNode * successor = init_node->left_child;


    fixed_ptr= (rec_type*)malloc(sizeof(rec_type));

    *(fixed_ptr)=data;
    strcpy(fixed_ptr->name,successor->lexeme);

    main_pointer->next=fixed_ptr;
      main_pointer=main_pointer->next;

	init_node = successor->right_sibling;

  }

}

}

void traverse_prim(TreeNode* root)
{
    TreeNode* t0= root->left_child;

    while(strcmp(t0->token,"ID"))
    t0=t0->right_sibling;

    TreeNode* t1= t0;

    while(strcmp(t1->token,"<dtype>"))
    t1=t1->right_sibling;

    char req_type[10];

    t1= t1->left_child;
    strcpy(req_type,t1->lexeme);

    rec_type* data= (rec_type* )malloc(sizeof(rec_type));
    strcpy(data->name,t0->lexeme);
    data->arrtype= primary;
    data->rectype=not_applicable;
 
    strcpy((data->typeexpression).p.prim_type,req_type);
    data->next=NULL;

    main_pointer->next=data;
    main_pointer=main_pointer->next;

    t0=t0->right_sibling;
    if(!(strcmp(t0->token,"<v_list>")))
    {
       t0= t0->left_child;

      while(strcmp(t0->token,"e"))
      {
    
      rec_type* data= (rec_type* )malloc(sizeof(rec_type));
      strcpy(data->name,t0->lexeme);
      data->arrtype= primary;
      data->rectype=not_applicable;
    
      strcpy((data->typeexpression).p.prim_type,req_type);
          data->next=NULL;

      main_pointer->next=data;
      main_pointer=main_pointer->next;

        t0= t0->right_sibling->left_child;
      }
    }

}


void createrec(TreeNode* root)
{
  TreeNode *req= root->left_child;
  if(!(strcmp(req->token,"<dec_pm>")))
  {
    traverse_prim(req);
  }
  else if(!(strcmp(req->token,"<dec_array>")))
  {
    traverse_arr(req);
  }
  else if(!(strcmp(req->token,"<dec_jagged>")))
  {
    traverse_jagg(req);
  }

}


rec_type* traversedeclare(TreeNode* root,rec_type* t)
{
  rec_type* head= (rec_type*)malloc(sizeof(rec_type));

  main_pointer=head;

  TreeNode *curr= root->left_child->right_sibling->right_sibling->right_sibling->right_sibling;
  printf("%s",curr->token);

  TreeNode* temp = curr->right_sibling->right_sibling;

  while(strcmp(curr->token,"e")!=0)
  {
    createrec(curr);
    curr= curr->right_sibling->left_child;
  }

  head=head->next;
  return head;

}

rec_type* traversedeclare_noprint(TreeNode* root,rec_type* t)
{

  rec_type* head= (rec_type*)malloc(sizeof(rec_type));
  main_pointer=head;

  TreeNode *curr= root->left_child->right_sibling->right_sibling->right_sibling->right_sibling;

  TreeNode* temp = curr->right_sibling->right_sibling;

  while(strcmp(curr->token,"e")!=0)
  {
    createrec(curr);
    curr= curr->right_sibling->left_child;
  }

  head=head->next;
  return head;

}



Grammar* select_nt(Grammar *g, char *c, tokenStream *p, non_terminal *nt)
{
    Grammar *gram = find_non_terminal(g, c);
    tokenStream *ts = p;
    int i = val_nt(nt, gram ->LHS);
 
    if(i==0){
        last_rem = 0;
        last = NULL;
        if(strcmp(p ->token, "DECLARE") == 0)
            return gram;
        else
            return gram + 1;
    }
    else if(i==1){

        last_rem = 0;
        last = NULL;
        while(ts && (strcmp(ts ->token, "COLON") != 0))
            ts = ts ->next;
        ts = ts ->next;
        if(strcmp(ts ->token, "JAGGED") == 0)
            return gram + 2;
        else if(strcmp(ts ->token, "ARRAY") == 0)
            return gram + 1;
        else
            return gram;
    }

        
    else if(i==2 || i==3 || i==4){
        last_rem = 0;
        last = NULL;
        while(ts && (strcmp(ts ->token, "DECLARE") != 0))
            ts = ts ->next;
        ts = ts ->next;
        if(strcmp(ts ->token, "LIST") == 0)
            return gram + 1;
        else
            return gram;
    }
    else if(i==5 || i==12 || i==18){
        last_rem = 0;
        last = NULL;
        if(strcmp(ts ->token, "ID") == 0)
            return gram;
        else
            return gram + 1;
    }
    else if(i==6){
        last_rem = 0;
        last = NULL;
        if(ts ->token[0] == 'I')
            return gram;
        else if(ts ->token[0] == 'B')
            return gram + 1;
        else
            return gram + 2;
    }
    else if(i==7){
        last_rem = 0;
        last = NULL;
        if(strcmp(ts ->token, "ID") == 0)
            return gram + 1;
        else
            return gram;
    }
    else if(i==8){
        last_rem = 0;
        last = NULL;
        if(strcmp(ts ->token, "R1") == 0)
            return gram;
        else
            return gram + 1;
    }
    else if(i==9){
        last_rem = 0;
        last = NULL;
        if(strcmp(ts ->token, "LSQ_BRAC") == 0)
            return gram;
        else
            return gram + 1;
    }
    
    else if(i==10){
        last_rem = 0;
        last = NULL;
        while(strcmp(ts ->token, "NUM_LITERAL") == 0)
            ts = ts ->next;
        if(strcmp(ts ->token, "SEMICOLON") == 0)
            return gram;
        else
            return gram + 1;
    }
    
    else if(i==11){
        
        last_rem = 0;
        last = NULL;
        if(strcmp(ts ->token, "NUM_LITERAL") == 0)
            return gram;
        else
            return gram + 1;
    }
    
    else if(i==13){
        last_rem = 0;
        last = NULL;
        while(ts && (strcmp(ts ->token, "ID") != 0))
            ts = ts ->next;
        ts = ts ->next;

        if(strcmp(ts ->token, "LSQ_BRAC") != 0)
            return gram;
        else
            return gram + 1;
    }
    
    else if(i==14){
        if(last_rem == 0)
        {
            last_rem = 1;
            last = p;
            while(last && strcmp(last ->token, "SEMICOLON") != 0)
                last = last ->next;
        }
        while(last != p && (strcmp(last ->token, "OR") != 0))
            last = last ->prev;
        if(last == p)
        {
            last_rem = 0;
            last = NULL;
            return gram + 1;
        }
        else
        {
            last = last ->prev;
            return gram;
        }
    }
    else if(i==15){
        if(last_rem == 0)
        {
            last_rem = 1;
            last = p;
            while(last && (strcmp(last ->token, "SEMICOLON") != 0) && (strcmp(last ->token, "OR") != 0))
                last = last ->next;
        }
        while(last != p && (strcmp(last ->token, "AND") != 0))
            last = last ->prev;
        if(last == p)
        {
            last_rem = 0;
            last = NULL;
            return gram + 1;
        }
        else
        {
            last = last ->prev;
            return gram;
        }
    }
    
    else if(i==16){
        if(last_rem == 0)
        {
            last_rem = 1;
            last = p;
            while(last && (strcmp(last ->token, "SEMICOLON")) && (strcmp(last ->token, "OR") != 0) && (strcmp(last ->token, "AND")))
                last = last ->next;
        }
        while(last != p && (strcmp(last ->token, "ADD")) && (strcmp(last ->token, "SUB")))
            last = last ->prev;
        if(last == p)
        {
            last_rem = 0;
            last = NULL;
            return gram + 2;
        }
        else if(last ->token[0] == 'P')
        {
            last = last ->prev;
            return gram;
        }
        else
        {
            last = last ->prev;
            return gram + 1;
        }
    }
    
    else if(i==17){
        if(last_rem == 0)
        {
            last_rem = 1;
            last = p;
            while(last && (strcmp(last ->token, "SEMICOLON") != 0) && (strcmp(last ->token, "OR") != 0) && (strcmp(last ->token, "AND") != 0)
                  && (strcmp(last ->token, "ADD") != 0) && (strcmp(last ->token, "SUB") != 0))
                last = last ->next;
        }
        while(last != p && (strcmp(last ->token, "PROD") != 0) && (strcmp(last ->token, "DIVIDE") != 0))
            last = last ->prev;
        if(last == p)
        {
            last_rem = 0;
            last = NULL;
            return gram + 2;
        }
        else if(last ->token[0] == 'M')
        {
            last = last ->prev;
            return gram;
        }
        else
        {
            last = last ->prev;
            return gram + 1;
        }
    }
    
    else if(i==19){
        last_rem = 0;
        last = NULL;
        if(strcmp(ts ->token, "ID") == 0 || strcmp(ts ->token, "NUM_LITERAL") == 0)
            return gram;
        else
            return gram + 1;
    }
    
    
    else if(i==20){
        last_rem = 0;
        last = NULL;
        if(strcmp(ts ->token, "LSQ_BRAC") == 0)
            return gram;
        else
            return gram + 1;
    }
    
    else {
        last_rem = 0;
        last = NULL;
        return gram;
    }
}

void createParseTree(Stack *s, Grammar *g, tokenStream *ts, non_terminal *nt)
{
    Grammar *gram = NULL;
    char *c = NULL;
    TreeNode *tree_node = NULL;

    while(s ->top >= 0)
    {
        tree_node = pop(s);
        c = tree_node ->token;
        int len = strlen(c);
        while(c[len - 1] == '\0' || c[len - 1] == '\r' || c[len - 1] == '\t' || c[len - 1] == '\n')
        {
            c[len - 1] = '\0';
            len--;
        }
        
        tree_node ->token = c;

        if(tree_node ->token[0] == '<')
        {
            gram = select_nt(g, c, ts, nt);
            Node *h = gram ->head;
            push_stack(tree_node, h, s);
        }
        else if(tree_node ->token[0] == 'e')
        {
            tree_node ->terminal = 1;
            tree_node ->lexeme = "epsilon";
        }
        else
        {
            
//            if(strcmp(ts ->token, tree_node ->token) == 0)
//            {
//                printf("%s %s\n", ts ->token, tree_node ->token);
//                printf("ERROR at line: %d\n", ts ->line_no);
//                return;
//            }

            tree_node ->lexeme = ts ->lexeme;
            tree_node ->terminal = 1;
            tree_node ->line_number = ts ->line_no;
            ts = ts ->next;
            c = tree_node ->lexeme;
            len = strlen(c);
            while(c[len - 1] == '\0' || c[len - 1] == '\r' || c[len - 1] == '\t' || c[len - 1] == '\n')
            {
                c[len - 1] = '\0';
                len--;
            }
        }
    }
    printf("SUCESS: The parse tree has been created successfully! \n");
}

void push_stack(TreeNode *tree_node, Node *h, Stack *s)
{
    if(h == NULL)
        return;
    push_stack(tree_node, h ->next, s);
    TreeNode *temp = (TreeNode*) malloc (sizeof(TreeNode));
    temp ->terminal = 0;
    temp ->token = h ->RHS;
    temp ->parent = tree_node;
    temp ->left_child = NULL;
    temp ->right_sibling = NULL;
    temp ->right_sibling = tree_node ->left_child;
    temp ->level = tree_node ->level + 1;
    tree_node ->left_child = temp;
    tree_node ->type_expression = "None";
    tree_node ->lexeme = "None";
    push(s, temp);
}

char* get_type_expression(TreeNode *tree_node, non_terminal *nt)
{
    TreeNode *temp_node = NULL;
    int value = val_nt(nt, tree_node ->token);
    
    char *range_var = NULL;
    char *c = NULL;
    char *temp = NULL;
    int len;
    
    if(value==1)
    {
        temp = get_type_expression(tree_node ->left_child, nt);
        c = (char*) malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(c, temp);
        tree_node ->type_expression = c;
        return NULL;
    }
    
    else if(value==2){

        temp_node = tree_node ->left_child;
        while(temp_node && val_nt(nt, temp_node ->token) != 6)
            temp_node = temp_node ->right_sibling;
        temp = get_type_expression(temp_node, nt);
        c = (char*) malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(c, temp);
        tree_node ->type_expression = c;
        c = NULL;
        return tree_node ->type_expression;
    }
    else if(value==3){

        tree_node ->type_expression = "type = rectangular array, dimensions = ";
        temp_node = tree_node ->left_child;
        range_var = ", ranges = (";
        dimension_cnt = 0;
        while(temp_node && val_nt(nt, temp_node ->token) != 7)
            temp_node = temp_node ->right_sibling;
        dimension_cnt++;
        temp = get_type_expression(temp_node, nt);
        c = (char*) malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(c, temp);
        c[strlen(temp)] = 0;
        range_var = concat_str(range_var, c);
        range_var = concat_str(range_var, ", ");
        temp_node = temp_node ->right_sibling ->right_sibling;
        temp = get_type_expression(temp_node, nt);
        c = (char*) malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(c, temp);
        c[strlen(temp)] = 0;
        range_var = concat_str(range_var, c);
        range_var = concat_str(range_var, "), ");
        while(temp_node && val_nt(nt, temp_node ->token) != 20)
            temp_node = temp_node ->right_sibling;
        temp = get_type_expression(temp_node, nt);
        c = (char*) malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(c, temp);
        c[strlen(temp)] = 0;
        temp = NULL;
        range_var = concat_str(range_var, c);
        temp = to_string(dimension_cnt);
        tree_node ->type_expression = concat_str(tree_node ->type_expression, temp);
        while(temp_node && val_nt(nt, temp_node ->token) != 6)
            temp_node = temp_node ->right_sibling;
        temp = get_type_expression(temp_node, nt);
        c = (char*) malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(c, temp);
        c[strlen(temp)] = 0;
        range_var = concat_str(range_var, c);
        tree_node ->type_expression = concat_str(tree_node ->type_expression, range_var);
        return tree_node ->type_expression;
    }
    
    else if(value==4){

        dimension_jagged = 0;
        tree_node ->type_expression = "type = jagged array, dimensions = ";
        temp_node = tree_node ->left_child;
        range_var = ", range_R1 = (";
        while(temp_node && val_nt(nt, temp_node ->token) != 7)
            temp_node = temp_node ->right_sibling;
        temp = get_type_expression(temp_node, nt);
        range_var = concat_str(range_var, temp);
        range_var = concat_str(range_var, ", ");
        temp_node = temp_node ->right_sibling;
        while(temp_node && val_nt(nt, temp_node ->token) != 7)
            temp_node = temp_node ->right_sibling;
        temp = get_type_expression(temp_node, nt);
        range_var = concat_str(range_var, temp);
        range_var = concat_str(range_var, "), ");
        while(temp_node && val_nt(nt, temp_node ->token) != 9)
            temp_node = temp_node ->right_sibling;
        get_type_expression(temp_node, nt);
        dimension_cnt++;
        temp = to_string(dimension_cnt);
        dimension_jagged = dimension_cnt;
        dimension_cnt = 0;
        tree_node ->type_expression = concat_str(tree_node ->type_expression, temp);
        tree_node ->type_expression = concat_str(tree_node ->type_expression, range_var);
        while(temp_node && val_nt(nt, temp_node ->token) != 6)
            temp_node = temp_node ->right_sibling;
        range_var = get_type_expression(temp_node, nt);
        tree_node ->type_expression = concat_str(tree_node ->type_expression, "range_R2 = (");
        while(temp_node && val_nt(nt, temp_node ->token) != 8)
            temp_node = temp_node ->right_sibling;
        temp = get_type_expression(temp_node, nt);

        tree_node ->type_expression = concat_str(tree_node ->type_expression, temp);
        len = strlen(tree_node ->type_expression) - 1;
        while(len > 0 && tree_node ->type_expression[len] != ',')
        {
            tree_node ->type_expression[len] = '\0';
            len--;
        }
        tree_node ->type_expression[len] = '\0';
        tree_node ->type_expression = concat_str(tree_node ->type_expression, "), ");
        tree_node ->type_expression = concat_str(tree_node ->type_expression, range_var);
        return tree_node ->type_expression;
    }
    else if(value==6){

        temp_node = tree_node ->left_child;
        if(temp_node ->token[0] == 'I')
            tree_node ->type_expression = "dtype = integer";
        else if(temp_node ->token[0] == 'B')
            tree_node ->type_expression = "dtype = boolean";
        else
            tree_node ->type_expression = "dtype = real";
        return tree_node ->type_expression;
    }
    
    else if(value==7){
 
        temp_node = tree_node ->left_child;
        tree_node ->type_expression = "value = ";
        tree_node ->type_expression = concat_str(tree_node ->type_expression, temp_node ->lexeme);
        return temp_node ->lexeme;
    }
    
    else if(value==8){
        
        temp_node = tree_node ->left_child;
        if(temp_node ->token[0] == 'e')
        {
            tree_node ->type_expression = "range_R2 = ()";
            return "";
        }
        tree_node ->type_expression = "range_R2 = (";
        range_var = get_type_expression(temp_node, nt);
        range_var = concat_str(range_var, ", ");
        temp_node = temp_node ->right_sibling;
        temp = get_type_expression(temp_node, nt);
        range_var = concat_str(range_var, temp);
        tree_node ->type_expression = concat_str(tree_node ->type_expression, range_var);
        len = strlen(tree_node ->type_expression) - 1;
        while(len > 0 && tree_node ->type_expression[len] != ',')
        {
            tree_node ->type_expression[len] = '\0';
            len--;
        }
        tree_node ->type_expression[len] = '\0';
        tree_node ->type_expression = concat_str(tree_node ->type_expression, ")");
        return range_var;
    }
    
    else if(value==9){
        
        temp_node = tree_node ->left_child;
        tree_node ->type_expression = "dim = ";
        if(temp_node ->token[0] == 'e')
        {
            dimension_cnt = 0;
            tree_node ->type_expression = "dim = 0";
        }
        else
        {
            while(temp_node && val_nt(nt, temp_node ->token) != 9)
                temp_node = temp_node ->right_sibling;
            get_type_expression(temp_node, nt);
            dimension_cnt++;
            temp = to_string(dimension_cnt);
            tree_node ->type_expression = concat_str(tree_node ->type_expression, temp);
        }
        return "None";
    }
    
    else if(value==10){

        temp_node = tree_node ->left_child;
        tree_node ->type_expression = "values = ";
        get_type_expression(temp_node, nt);
        range_var = to_string(dimension_cnt);
        temp_node = temp_node ->right_sibling;
        if(temp_node)
        {
            range_var = concat_str(range_var, ", ");
            temp_node = temp_node ->right_sibling;
            temp = get_type_expression(temp_node, nt);
            range_var = concat_str(range_var, temp);
        }
        tree_node ->type_expression = concat_str(tree_node ->type_expression, range_var);
        return range_var;
    }

    else if(value==11){

        temp_node = tree_node ->left_child;
        if(temp_node ->token[0] == 'e')
        {
            dimension_cnt = 0;
            tree_node ->type_expression = "value = None";
            return "";
        }
        tree_node ->type_expression = "value = ";
        temp_node = temp_node ->right_sibling;
        get_type_expression(temp_node, nt);
        dimension_cnt++;
        temp = to_string(dimension_cnt);
        tree_node ->type_expression = concat_str(tree_node ->type_expression, temp);
        return "";
    }
    
    else if(value==20){
        temp_node = tree_node ->left_child;
        if(temp_node ->token[0] == 'e')
        {
            tree_node ->type_expression = "None";
            return "";
        }
        tree_node ->type_expression = "range = ";
        range_var = "(";
        temp_node = temp_node ->right_sibling;
        dimension_cnt++;
        temp = get_type_expression(temp_node, nt);
        c = (char*) malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(c, temp);
        c[strlen(temp)] = 0;
        range_var = concat_str(range_var, c);
        range_var = concat_str(range_var, ", ");
        temp_node = temp_node ->right_sibling ->right_sibling;
        temp = get_type_expression(temp_node, nt);
        c = (char*) malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(c, temp);
        c[strlen(temp)] = 0;
        range_var = concat_str(range_var, c);
        range_var = concat_str(range_var, "), ");
        while(temp_node && val_nt(nt, temp_node ->token) != 20)
            temp_node = temp_node ->right_sibling;
        temp = get_type_expression(temp_node, nt);
        c = (char*) malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(c, temp);
        c[strlen(temp)] = 0;
        range_var = concat_str(range_var, c);
        tree_node ->type_expression = concat_str(tree_node ->type_expression, range_var);
        int len = strlen(tree_node ->type_expression);
        for(int i = 0; i < len; i++)
        {
            if(tree_node ->type_expression[len - i] == ')')
                break;
            else
               tree_node ->type_expression[len - i] = '\0';
        }
        return range_var;
    }
    
    
    else if(value==21)
    {
        temp_node = tree_node ->left_child;
        while(temp_node && temp_node ->token[0] != 'S')
            temp_node = temp_node ->right_sibling;
        temp_node = temp_node ->right_sibling;
        tree_node ->type_expression = "range_R2 = ";
        range_var = temp_node ->lexeme;
        if(dimension_jagged == 3)
        {
            range_var = concat_str(range_var, " [ ");
            while(temp_node && val_nt(nt, temp_node ->token) != 10)
                temp_node = temp_node ->right_sibling;
            temp = get_type_expression(temp_node, nt);
            range_var = concat_str(range_var, temp);
            range_var = concat_str(range_var, " ]");
        }
        tree_node ->type_expression = concat_str(tree_node ->type_expression, range_var);
        return range_var;
    }
    else{
        tree_node ->type_expression = "None";
        return "None";
    }
}

void traverseParseTree(TreeNode *root, non_terminal *nt)
{
    if(root == NULL)
        return;
    int value = val_nt(nt, root ->token);

    if(value == 1)
    {
        get_type_expression(root, nt);
        traverseParseTree(root ->right_sibling, nt);
    }
    else
    {
        get_type_expression(root, nt);
        traverseParseTree(root ->left_child, nt);
        traverseParseTree(root ->right_sibling, nt);
    }
}


void printParseTree(TreeNode *root)
{
    if(root == NULL)
        return;
    printf("\n");
    if(root ->terminal)
    {
        printf("%-15s %-15s %-10s %-10s %-10d %-50s %-50d\n", root ->token, "Terminal", "None", root ->lexeme, root ->line_number, "None", root ->level);
    }
    else
    {
        char *gram_rule = root ->token;
        gram_rule = concat_str(gram_rule, " ->");
        TreeNode *temp = root ->left_child;
        while(temp)
        {
            gram_rule = concat_str(gram_rule, " ");
            gram_rule = concat_str(gram_rule, temp ->token);
            temp = temp ->right_sibling;
        }
        int len1 = 0;
        int len2 = 0;
        int LEN1 = strlen(root ->type_expression);
        int LEN2 = strlen(gram_rule);
        
        char *sub1 = NULL;
        char *sub2 = NULL;
        
        if(LEN1 - len1 < 30)
            sub1 = substr(root ->type_expression, len1, LEN1);
        else
            sub1 = substr(root ->type_expression, len1, len1 + 30);
        if(LEN2 - len2 < 30)
            sub2 = substr(gram_rule, len2, LEN2);
        else
            sub2 = substr(gram_rule, len2, len2 + 30);

        len1 += 30;
        len2 += 30;
        printf("%-15s %-15s %-10s %-10s %-10s %-50s %-50d", root ->token, "Non-Terminal", sub1, "None", "None", sub2, root ->level);
        while(len1 < LEN1 || len2 < LEN2)
        {
            if(LEN1 - len1 < 30)
                sub1 = substr(root ->type_expression, len1, LEN1);
            else
                sub1 = substr(root ->type_expression, len1, len1 + 30);
            if(LEN2 - len2 < 30)
                sub2 = substr(gram_rule, len2, LEN2);
            else
                sub2 = substr(gram_rule, len2, len2 + 30);
            
            printf("%-30s\r %-40s %-20s %-39s\n", "", sub1, "", sub2);
            len1 += 30;
            len2 += 30;
        }
    }
    printParseTree(root ->left_child);
    printParseTree(root ->right_sibling);
}


int main()
{
    int i=1000;
    while(i!=0)
    {
    printf("Enter option: \n");
    printf("0 - Exit\n");
    printf("1 - Create Parse Tree\n");
    printf("2 - Create Type Expression Table and Print Type Errors\n");
    printf("3 - Print Parse Tree\n");
    printf("4 - Print Type Expression Table\n");
        
    scanf("%d", &i);
        
               
    Grammar *g = NULL;
    tokenStream *ts, *t1 = NULL, *temp = NULL;
    char gram[100] = "grammar.txt";
    char c[100]="program.txt";
    g = populate(gram);
    ts = tokeniseSourcecode(c,ts);
    non_terminal *nt = initialize_nt();
    TreeNode *root = initialize_tree();
    Stack *s = initialize_stack(root);
        
        if (i==1){
            createParseTree(s, g, ts, nt);
        }
        
        else if(i==2){
            createParseTree(s, g, ts, nt);
            traverseParseTree(root, nt);
            rec_type* t;
            t=traversedeclare(root, t);
        }
    
        else if(i==3){
            createParseTree(s, g, ts, nt);
            traverseParseTree(root, nt);
            printParseTree(root);
        }
    
        else if(i==4){
            
    createParseTree(s, g, ts, nt);
    traverseParseTree(root, nt);
    rec_type* t;
    t= traversedeclare_noprint(root, t);
            
    while(t!=NULL)
    {
      printf("%s %d %d ",t->name,t->arrtype,t->rectype );
      if(t->arrtype==0)
      {
          printf("%s \n\n",(t->typeexpression).p.prim_type);
      }
      else if(t->arrtype==1)
      {
        printf("%s %d ",(t->typeexpression).r.prim_type,(t->typeexpression).r.dim);
        for(int i=0;i<(t->typeexpression).r.dim * 2;i++)
        {
          if((t->typeexpression).r.key[i] ==0)
          {
            printf("%d ",(t->typeexpression).r.ranges[i].num);
          }
          else
            printf("%s ",(t->typeexpression).r.ranges[i].id);
        }
        printf("\n\n");

      }
      else if(t->arrtype==2)
      {
        printf("%s %d %d %d\n",(t->typeexpression).j.prim_type,(t->typeexpression).j.dim,(t->typeexpression).j.low,(t->typeexpression).j.high);

    if((t->typeexpression).j.dim ==2)
    {
        for(int i=0;i<(t->typeexpression).j.high-(t->typeexpression).j.low+1 ;i++ )
        {
              printf("%d ",(t->typeexpression).j.ranges[i]);
        }

    }
        
    printf(" \n\n" );
	

    }

      t=t->next;

    }
    }
        
    else if (i!=0){
        printf("Invalid Input!\n");
    }
        
    else if (i==0){
        printf("Exit Successful! \n");
    }
        
    }
    return 0;
}



