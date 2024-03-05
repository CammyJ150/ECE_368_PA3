#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"

/*
Need to build a tree where string values of H and V
are non leaf nodes and are roots of subtrees.  The values
that follow those string values are the values of that
subtree
*/
TreeNode * create_tree_node(int non_l, int node_v, int h, int w, TreeNode * r, TreeNode * l)
{
  TreeNode* new_node = malloc(sizeof(TreeNode));
  new_node->non_leaf_node = non_l;
  new_node->node_val = node_v;
  new_node->width = w;
  new_node->height = h;
  new_node->right = r;
  new_node->left = l;
  new_node->new_height = 0;
  new_node->new_width = 0;
  return new_node;
}
TreeNode * buildTree_node(Stack_Dyn_t Stack, TreeNode * tn, FILE * fptr)
{
    int node_val;
    int width;
    int height;
    int my_char;
    while((my_char = fgetc(fptr)) != EOF)
    {
        fgetc(fptr); // Want to ignore the newline character
        //Post order go through tree first then do something
        if(my_char == 'H' || my_char == 'V') // Check for non leaf node
        {
            TreeNode * right = Stack_Dyn_Pop(&Stack);
            TreeNode * left = Stack_Dyn_Pop(&Stack);
            Stack_Dyn_Push(&Stack, create_tree_node(my_char, 0, 0, 0, right, left));
        }
        else
        {
            fseek(fptr, -2, SEEK_CUR); // Go back to where line started
            fscanf(fptr, "%d(%d, %d)", &node_val, &width, &height);
            Stack_Dyn_Push(&Stack, create_tree_node(NULL, node_val, height, width, NULL, NULL));
            fgetc(fptr); // Want to ignore the newline character
        }
    }

    return Stack_Dyn_Pop(&Stack);
}
/*
For deliverables 1 and 2 want to find the first leaf node we encounter.  We do
this by going left right then right left through the tree.  It is opposite for the second deliverable*/
TreeNode * reroot(TreeNode * root, bool left_right)
{
    TreeNode * new_root = root;
    while(root->right->non_leaf_node != NULL && root->left->non_leaf_node != NULL)
    {
        if(left_right == true)
        {
            new_root = root->left;
            TreeNode * new_root_left = new_root->left;
            root->left = new_root_left;
            new_root->left = root;
            left_right = false;
        }
        else if(left_right == false)
        {
            new_root = root->right;
            TreeNode * new_root_right = new_root->right;
            root->right = new_root_right;
            new_root->right = root;
            left_right = true;
        }

        root = new_root;

    }
    return new_root;
}
void write_tree_to_file(FILE * outptr, TreeNode * tn)
{
    if(tn == NULL)
    {
        return;
    }
    if(tn->non_leaf_node == 'H' || tn->non_leaf_node == 'V')
    {
      fprintf(outptr, "%c\n", tn->non_leaf_node);
    }
    else
    {
      fprintf(outptr, "%d(%d,%d)\n", tn->node_val, tn->width, tn->height);
    }
    write_tree_to_file(outptr, tn -> left);
    write_tree_to_file(outptr, tn -> right);
}
TreeNode * reroot3(TreeNode * root, bool left_right, bool right_left, bool left_left, bool right_right)
{
    TreeNode * new_root = root;
    if((left_right == true) && (new_root->left->non_leaf_node != NULL))
    {
        //For left_right the only reroots you can do is RL and RR
        new_root = reroot_node(new_root, true, false, false, false); // LR
        calc_size_node(new_root->left);
        calc_size_node(new_root);
        new_root->right->new_height = new_root->height;
        new_root->right->new_width = new_root->width;
        new_root = reroot3(new_root, false, true, false, false); // RL
        new_root = reroot3(new_root, false, false, false, true); // RR 
        new_root = reroot_node(new_root, true, false, false, false); // UNDO LR
        calc_size_node(new_root->left);
        calc_size_node(new_root);
        
    }
    if((right_left == true) && (new_root->right->non_leaf_node != NULL))
    {
        //For right_left the only reroots you can do is LL and LR
        new_root = reroot_node(new_root, false, true, false, false);
        calc_size_node(new_root->right);
        calc_size_node(new_root);
        new_root->left->new_height = new_root->height;
        new_root->left->new_width = new_root->width;
        new_root = reroot3(new_root, false, false, true, false); // LL
        new_root = reroot3(new_root, true, false, false, false); // LR
        new_root = reroot_node(new_root, false, true, false, false); // UNDO RL 
        calc_size_node(new_root->right);
        calc_size_node(new_root);

    }
    if((left_left == true) && (new_root->left->non_leaf_node != NULL))
    {
        //For left_left the only reroots you can do is LL and LR
        new_root = reroot_node(new_root, false, false, true, false);
        calc_size_node(new_root->right);
        calc_size_node(new_root);
        new_root->left->new_height = new_root->height;
        new_root->left->new_width = new_root->width;
        new_root = reroot3(new_root, false, false, true, false); // LL
        new_root = reroot3(new_root, true, false, false, false); // LR
        new_root = reroot_node(new_root, false, false, false, true); // UNDO LL by doing RR
        calc_size_node(new_root->left);
        calc_size_node(new_root);
    }
    if((right_right == true) && (new_root->right->non_leaf_node != NULL))
    {
        //For right_right the only reroots you can do is RL and RR
        new_root = reroot_node(new_root, false, false, false, true);
        calc_size_node(new_root->left);
        calc_size_node(new_root);
        new_root->right->new_height = new_root->height;
        new_root->right->new_width = new_root->width;
        new_root = reroot3(new_root, false, true, false, false); // RL
        new_root = reroot3(new_root, false, false, false, true); // RR
        new_root = reroot_node(new_root, false, false, true, false); //UNDO RR by doing LL
        calc_size_node(new_root->right);
        calc_size_node(new_root);
    }
    return new_root;
    
}
TreeNode * reroot_node(TreeNode * root, bool left_right, bool right_left, bool left_left, bool right_right)
{
    TreeNode * new_root = root;
    if(left_right == true)
    {
        new_root = root->left;
        root->left = new_root->left;
        new_root->left = root;
    }
    else if(right_left == true)
    {
        new_root = root->right;
        root->right = new_root->right;
        new_root->right = root;
    }
    else if(left_left == true)
    {
        new_root = root->left;
        root->left = new_root->right;
        new_root->right = root;

    }
    else if(right_right == true)
    {
        new_root = root->right;
        root->right = new_root->left;
        new_root->left = root;
    }
    return new_root;
}
/*
Need to determine the size of the box.  If the non_leaf_node is
V then the x sizes need to be added together and the y sizes need
to be compared to each other to see which one is largest.  If it is
H then the x sizes need to be compared to gether to see which one is the largest
and the y sizes are added together.*/
void calc_size_node(TreeNode * tn)
{
    if(tn->non_leaf_node == 'H')
  {
    tn->height = tn->left->height + tn->right->height;
    if(tn->right->width >= tn->left->width)
    {
        tn->width = tn->right->width;
    }
    else
    {
        tn->width = tn->left->width;
    }
    
  }
  else if(tn->non_leaf_node == 'V')
  {
    tn->width = tn->left->width + tn->right->width;
    if(tn->right->height >= tn->left->height)
    {
        tn->height = tn->right->height;
    }
    else
    {
        tn->height = tn->left->height;
    }
  }
}
void calc_size_orig(TreeNode * tn)
{
  if(tn == NULL)
  {
    return;
  }
  calc_size_orig(tn->left);
  calc_size_orig(tn->right);
  if(tn->non_leaf_node == 'H')
  {
    tn->height = tn->left->height + tn->right->height;
    if(tn->left->width > tn->right->width)
    {
      tn->width = tn->left->width;
    }
    else
    {
      tn->width = tn->right->width;
    }
  }
  else if(tn->non_leaf_node == 'V')
  {
    tn->width = tn->left->width + tn->right->width;
    if(tn->left->height > tn->right->height)
    {
      tn->height = tn->left->height;
    }
    else
    {
      tn->height = tn->right->height;
    }
  }
}
void write_size_to_file(FILE * outptr, TreeNode * tn)
{
    if(tn == NULL)
    {
        return;
    }
    if(tn->non_leaf_node == 'H' || tn->non_leaf_node == 'V')
    {
      if(tn->new_height == 0 && tn->new_width == 0)
      {
        fprintf(outptr, "%c\n", tn->non_leaf_node);
      }
      else
      {
        fprintf(outptr, "%c(%d,%d)\n", tn->non_leaf_node, tn->new_width, tn->new_height);
      }
    }
    else
    {
      if(tn->new_height == 0 && tn->new_width)
      {
        fprintf(outptr, "%d\n", tn->node_val);
      }
      else
      {
        fprintf(outptr, "%d(%d,%d)\n", tn->node_val, tn->new_width, tn->new_height);
      }
    }
    write_size_to_file(outptr, tn -> left);
    write_size_to_file(outptr, tn -> right);
}
void Stack_Dyn_Deinit(Stack_Dyn_t * S_Ptr)
{ /* deallocate memory of stack */
    free(S_Ptr->tn);
    S_Ptr->tn = NULL;
    S_Ptr->Size = 0;
    S_Ptr->Minimum_Size = 0;
    S_Ptr->Top = -1;
} 

int Stack_Dyn_Init (Stack_Dyn_t * S_Ptr, int Initial_Size)
{
    /* initialize the stack of type El_t */
    if (Initial_Size <= 0)
    {
        Initial_Size = STACK_INITIAL_SIZE;
    }
    S_Ptr->Top = -1;
    S_Ptr->Size = Initial_Size;
    S_Ptr->Minimum_Size = Initial_Size;
    S_Ptr->tn = (TreeNode **)malloc(sizeof(TreeNode *) * Initial_Size);
    if (S_Ptr->tn == NULL)
    {
        S_Ptr->Size = 0;
        return false; // out of memory!
    }
    return true;
} 

Stack_Dyn_t *Stack_Dyn_New (int Initial_Size)
{ /* initialize the stack of type El_t */
    Stack_Dyn_t *Result_Ptr = NULL;
    Result_Ptr = (Stack_Dyn_t *) malloc(sizeof(Stack_Dyn_t));
    if (Result_Ptr == NULL)
    { 
        return NULL; // out of memory!
    }
    if (!Stack_Dyn_Init(Result_Ptr, Initial_Size))
    { 
        free (Result_Ptr);
        return NULL;
    }
    return Result_Ptr;
} 
void Stack_Dyn_Delete(Stack_Dyn_t *S_Ptr)
{ /* delete stack and the struct from memory */
    Stack_Dyn_Deinit(S_Ptr);
    free(S_Ptr);
}
int Stack_Dyn_Empty(Stack_Dyn_t *S_Ptr)
{ /* is stack empty?? (TRUE/FALSE) */
    return (S_Ptr->Top < 0);
} 


TreeNode * Stack_Dyn_Pop(Stack_Dyn_t *S_Ptr)
{ /* pull top item off stack (adjust stack) */
    TreeNode * Result;
    int New_Size = 0;
    TreeNode ** New_Items = NULL;
    Result = S_Ptr->tn[S_Ptr->Top];
    S_Ptr->Top--;
    if ( (S_Ptr->Size == S_Ptr->Minimum_Size) || ((int)((S_Ptr->Top + 1) * STACK_GROW_FACTOR * STACK_GROW_FACTOR) + 1) >= S_Ptr->Size)
    { 
        return (Result); // no stack adjustment
    }
    New_Size = (int)((S_Ptr->Top + 1) * STACK_GROW_FACTOR) + 1;
    if (New_Size <= S_Ptr->Top + 1)
    { 
        return (Result); // no stack adjustment
    }
    if (New_Size < S_Ptr->Minimum_Size)
    { 
        New_Size = S_Ptr->Minimum_Size;
    }
    New_Items = (TreeNode **)realloc(S_Ptr->tn, sizeof(TreeNode *) * New_Size);
    if (New_Items == NULL)
    { 
        return Result; // problem with memory
    }
    S_Ptr->Size = New_Size;
    S_Ptr->tn = New_Items;
    return Result; // stack adjusted correctly
}
int Stack_Dyn_Push (Stack_Dyn_t *S_Ptr, TreeNode * Item)
{ /* place Item on top of stack (adjust stack) */
    int New_Size = 0;
    TreeNode ** New_Items = NULL;
    if (S_Ptr->Top >= S_Ptr->Size - 1)
    { 
        New_Size = (int)(STACK_GROW_FACTOR * S_Ptr->Size)+ 1;
        if (New_Size <= S_Ptr->Size)
        {
            return false; // overflow
        }
        New_Items = (TreeNode **)realloc(S_Ptr->tn, sizeof(TreeNode *) * New_Size);
        if (New_Items == NULL)
        {
            return true; // out of memory!
        }
        S_Ptr->Size = New_Size;
        S_Ptr->tn = New_Items;
    }
    S_Ptr->tn[++S_Ptr->Top] = Item;
    return (true);
}
void destroy_tree(TreeNode * tn)
{
  if(tn == NULL)
  {
    return;
  }
  destroy_tree(tn->right);
  destroy_tree(tn->left);
  free(tn);
}




