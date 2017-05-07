#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "lexer.h"

void token_next(Token *t)
{
	static bool can_be_unary = true;
	char c;

	do {
		c = fgetc(stdin);
	} while (isspace(c));

	if (c == EOF) {
		t->type = FINAL;
	} else if (isalpha(c)) {
		t->type = VARIABLE;
		t->data.variable = c;
		can_be_unary = false;
	} else if (isdigit(c)) {
		float result;
		ungetc(c, stdin);
		scanf("%f", &result);

		if (result == (int) result) {
			t->type = INTEGER;
			t->data.value_int = (int) result;
		} else {
			t->type = FLOATING;
			t->data.value_float = result;
		}
		can_be_unary = false;
	} else if (c == '(' || c == ')') {
		t->type = BRACKET;
		t->data.is_left_bracket = (c == '(');
		can_be_unary = t->data.is_left_bracket;
	} else if (can_be_unary && (c == '-' || c == '+')) {
		int m = (c == '+') ? +1 : -1;

		do {
			c = fgetc(stdin);
		} while (isspace(c));

		if (isdigit(c)) {
			ungetc(c, stdin);
			token_next(t);

			if (t->type == INTEGER)
				t->data.value_int *= m;
			else
				t->data.value_float *= m;
		} else {
			ungetc(c, stdin);
			t->type = OPERATOR;
			t->data.operator = '-';
			can_be_unary = true;
		}
	} else {
		t->type = OPERATOR;
		t->data.operator = c;
		can_be_unary = true;
	}
}

void token_print(Token *t)
{
	switch (t->type) {

		case FINAL:
			break;

		case INTEGER:
			printf("%u", t->data.value_int);
			break;

		case FLOATING:
			printf("%lf", t->data.value_float);
			break;

		case VARIABLE:
			printf("%c", t->data.variable);
			break;

		case OPERATOR:
			printf("%c", t->data.operator);
			break;

		case BRACKET:
			printf("%c", t->data.is_left_bracket ? '(' : ')');
			break;
	}
}