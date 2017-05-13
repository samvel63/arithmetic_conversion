#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "tree.h"

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

	for (uint32_t i = 0; i < token_cap; ++i)
		token_print(&tokens[i]);
	printf("\n\n");
	Tree tree;
	tree = tree_create(&tokens[0], 0, token_cap - 1);

	tree_print(tree, 0);
	tree_destroy(&tree);

	return 0;
}