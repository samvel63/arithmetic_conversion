#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdbool.h>

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
		int 	value_int;
		float	value_float;
		char	variable;
		bool 	is_left_bracket;
		char 	operator;
	} data;
} Token;


#endif // _LEXER_H_