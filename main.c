#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "tree.h"

int main(void)
{
	Token tokens[TOKEN_SIZE];
	uint32_t token_cap = 0;

	Token token;
	token_next(&token);

	while (token.type != FINAL) {
		tokens[token_cap++] = token;
		token_next(&token);
	}

	for (int i = 0; i < token_cap; ++i)
		token_print(&tokens[i]);

	return 0;
}