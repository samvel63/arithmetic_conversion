#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdbool.h>
#include <inttypes.h>

#include "mystring.h"

#define TOKEN_SIZE 256

typedef enum {
	INTEGER,
	FLOATING,
	VARIABLE,
	BRACKET,
	OPERATOR,
	FINAL
} Token_type;

typedef struct {
	Token_type type;
	union {
		uint32_t 	value_int;
		float		value_float;
		String		variable;
		bool 		is_left_bracket;
		char 		operator;
	} data;
} Token;

void token_next(Token *t);
void token_print(Token *t);

#endif // _LEXER_H_