#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tree.h"

uint32_t get_priority(char c)
{
	switch(c) {
		case '+': case '-': return 1;
		case '*': case '/': return 2;
		case '^': return 3;
	}
	return 18; // Priority of numbers, brackets, variables, etc
}

Tree tree_create(Token *tokens, uint32_t idx_left, uint32_t idx_right)
{
	Tree t = (Tree)malloc(sizeof(struct _tree));

	if (idx_left > idx_right)
		return NULL;

	if (idx_left == idx_right) {
		t->node  = tokens[idx_left];
		t->left  = NULL;
		t->right = NULL;
		return t;
	}

	uint32_t priority = 0;
	uint32_t priority_min = get_priority('a');
	uint32_t brackets = 0;
	uint32_t op_pos = 0;

	for (uint32_t i = idx_left; i < idx_right; ++i) {
		if ((tokens[i].type == BRACKET) && tokens[i].data.is_left_bracket) {
			++brackets;
			continue;
		}
		if ((tokens[i].type == BRACKET) && !tokens[i].data.is_left_bracket) {
			--brackets;
			continue;
		}
		if (brackets > 0)
			continue;
		if (tokens[i].type == OPERATOR) {
			priority = get_priority(tokens[i].data.operator);
			if (priority <= priority_min) {
				priority_min = priority;
				op_pos = i;
			}
		}
	}
	if ((priority_min == get_priority('a')) 	&&
		(tokens[idx_left].type == BRACKET) 		&&
		 tokens[idx_left].data.is_left_bracket	&&
		(tokens[idx_right].type == BRACKET)		&&
		!tokens[idx_right].data.is_left_bracket) {
		free(t);
		return tree_create(tokens, idx_left + 1, idx_right - 1);
	}

	if (tokens[op_pos].data.operator == '^') {
        brackets = 0;
        for (uint32_t i = op_pos; i >= idx_left; --i) {
            if ((tokens[i].type == BRACKET) && !(tokens[i].data.is_left_bracket)) {
                ++brackets;
                continue;
            }
            if ((tokens[i].type == BRACKET) && (tokens[i].data.is_left_bracket)) {
                --brackets;
                continue;
            }
            if (brackets > 0) {
                continue;
            }
            if (tokens[i].type == OPERATOR) {
                priority = get_priority(tokens[i].data.operator);
                if (priority == 3) {
                    op_pos = i;
                }
            }
        }
    }

    t->node  = tokens[op_pos];
    t->left  = tree_create(tokens, idx_left, op_pos - 1);
    t->right = tree_create(tokens, op_pos + 1, idx_right);
    if (t->right == NULL) {
        fprintf(stderr, "Error: operator at the expression's end.");
        exit(1);
    }
    return t;
}

void tree_destroy(Tree *t)
{
    if ((*t) != NULL) {
        tree_destroy(&((*t)->left));
        tree_destroy(&((*t)->right));
    }
    free(*t);
    *t = NULL;
}

void tree_infix(Tree t)
{
    if (t != NULL) {
        if (t->left && t->right)
            printf("(");

        tree_infix(t->left);
        token_print(&(t->node));
        tree_infix(t->right);

        if (t->right && t->left)
            printf(")");
    }
}

void tree_print(Tree t, uint32_t depth)
{
    if (t != NULL) {
        for (uint32_t i = 0; i < depth; ++i) {
            printf("\t");
        }
        token_print(&(t->node));
        printf("\n");
        tree_print(t->left,  depth + 1);
        tree_print(t->right, depth + 1);
    }
}