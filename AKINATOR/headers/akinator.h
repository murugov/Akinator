#ifndef AKINATOR_H
#define AKINATOR_H

#include "tree.h"
#include "colors.h"
#include "sfml_func.h"


enum AkinErr_t
{
    AKIN_SUCCESS = 0,
    AKIN_ERROR   = 1
};


AkinErr_t DataReader(const char *src, tree_t *tree);
AkinErr_t DataUpdate(const char *src, tree_t *tree);
AkinErr_t PreOrder(FILE *data, const node_t *node);
node_t* NodeReader(char* cur_pos, char** next_pos, node_t *parent);
int NameNodeReader(char* cur_pos);

AkinErr_t AkinCtor(tree_t **tree);
AkinErr_t AkinDtor(tree_t *tree);

node_t* NodeFind(const char* name, node_t *current);
AkinErr_t GetNodeDef(tree_t *tree);
AkinErr_t NodesCompare(tree_t *tree);

AkinErr_t Akinator(tree_t *tree);

AkinErr_t GenHTML();
AkinErr_t GenGraphs(tree_t *tree, const char *func);
AkinErr_t GenDot(FILE *src, tree_t *tree, const char *func);


#endif