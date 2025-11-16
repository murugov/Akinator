#include "akinator.h"


AkinErr_t AkinCtor(tree_t **tree)
{
    *tree = (tree_t*)calloc(1, sizeof(tree_t));
    if (IS_BAD_PTR(tree)) return AKIN_ERROR;

    TREE_INIT((*tree));

    arg_t root_val = TREE_POISON;
    (*tree)->root = NewNode(root_val);

    return (IS_BAD_PTR((*tree)->root)) ? AKIN_ERROR: AKIN_SUCCESS;
}


AkinErr_t AkinDtor(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) {LOG(ERROR, "AKIN_BAD_TREE_PTR"); return AKIN_ERROR; } )

    FreeNodes(tree->root);
    return AKIN_SUCCESS;
}


node_t* NodeFind(const char* name, node_t *current)
{
    ON_DEBUG( if (IS_BAD_PTR(name) || IS_BAD_PTR(current)) return NULL; )

    if (strcmp(name, current->item) == 0) return current;

    node_t* found = NULL;

    if (current->left != NULL)
    {
        found = NodeFind(name, current->left);
        if (found != NULL) return found;
    }

    if (current->right != NULL)
    {
        found = NodeFind(name, current->right);
        if (found != NULL) return found;
    }

    return NULL;
}


AkinErr_t EnterAnswer(char** answer, ssize_t *len, size_t *capacity)
{
    ON_DEBUG( if (IS_BAD_PTR(answer) || IS_BAD_PTR(len) || IS_BAD_PTR(capacity)) return AKIN_ERROR; )

    printf(ANSI_COLOR_BLUE "Enter \"yes\" or \"no\"\n" ANSI_COLOR_RESET);
    *len = getline(answer, capacity, stdin);
    if (*len == -1) { perror("Error reading user answer:"); return AKIN_ERROR; }
    (*answer)[*len] = '\0';
    
    for (ssize_t i = 0; i < *len - 1; ++i)
        (*answer)[i] = (char)tolower((*answer)[i]);

    return AKIN_SUCCESS;
}

AkinErr_t YesOrNo(char** answer, ssize_t *len, size_t *capacity)
{
    ON_DEBUG( if (IS_BAD_PTR(answer) || IS_BAD_PTR(len) || IS_BAD_PTR(capacity)) return AKIN_ERROR; )

    EnterAnswer(answer, len, capacity);
    
    while (strncmp(*answer, "yes", 3) != 0 && strncmp(*answer, "no", 2) != 0)
    {
        printf(ANSI_COLOR_RED "Invalid input\n" ANSI_COLOR_RESET);
        EnterAnswer(answer, len, capacity);
    }

    return AKIN_SUCCESS;
}


AkinErr_t Akinator(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) {LOG(ERROR, "AKIN_BAD_TREE_PTR"); return AKIN_ERROR; } )
    
    if (tree->root == NULL) {
        printf(ANSI_COLOR_MAGENTA "Akinator went on vacation!\n" ANSI_COLOR_RESET);
        return AKIN_ERROR;
    }

    node_t *current = tree->root;

    char *answer = NULL;
    // size_t capacity = 0;
    // ssize_t len = 0;
    
    while (current->left != NULL || current->right != NULL)
    {
        int verd = FindMode(current->item);

        if (verd == YES)
        {
            if (current->left == NULL) break;
            current = current->left;
        }
        else if (verd == NO)
        {
            if (current->right == NULL) break;
            current = current->right;            
        }
        // printf("Is it %s?\n", current->item);

        // YesOrNo(&answer, &len, &capacity);

        // if (strncmp(answer, "yes", 3) == 0)
        // {
        //     if (current->left == NULL) break;
        //     current = current->left;
        // }
        // else
        // {
        //     if (current->right == NULL) break;
        //     current = current->right;
        // }
    }

    FinalFind(current->item, tree, current);

    // printf("Is it %s?\n", current->item);
    // YesOrNo(&answer, &len, &capacity);

    // if (strncmp(answer, "yes", 3) == 0)
    //     printf(ANSI_COLOR_GREEN "I knew it! I'm so smart!\n" ANSI_COLOR_RESET);
    // else 
    // {
    //     printf("What did you guess?\n");
        
    //     char new_answer[256] = {0};
    //     scanf(" %255[^\n]", new_answer);
        
    //     node_t *new_node_for_ans = NewNode(strdup(new_answer));
        
    //     printf("What is the difference between %s and %s?\n", new_answer, current->item);
        
    //     char question[256] = {0};
    //     scanf(" %255[^\n]", question);

    //     node_t *new_node_for_qst = NewNode(strdup(question));
    //     node_t *old_node = current;
        
    //     new_node_for_qst->left = new_node_for_ans;
    //     new_node_for_qst->right = old_node;
    //     new_node_for_ans->parent = new_node_for_qst;
    //     new_node_for_qst->parent = old_node->parent;
    //     new_node_for_qst->parent->right = new_node_for_qst;
    //     old_node->parent = new_node_for_qst;
        
    //     printf(ANSI_COLOR_GREEN "Successfully added new knowledge!\n" ANSI_COLOR_RESET);
    // }

    free(answer);
    
    return AKIN_SUCCESS;
}


// AkinErr_t GetNodeDef(const char* name, tree_t *tree)
AkinErr_t GetNodeDef(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return AKIN_ERROR; )
    
    DefineMode(tree);
    
    // node_t *found = NodeFind(name, tree->root);
    // if (found == NULL)
    // {
    //     printf(ANSI_COLOR_RED "The node named \"%s\" is missing!\n" ANSI_COLOR_RESET, name);
    //     return AKIN_ERROR;
    // }

    // node_t *parent = found->parent;
    // node_t *child  = found;
    // while (parent != NULL)
    // {
    //     printf(ANSI_COLOR_YELLOW "-%s is%s %s\n" ANSI_COLOR_RESET, found->item, (child == parent->right) ? " not" : "", parent->item);
    //     parent = parent->parent;
    //     child  = child->parent;
    // }

    return AKIN_SUCCESS;
}


// AkinErr_t NodesCompare(const char* name_1, const char* name_2, tree_t *tree)
AkinErr_t NodesCompare(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return AKIN_ERROR; )

    CmpMode(tree);

    // stk_t<node_t*> path_1 = {};
    // STACK_CTOR(&path_1, tree->size / 2 + 1);

    // PathFinder(name_1, tree, &path_1);

    // stk_t<node_t*> path_2 = {};
    // STACK_CTOR(&path_2, tree->size / 2 + 1);

    // PathFinder(name_2, tree, &path_2);

    // node_t *current_1 = NULL;
    // node_t *current_2 = NULL;

    // StackPop(&path_1, &current_1);
    // StackPop(&path_2, &current_2);

    // if (path_1.size > 2 && path_2.size > 2)
    // {
    //     while(current_1 == current_2 && path_1.size > 0 && path_2.size > 0)
    //     {
    //         printf(ANSI_COLOR_CYAN "-%s is %s, just like %s\n" ANSI_COLOR_RESET, name_1, current_1->item, name_2);
    //         StackPop(&path_1, &current_1);
    //         StackPop(&path_2, &current_2);
    //     }
    // }
    // else
    // {
    //     StackPop(&path_1, &current_1);
    //     StackPop(&path_2, &current_2);
    // }

    // if (current_1->parent->left == current_1)
    //     printf(ANSI_COLOR_CYAN "-%s is %s, but %s is not\n" ANSI_COLOR_RESET, name_1, current_1->parent->item, name_2);
    // else
    //     printf(ANSI_COLOR_CYAN "-%s is not %s, but %s is\n" ANSI_COLOR_RESET, name_1, current_1->parent->item, name_2);

    // STACK_DTOR(&path_1);
    // STACK_DTOR(&path_2);

    return AKIN_SUCCESS;
}