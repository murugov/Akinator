#include "akinator.h"
#include "SizeFile.h"
#include "LineCounter.h"

// ("Is it from Star Wars?" ("Is it a character?" ("Is it from the Light Side?" ("Is it a Jedi Master?" ("Luke Skywalker" nil nil) ("Is it a droid?" ("R2-D2" nil nil) ("Princess Leia" nil nil))) ("Is it from the Dark Side?" ("Darth Vader" nil nil) ("Is it the Emperor?" ("Palpatine" nil nil) ("Kylo Ren" nil nil)))) ("Is it a spaceship?" ("Is it from the Rebellion?" ("Millennium Falcon" nil nil) ("X-wing" nil nil)) ("Is it from the Empire?" ("Star Destroyer" nil nil) ("TIE Fighter" nil nil)))) ("Is it a planet?" ("Is it a desert planet?" ("Tatooine" nil nil) ("Jakku" nil nil)) ("Is it a gas giant?" ("Bespin" nil nil) ("Is it an ice planet?" ("Hoth" nil nil) ("Is it a forest moon?" ("Endor" nil nil) ("Coruscant" nil nil))))))

AkinErr_t DataReader(const char *src, tree_t *tree)
{
    FILE *data = fopen(src, "r");
    if (IS_BAD_PTR(data)) return AKIN_ERROR;

    ssize_t file_size = SizeFile(data);
    if (file_size < 0) return AKIN_ERROR;

    char* buffer = (char*)calloc((size_t)file_size + 1, sizeof(char));
    if (IS_BAD_PTR(buffer)) return AKIN_ERROR;

    size_t len_buffer = fread(buffer, sizeof(char), (size_t)file_size, data);
    buffer[len_buffer] = '\0';

    fclose(data);

    char* next_pos = buffer;
    tree->root = NodeReader(buffer, &next_pos, NULL);
    ON_DEBUG( if (IS_BAD_PTR(tree->root)) return AKIN_ERROR;)

    return AKIN_SUCCESS;
}

#define SKIP_SPACES(ptr) while (isspace((int)*ptr)) ptr++

node_t* NodeReader(char* cur_pos, char** next_pos, node_t *parent)
{
    if (cur_pos == NULL) return NULL;
    
    SKIP_SPACES(cur_pos);

    if (*cur_pos == '(')
    {
        cur_pos++;
        SKIP_SPACES(cur_pos);

        int len_name = NameNodeReader(cur_pos);
        cur_pos++;
        *(cur_pos + len_name) = '\0';

        node_t *new_node = NewNode(cur_pos);
        if (IS_BAD_PTR(new_node)) return NULL;

        cur_pos += len_name + 1;
        SKIP_SPACES(cur_pos);

        new_node->parent = parent;
        
        new_node->left = NodeReader(cur_pos, &cur_pos, new_node);
        SKIP_SPACES(cur_pos);
        
        new_node->right = NodeReader(cur_pos, &cur_pos, new_node);
        SKIP_SPACES(cur_pos);
        
        if (*cur_pos == ')') cur_pos++;
        if (next_pos) *next_pos = cur_pos;

        return new_node;
    }
    else if (strncmp(cur_pos, "nil", 3) == 0)
    {
        cur_pos += 3;
        if (next_pos) *next_pos = cur_pos;
        return NULL;
    }
    
    if (next_pos) *next_pos = cur_pos;
    return NULL;
}

#undef SKIP_SPACES


int NameNodeReader(char* cur_pos)
{
    ON_DEBUG( if (cur_pos == NULL) return 0; )

    int len_name = 0;
    sscanf(cur_pos, "\"%*[^\"]\"%n", &len_name);

    return len_name - 2;
}


AkinErr_t DataUpdate(const char *src, tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(src) || IS_BAD_PTR(tree)) return AKIN_ERROR; )

    FILE *data = fopen(src, "w");
    if (IS_BAD_PTR(data)) return AKIN_ERROR;

    PreOrder(data, tree->root);

    fclose(data);
    return AKIN_SUCCESS;
}


AkinErr_t PreOrder(FILE *data, const node_t *node)
{
    ON_DEBUG( if (IS_BAD_PTR(data) || IS_BAD_PTR(node)) return AKIN_ERROR; )

    fprintf(data, "( \"%s\" ", node->item);
    
    if (!IS_BAD_PTR(node->left))  { if (PreOrder(data, node->left))  return AKIN_ERROR; }
    else fprintf(data, "nil ");
    if (!IS_BAD_PTR(node->right)) { if (PreOrder(data, node->right)) return AKIN_ERROR; }
    else fprintf(data, "nil ");

    fprintf(data, ") ");

    return AKIN_SUCCESS;
}