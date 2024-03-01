#ifndef TREE_H
#define TREE_H
#define STACK_INITIAL_SIZE 1000
#define STACK_GROW_FACTOR 1.5


typedef struct tnode
{
    char non_leaf_node;
    int node_val;
    int width;
    int height;
    struct tnode * left;
    struct tnode * right;
} TreeNode;


struct Stack_Dyn
{ 
    int Top;
    TreeNode ** tn;
    int Size;
    int Minimum_Size;
};
typedef struct Stack_Dyn Stack_Dyn_t;
// function prototypes
TreeNode * create_tree_node(int non_l, int node_v, int h, int w, TreeNode * r, TreeNode * l);
TreeNode * buildTree_node(Stack_Dyn_t stack, TreeNode * tn, FILE * fptr);
TreeNode * reroot(TreeNode * root, bool left_right);
void write_tree_to_file(FILE * outptr, TreeNode * tn);
void destroy_tree(TreeNode * tn);

int Stack_Dyn_Init(Stack_Dyn_t *S_Ptr, int Initial_Size);
void Stack_Dyn_Deinit(Stack_Dyn_t *S_Ptr);
Stack_Dyn_t *Stack_Dyn_New(int Initial_Size);
void Stack_Dyn_Delete(Stack_Dyn_t *S_Ptr);
int Stack_Dyn_Empty(Stack_Dyn_t *S_Ptr);
TreeNode * Stack_Dyn_Pop(Stack_Dyn_t *S_Ptr);
int Stack_Dyn_Push(Stack_Dyn_t *S_Ptr, TreeNode * tn);
#endif