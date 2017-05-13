#ifndef _TREE_H_
#define _TREE_H_

#include <stdlib.h>

#include "lexer.h"

typedef struct _tree *Tree;
struct _tree {
	Token node;
	Tree left;
	Tree right;
};

Tree tree_create(Token *tokens, uint32_t idx_left, uint32_t idx_right);
void tree_destroy(Tree *t);
void tree_infix(Tree t);
void tree_print(Tree t, uint32_t depth);

#endif // _TREE_H_