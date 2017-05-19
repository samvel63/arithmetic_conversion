#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "tree.h"
#include "transform.h"
#include "mystring.h"

uint32_t main(void)
{
	Token tokens[TOKEN_SIZE];
	uint32_t token_cap = 0;

	Token token;
	token_next(&token);

	while (token.type != FINAL) {
		tokens[token_cap++] = token;
		token_next(&token);
	}

	if (token_cap == 0)
		exit(0);

	printf("\n\nВыражение в линейном представлении:\n");
	for (uint32_t i = 0; i < token_cap; ++i) {
		token_print(&tokens[i]);
	}

	printf("\n\nДерево выражения до преобразования:\n");
	Tree tree;
	tree = tree_create(&tokens[0], 0, token_cap - 1);
	tree_print(tree, 0);
	printf("\n\n");


	printf("\n\nДерево выражения после преобразования:\n");
    find_det(tree);
    tree_simplify(tree);
    tree_print(tree, 0);

	printf("\n\nВыражение в линейном представлении после преобразования:\n");
	tree_infix(tree);
	printf("\n\n");
	tree_destroy(&tree);
	printf("\n\n");
	return 0;
}