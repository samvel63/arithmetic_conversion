#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "tree.h"
#include "transform.h"

Tree tree_cpy(Tree t) { 

    if (!t)
        return t;

   Tree temp = (Tree)malloc(sizeof(struct _tree));

    if (t->node.type == OPERATOR) {
        temp->node.type = t->node.type;
        temp->node.data.operator = t->node.data.operator;
    } else if (t->node.type == INTEGER) {
        temp->node.type = t->node.type;
        temp->node.data.value_int = t->node.data.value_int;
    } else if (t->node.type == VARIABLE) {
        temp->node.type = t->node.type;
        temp->node.data.variable = t->node.data.variable;
    }   

   temp->left = tree_cpy(t->left);   

   temp->right = tree_cpy(t->right);  

   return temp;
}
/*
void tree_cpy(Tree t1, Tree t2)
{
	if (!t2) {
		tree_destroy(&t1);
		return;
	}
	if (!t1)
	t1 = (Tree)malloc(sizeof(struct _tree));
	printf("\n!!!!\n\n");
	if (t2->node.type == OPERATOR) {
		t1->node.type = t2->node.type;
		t1->node.data.operator = t2->node.data.operator;
	} else if (t2->node.type == INTEGER) {
		t1->node.type = t2->node.type;
		t1->node.data.value_int = t2->node.data.value_int;
	} else if (t2->node.type == VARIABLE) {
		t1->node.type = t2->node.type;
		t1->node.data.variable = t2->node.data.variable;
	}
	tree_cpy(t1->left, t2->left);
	tree_cpy(t1->right, t2->right);
}
*/
void tree_simplify(Tree t)
{
	if (!t)
		return;

	tree_simplify(t->left);
	tree_simplify(t->right);

	if (t->node.type == OPERATOR && t->node.data.operator == '/') {
		if (t->right->node.type == INTEGER) {
			if (t->right->node.data.value_int == 1) {
				t->node.type = t->left->node.type;
				t->node.data.value_int = t->left->node.data.value_int;
				tree_destroy(&t->right);
				tree_destroy(&t->left);
			}
		}
	}


	if (t->node.type == OPERATOR && t->node.data.operator == '*') {
		if (t->left->node.type == INTEGER) {
			if (t->left->node.data.value_int == 1) {
				if (t->right->node.type == INTEGER) {
					t->node.type = INTEGER;
					t->node.data.value_int = t->right->node.data.value_int;
					tree_destroy(&t->right);
                    tree_destroy(&t->left);
				} else if (t->right->node.type == VARIABLE) {
					t->node.type = VARIABLE;
					t->node.data.variable = t->right->node.data.variable;
					tree_destroy(&t->right);
                    tree_destroy(&t->left);
				} else if (t->right->node.type == OPERATOR) {
                    t->node.data.operator = t->right->node.data.operator;
                    Tree tmp_1 = t->right->right;
                    Tree tmp_2 = t->right->left;
                    t->left = NULL;
                    t->left = tmp_1;
                    t->right = tmp_2;
				}
			}
		} else if (t->right->node.type == INTEGER) {
			if (t->right->node.data.value_int == 1) {
				if (t->left->node.type == INTEGER) {
					t->node.type = INTEGER;
					tree_destroy(&t->left);
                    tree_destroy(&t->right);
					t->node.data.value_int = t->left->node.data.value_int;
				} else if (t->left->node.type == VARIABLE) {
					t->node.type = VARIABLE;
					t->node.data.variable = t->left->node.data.variable;
					tree_destroy(&t->left);
                    tree_destroy(&t->right);
				} else if (t->left->node.type == OPERATOR) {
                    t->node.data.operator = t->left->node.data.operator;
                    Tree tmp_1 = t->left->right;
                    Tree tmp_2 = t->left->left;
                    t->right = NULL;
                    t->right = tmp_1;
                    t->left = tmp_2;
				}
			}

		}
	}

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