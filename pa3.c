#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
int main(int argc, char * * argv)
{
    /*
    if(argc != 5)
    {
        return EXIT_FAILURE;
    }
    */
    // Build the strictly binary tree
    Stack_Dyn_t Stack;
    Stack_Dyn_Init(&Stack, 1000);
    TreeNode * root = NULL;
    FILE * fptr = fopen(argv[1], "r");
    if(fptr == NULL)
    {
        return EXIT_FAILURE;
    }
    root = buildTree_node(Stack, root, fptr);
    fclose(fptr);
    root = reroot(root, true);
    FILE * outptr = fopen(argv[2], "w");
    if(outptr == NULL)
    {
        return EXIT_FAILURE;
    }
    write_tree_to_file(outptr, root);
    fclose(outptr);
    destroy_tree(root);



    // Write to a file using postorder format
    FILE * fptr2 = fopen(argv[1], "r");
    if(fptr2 == NULL)
    {
        return EXIT_FAILURE;
    }
    TreeNode * root2 = NULL;
    root2 = buildTree_node(Stack, root2, fptr2);
    fclose(fptr2);
    root2 = reroot(root2, false);
    FILE * outptr2 = fopen(argv[3], "w");
    if(outptr2 == NULL)
    {
        return EXIT_FAILURE;
    }
    write_tree_to_file(outptr2, root2);
    fclose(outptr2);
    destroy_tree(root2);

    // Procedure 3
    FILE * fptr3 = fopen(argv[1], "r");
    if(fptr3 == NULL)
    {
        return EXIT_FAILURE;
    }
    TreeNode * root3 = NULL;
    root3 = buildTree_node(Stack, root3, fptr3);
    //write_size_to_file(root3);
    fclose(fptr3);
    reroot3(root3, true, true, true, true);
    printf("\n");
    write_size_to_file(root3);
    destroy_tree(root3);
    Stack_Dyn_Deinit(&Stack);

    
    return EXIT_SUCCESS;
}