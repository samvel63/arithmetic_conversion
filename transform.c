#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "tree.h"
#include "transform.h"

uint32_t trees_is_equal(Tree t1, Tree t2)
{
    if (!t1 && !t2)
        return 1;

    if (!t1 || !t2)
        return 0;

    if (t1->node.type == OPERATOR && t1->node.type == t2->node.type && (t1->node.data.operator == '/' || t1->node.data.operator == '-'))
        return  t1->node.data.operator == t2->node.data.operator && trees_is_equal(t1->right, t2->right) && trees_is_equal(t1->left, t2->left);

    return ((t1->node.type == INTEGER && t1->node.type == t2->node.type && t1->node.data.value_int == t2->node.data.value_int) ||
           (t1->node.type == VARIABLE && t1->node.type == t2->node.type && t1->node.data.variable == t2->node.data.variable)  ||
           (t1->node.type == OPERATOR && t1->node.type == t2->node.type && t1->node.data.operator == t2->node.data.operator)) &&
           ((trees_is_equal(t1->right, t2->right) && trees_is_equal(t1->left, t2->left)) || (trees_is_equal(t1->left, t2->right) && trees_is_equal(t1->right, t2->left)));
}

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
                Tree tmp_1 = t->left->right;
                Tree tmp_2 = t->left->left;
				tree_destroy(&t->right);
				free(t->left);
                t->right = tmp_1;
                t->left = tmp_2;
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
                   // t->node.data.operator = t->right->node.data.operator;
                    char operator = t->right->node.data.operator;
                    Tree tmp_1 = t->right->right;
                    Tree tmp_2 = t->right->left;
                    tree_destroy(&t->left);
                    free(t->right);
                    t->node.type = OPERATOR;
                    t->node.data.operator = operator;
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
                    //t->node.data.operator = t->left->node.data.operator;
                    char operator = t->left->node.data.operator;
                    Tree tmp_1 = t->left->right;
                    Tree tmp_2 = t->left->left;
                    tree_destroy(&t->right);
                    free(t->left);
                    t->node.type = OPERATOR;
                    t->node.data.operator = operator;
                    t->right = tmp_1;
                    t->left = tmp_2;
				}
			}

		}
	} // (a+a+a)/(a+a+a)+(a*v+i*u*v)/(v*a+i*v)+(5*a/(5*b))

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

    if (t->node.type == OPERATOR && (t->node.data.operator == '+' || t->node.data.operator == '-'))
        return find_variable_elem(t->left, c) && find_variable_elem(t->right, c);

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

    if (t->node.type == OPERATOR && (t->node.data.operator == '+' || t->node.data.operator == '-'))
        return find_constant_elem(t->left, n) && find_constant_elem(t->right, n);

	if (!find_constant_elem(t->left, n))
		return find_constant_elem(t->right, n);

	return 1;
}

uint32_t is_variable_in_tree(Tree t, char c)
{
    if (!t)
        return 0;

    if (t->node.data.variable == c) {
        return 1;
    }

    if (t->node.type == OPERATOR && (t->node.data.operator == '+' || t->node.data.operator == '-'))
        return is_variable_in_tree(t->left, c) && is_variable_in_tree(t->right, c);

    if (!is_variable_in_tree(t->left, c))
        return is_variable_in_tree(t->right, c);

    return 1;
}

uint32_t is_constant_in_tree(Tree t, uint32_t n)
{
    if (!t)
        return 0;




    if (t->node.data.value_int == n) {
        return 1;
    }

    if (t->node.type == OPERATOR && (t->node.data.operator == '+' || t->node.data.operator == '-'))
        return is_constant_in_tree(t->left, n) && is_constant_in_tree(t->right, n);

    if (!is_constant_in_tree(t->left, n))
        return is_constant_in_tree(t->right, n);

    return 1;
}

uint32_t is_common(Tree t1, Tree t2)
{
    if (!t1)
        return;

    if (t1->node.type == OPERATOR && t1->node.data.operator == '*')
        return is_common(t1->left, t2) || is_common(t1->right, t2);
    else if (t1->node.type == OPERATOR && (t1->node.data.operator == '-' || t1->node.data.operator == '+'))
        return is_common(t1->left, t2) && is_common(t1->right, t2);
    else if (t1->node.type == VARIABLE)
        return is_variable_in_tree(t2, t1->node.data.variable);
    else if (t1->node.type == INTEGER)
        return is_constant_in_tree(t2, t1->node.data.value_int);
}

void transform(Tree t1, Tree t2)
{
	if (!t1)
		return;


    if (t1->node.type == VARIABLE) {
        if(is_variable_in_tree(t2, t1->node.data.variable)) {
            find_variable_elem(t2, t1->node.data.variable);
            t1->node.type = INTEGER;
            t1->node.data.value_int = 1;
            return;
        }
    }

    if (t2->node.type == VARIABLE) {
        if(is_variable_in_tree(t1, t2->node.data.variable)) {
            find_variable_elem(t1, t2->node.data.variable);
            t2->node.type = INTEGER;
            t2->node.data.value_int = 1;
            return;
        }
    }

    if (t1->node.type == INTEGER) {
        if (is_constant_in_tree(t2, t1->node.data.value_int)) {
            find_constant_elem(t2, t1->node.data.value_int);
            t1->node.type = INTEGER;
            t1->node.data.value_int = 1;
            return;
        }
    }

    if (t2->node.type == INTEGER) {
        if (is_constant_in_tree(t1, t2->node.data.value_int)) {
            find_constant_elem(t1, t2->node.data.value_int);
            t2->node.type = INTEGER;
            t2->node.data.value_int = 1;
            return;
        }
    }

    if (t1->node.type == OPERATOR && t1->node.data.operator == '/' && t1->node.type == t2->node.type) {
        if (is_common(t1->right, t2->right))
            transform(t1->right, t2->right);

        if (is_common(t1->left, t2->left))
            transform(t1->left, t2->left);
        return;
    }

    if (t1->node.type == OPERATOR && t1->node.data.operator == '*') {
        if (is_common(t1->left, t2))
            transform(t1->left, t2);

        if (is_common(t1->right, t2))
            transform(t1->right, t2);

        return ;
    }

    if (t2->node.type == OPERATOR && t2->node.data.operator == '*') {
        if (is_common(t2->left, t1))
            transform(t2->left, t1);

        if (is_common(t2->right, t1))
            transform(t2->right, t1);

        return;
    }

    if (t1->node.type == OPERATOR && (t1->node.data.operator == '-' || t1->node.data.operator == '+')) {
        if (is_common(t1->left, t2) && is_common(t1->right, t2)) {
            transform(t1->left, t1->right);
            transform(t2->left, t2->right);
            return;   
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
        if (trees_is_equal(tree->left, tree->right)) {
            tree->node.type = INTEGER;
            tree->node.data.value_int = 1;
            tree_destroy(&tree->left);
            tree_destroy(&tree->right);
            return;
        }
        transform(tree->left, tree->right);
    }
}