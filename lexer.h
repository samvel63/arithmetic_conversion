#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdbool.h>
#include <inttypes.h>

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
		char		variable;
		bool 		is_left_bracket;
		char 		operator;
	} data;
} Token;


#endif // _LEXER_H_