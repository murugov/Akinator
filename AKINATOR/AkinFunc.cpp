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


AkinErr_t Akinator(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) {LOG(ERROR, "AKIN_BAD_TREE_PTR"); return AKIN_ERROR; } )
    
    if (tree->root == NULL) {
        printf(ANSI_COLOR_MAGENTA "Akinator went on vacation!\n" ANSI_COLOR_RESET);
        return AKIN_ERROR;
    }

    Speak("\aHello, human! My name is Stars Akinator.", ANSI_COLOR_MAGENTA);
    Speak("\aI can:\n"
          "1) guess the element you've guessed\n"
          "2) give a description of the element\n"
          "3) compare two elements", ANSI_COLOR_YELLOW);
    Speak("\aSelect one of the operating modes:", ANSI_COLOR_MAGENTA);
    
    int mode = 0;
    scanf("%d", &mode);
    while (mode != FIND_MODE && mode != DEFINE_MODE && mode != CMP_MODE)
    {
        Speak("\aInvalid input!", ANSI_COLOR_RED);
        scanf("%d", &mode);
        while (getchar() != '\n');
    }
    while (getchar() != '\n');

    SelectMode(mode, tree);

    return AKIN_SUCCESS;
}


AkinErr_t SelectMode(int mode, tree_t *tree)
{
    ON_DEBUG(if (IS_BAD_PTR(tree)) return AKIN_ERROR;)

    switch (mode)
    {
    case FIND_MODE:
        FindMode(tree);
        break;
    case DEFINE_MODE:
        DefineMode(tree);
        break;
    case CMP_MODE:
        CmpMode(tree);
        break;
    default:
        break;
    }

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

    *len = getline(answer, capacity, stdin);
    if (*len == -1) { perror("Error reading user answer:"); return AKIN_ERROR; }
    (*answer)[*len - 1] = '\0';
    
    for (ssize_t i = 0; i < *len - 1; ++i)
        (*answer)[i] = (char)tolower((*answer)[i]);

    return AKIN_SUCCESS;
}


AkinErr_t YesOrNo(char** answer, ssize_t *len, size_t *capacity)
{
    ON_DEBUG( if (IS_BAD_PTR(answer) || IS_BAD_PTR(len) || IS_BAD_PTR(capacity)) return AKIN_ERROR; )
    
    printf(ANSI_COLOR_BLUE "Enter \"yes\" or \"no\"\n" ANSI_COLOR_RESET);
    EnterAnswer(answer, len, capacity);
    
    while (strncmp(*answer, "yes", 3) != 0 && strncmp(*answer, "no", 2) != 0)
    {
        printf(ANSI_COLOR_RED "Invalid input\n" ANSI_COLOR_RESET);
        EnterAnswer(answer, len, capacity);
    }

    return AKIN_SUCCESS;
}


AkinErr_t FindMode(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return AKIN_ERROR; )
    
    node_t *current = tree->root;
    char *answer = NULL;
    size_t capacity = 0;
    ssize_t len = 0;
    
    while (current->left != NULL || current != NULL)
    {
        printf("Is it %s?\n", current->item);

        YesOrNo(&answer, &len, &capacity);

        if (strncmp(answer, "yes", 3) == 0)
        {
            if (current->left == NULL) break;
            current = current->left;
        }
        else
        {
            if (current->right == NULL) break;
            current = current->right;
        }
    }

    if (strncmp(answer, "yes", 3) == 0)
    {
        printf(ANSI_COLOR_GREEN "I knew it! I'm so smart!\n" ANSI_COLOR_RESET);
    }
    else
    {
        printf("What did you guess?\n");
        EnterAnswer(&answer, &len, &capacity);
        
        node_t *new_node_for_ans = NewNode(strdup(answer));
        
        printf("What is the difference between %s and %s?\n", answer, current->item);
        EnterAnswer(&answer, &len, &capacity);

        node_t *new_node_for_qst = NewNode(strdup(answer));
        node_t *old_node = current;
        
        new_node_for_qst->left = new_node_for_ans;
        new_node_for_qst->right = old_node;
        new_node_for_ans->parent = new_node_for_qst;
        new_node_for_qst->parent = old_node->parent;
        
        if (old_node->parent)
        {
            if (old_node->parent->left == old_node)
                old_node->parent->left = new_node_for_qst;
            else
                old_node->parent->right = new_node_for_qst;
        }
        else
            tree->root = new_node_for_qst;
        
        printf(ANSI_COLOR_GREEN "Successfully added new knowledge!\n" ANSI_COLOR_RESET);        
    }

    free(answer);
    return AKIN_SUCCESS;
}


AkinErr_t PathFinder(const char* name, tree_t *tree, stk_t<node_t*> *path)
{
    ON_DEBUG( if (IS_BAD_PTR(name) || IS_BAD_PTR(tree) || IS_BAD_PTR(path)) return AKIN_ERROR; )

    node_t *found = NodeFind(name, tree->root);
    if (found == NULL) return AKIN_ERROR;


    node_t *parent = found->parent;
    node_t *child  = found;

    StackPush(path, child);
    while (parent != NULL)
    {
        StackPush(path, parent);
        parent = parent->parent;
        child  = child->parent;
    }
    
    return AKIN_SUCCESS;
}


AkinErr_t DefineMode(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return AKIN_ERROR; )
    
    Speak("\vPlease enter the name of the hidden item:", ANSI_COLOR_GREEN);
    char *name = NULL;
    size_t  capacity = 0;
    ssize_t len      = 0;

    if (EnterAnswer(&name, &len, &capacity) == AKIN_ERROR) return AKIN_ERROR;

    node_t *found = NodeFind(name, tree->root);
    if (found == NULL)
    {
        printf(ANSI_COLOR_RED "The node named \"%s\" is missing!\n" ANSI_COLOR_RESET, name);
        free(name);
        return AKIN_ERROR;
    }

    node_t *parent = found->parent;
    node_t *child  = found;
    while (parent != NULL)
    {
        printf(ANSI_COLOR_YELLOW "-%s is%s %s\n" ANSI_COLOR_RESET, found->item, (child == parent->right) ? " not" : "", parent->item);
        parent = parent->parent;
        child  = child->parent;
    }

    free(name);
    return AKIN_SUCCESS;
}


AkinErr_t CmpMode(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return AKIN_ERROR; )

    char* name_1 = NULL;
    char* name_2 = NULL;

    size_t  capacity = 0;
    ssize_t len      = 0;

    printf("Enter the name of the first element:\n");
    if (EnterAnswer(&name_1, &len, &capacity)) return AKIN_ERROR;
    printf("Enter the name of the second element:\n");
    if (EnterAnswer(&name_2, &len, &capacity)) return AKIN_ERROR;

    stk_t<node_t*> path_1 = {};
    STACK_CTOR(&path_1, tree->size / 2 + 1);

    PathFinder(name_1, tree, &path_1);

    stk_t<node_t*> path_2 = {};
    STACK_CTOR(&path_2, tree->size / 2 + 1);

    PathFinder(name_2, tree, &path_2);

    node_t *current_1 = NULL;
    node_t *current_2 = NULL;

    StackPop(&path_1, &current_1);
    StackPop(&path_2, &current_2);

    if (path_1.size > 2 && path_2.size > 2)
    {
        while(current_1 == current_2 && path_1.size > 0 && path_2.size > 0)
        {
            printf(ANSI_COLOR_CYAN "-%s is %s, just like %s\n" ANSI_COLOR_RESET, name_1, current_1->item, name_2);
            StackPop(&path_1, &current_1);
            StackPop(&path_2, &current_2);
        }
    }
    else
    {
        StackPop(&path_1, &current_1);
        StackPop(&path_2, &current_2);
    }

    if (current_1->parent->left == current_1)
        printf(ANSI_COLOR_CYAN "-%s is %s, but %s is not\n" ANSI_COLOR_RESET, name_1, current_1->parent->item, name_2);
    else
        printf(ANSI_COLOR_CYAN "-%s is not %s, but %s is\n" ANSI_COLOR_RESET, name_1, current_1->parent->item, name_2);

    STACK_DTOR(&path_1);
    STACK_DTOR(&path_2);

    free(name_1);
    free(name_2);
    return AKIN_SUCCESS;
}