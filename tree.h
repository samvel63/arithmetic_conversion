#ifndef _TREE_H_
#define _TREE_H_

#include <stdlib.h>

#include "lexer.h"

typedef struct _tree *tree;
struct _tree {
	Token node;
	Tree left;
	Tree right;
}

#endif // _TREE_H_