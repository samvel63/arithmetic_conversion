#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "tree.h"
#include "transform.h"

void tree_simplify(Tree t)
{
	if (!t)
		return;
	
}

uint32_t find_variable_elem(Tree t, char c)
{
	if (!t)
		return 0;

	if (t->node.data.variable == c) {
		t->node.type = INTEGER;
		t->node.data.value_int = 1;
		return 1;
	}

	if (!find_variable_elem(t->left, c))
		return find_variable_elem(t->right, c);

	return 1;
}

uint32_t find_constant_elem(Tree t, uint32_t n)
{
	if (!t)
		return 0;

	if (t->node.data.value_int == n) {
		t->node.type = INTEGER;
		t->node.data.value_int = 1;
		return 1;
	}

	if (!find_constant_elem(t->left, n))
		return find_constant_elem(t->right, n);

	return 1;
}

void transform(Tree t1, Tree t2)
{
	if (!t1)
		return;

    if (t1->node.type == OPERATOR) {
    	transform(t1->left, t2);
    	transform(t1->right, t2);
    	return;
    } else if (t1->node.type == VARIABLE) {
    	printf("\nkek\n\n");
    	if(find_variable_elem(t2, t1->node.data.variable)) {
    		t1->node.type = INTEGER;
    		t1->node.data.value_int = 1;
    	}
    } else if (t1->node.type == INTEGER) {
    	if (find_constant_elem(t2, t1->node.data.value_int)) {
    		t1->node.type = INTEGER;
    		t1->node.data.value_int = 1;
    	}
    }

}

void find_det(Tree tree)
{
    if (!tree)
        return;
    find_det(tree->left);
    find_det(tree->right);
    if (tree->node.type == OPERATOR && tree->node.data.operator == '/') {
        transform(tree->left, tree->right);
    }
}