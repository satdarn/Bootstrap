#include "tokenizer.h"


Tokenizer *init_tokenizer(FILE *file) {
	Tokenizer *t = calloc(1, sizeof(Tokenizer));
	if (!t) {
		return NULL;
	}
	t->file = file;
	t->curr_char = fgetc(file);
	t->line = 1;
	t->col = 1;
	t->buff_pos = 0;
	t->buffer[0] = '\0';
	return t;
}

void clear_buffer(Tokenizer *t) {
	t->buff_pos = 0;
	t->buffer[0] = '\0';
}

void free_tokenizer(Tokenizer *t) {
	if (t) {
		if (t->file)
			fclose(t->file);
		free(t);
	}
}

Token *create_token(Tokenizer *t, TokenType type) {
	Token *token = calloc(1, sizeof(Token));
	if (!token)
		return NULL;

	token->value = strndup(t->buffer, t->buff_pos);
	if (!token->value) {
		free(token);
		return NULL;
	};
	token->col = t->col;
	token->line = t->line;
	token->type = type;
	token->next = NULL;
	//printf("%s\n", token->value);
	clear_buffer(t);
	return token;
}

void free_tokens(Token *head) {
	Token *current = head;
	while (current != NULL) {
		Token *next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

void consume(Tokenizer *t) {
	if (t->curr_char == '\n') {
		t->line++;
		t->col = 1;
	} else if (t->curr_char != EOF) {
		t->col++;
	}
	t->curr_char = fgetc(t->file);
}

int peek(Tokenizer *t) {
	int c = fgetc(t->file);
	ungetc(c, t->file);
	return c;
}

void append_buffer(Tokenizer *t, char c) {
	if (t->buff_pos < BUFFER_SIZE - 1) {
		t->buffer[t->buff_pos++] = c;
		t->buffer[t->buff_pos] = '\0';
	}
}

Token *create_ident_token(Tokenizer *t) {
	while (isalnum(t->curr_char) || t->curr_char == '_') {
		append_buffer(t, t->curr_char);
		consume(t);
	}
	return create_token(t, TKN_IDENT);
}
Token *create_num_literal_token(Tokenizer *t) {
	while (isdigit(t->curr_char)) {
		append_buffer(t, t->curr_char);
		consume(t);
	}
	return create_token(t, TKN_NUM_LIT);
}
Token *create_str_literal_token(Tokenizer *t) {
	append_buffer(t, t->curr_char);
	consume(t);
	while (t->curr_char != '\"' && t->curr_char >= 0) {
		append_buffer(t, t->curr_char);
		consume(t);
	}
	append_buffer(t, t->curr_char);
	consume(t);
	return create_token(t, TKN_STR_LIT);
}

Token *create_single_char_token(Tokenizer *t) {
	TokenType type = TKN_UNKNOWN;
	clear_buffer(t);
	switch (t->curr_char) {
	case '=':
		type = TKN_ASSIGN;
		if (peek(t) == '=') {
			append_buffer(t, t->curr_char);
			consume(t);
			type = TKN_ASSIGN;
		}
		break;
	case '<':
		type = TKN_LESS_THAN;
		if (peek(t) == '=') {
			append_buffer(t, t->curr_char);
			consume(t);
			type = TKN_LESS_EQUAL;
		}
		break;
	case '>':
		type = TKN_GREAT_THAN;
		if (peek(t) == '=') {
			append_buffer(t, t->curr_char);
			consume(t);
			type = TKN_GREAT_EQUAL;
		}
		break;
	case '+':
		type = TKN_ADD;
		break;
	case '-':
		type = TKN_SUB;
		break;
	case '*':
		type = TKN_MULT;
		break;
	case '/':
		type = TKN_DIVI;
		break;
	case '%':
		type = TKN_MODULO;
		break;
	case '!':
		type = TKN_LOG_NOT;
		break;
	case '(':
		type = TKN_LPAREN;
		break;
	case ')':
		type = TKN_RPAREN;
		break;
	case '{':
		type = TKN_LBRACE;
		break;
	case '}':
		type = TKN_RBRACE;
		break;
	case ',':
		type = TKN_COMMA;
		break;
	case ';':
		type = TKN_SEMI;
		break;
	case '\"':
		type = TKN_DB_QUOTE;
		break;
	case '\'':
		type = TKN_QUOTE;
		break;
	}
	if (type != TKN_UNKNOWN) {
		append_buffer(t, t->curr_char);
		consume(t);
		return create_token(t, type);
	}
	return NULL;
}
void keyword_token_pass(Token *token) {
	if (strcmp(token->value, "fn") == 0) {
		token->type = TKN_FN_DEC;
	}
	if (strcmp(token->value, "let") == 0) {
		token->type = TKN_VAR_DEC;
	}
	if (strcmp(token->value, "back") == 0) {
		token->type = TKN_RTN;
	}
	if (strcmp(token->value, "if") == 0) {
		token->type = TKN_IF;
	}
	if (strcmp(token->value, "unless") == 0) {
		token->type = TKN_UNLESS;
	}
	if (strcmp(token->value, "else") == 0) {
		token->type = TKN_ELSE;
	}
	if (strcmp(token->value, "WHILE") == 0) {
		token->type = TKN_WHILE;
	}
	if (strcmp(token->value, "for") == 0) {
		token->type = TKN_FOR;
	}
	if (strcmp(token->value, "in") == 0) {
		token->type = TKN_FOR_INTER;
	}
	if (strcmp(token->value, "true") == 0) {
		token->type = TKN_TRUE;
	}
	if (strcmp(token->value, "false") == 0) {
		token->type = TKN_FALSE;
	}
	if (strcmp(token->value, "and") == 0) {
		token->type = TKN_AND;
	}
	if (strcmp(token->value, "or") == 0) {
		token->type = TKN_OR;
	}
}

Token *tokenize(Tokenizer *t) {
	Token *head = NULL;
	Token *prev = NULL;

	while (t->curr_char != EOF) {
		while (isspace(t->curr_char)) {
			consume(t);
		}
		if (t->curr_char == EOF) {
			break;
		}
		Token *current = NULL;

		if (isalpha(t->curr_char)) {
			current = create_ident_token(t);
			keyword_token_pass(current);
		} else if (isdigit(t->curr_char)) {
			current = create_num_literal_token(t);
		} else if (t->curr_char == '\"') {
			current = create_str_literal_token(t);
		} else {
			current = create_single_char_token(t);
		}
		if (!current) {
			consume(t);
			continue;
		}
		if (!head) {
			head = current;
		} else {
			prev->next = current;
		}
		prev = current;
	}
	return head;
}

void print_tokens(Token *head) {
	for (Token *current = head; current != NULL; current = current->next) {
		if (current->type != 39) {
			printf("%d:%d Token(%s, %d)\n",
				   current->line,
				   current->col,
				   current->value,
				   current->type);
		} else {
			printf("Unknown");
		}
	}
}
