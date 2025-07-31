#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 256

typedef enum {
	TKN_IDENT,
	TKN_INT_LIT,
	TKN_LPAREN,
	TKN_RPAREN,
	TKN_LBRACE,
	TKN_RBRACE,
	TKN_SEMI,
	TKN_EXIT,
	TKN_EOF,
	TKN_UNKNOWN
} TokenType;

typedef struct Token {
	TokenType type;
	char *value;
	int line;
	int col;
	struct Token *next;
} Token;

typedef struct {
	FILE *file;
	int curr_char;
	int line;
	int col;
	char buffer[BUFFER_SIZE];
	int buff_pos;
} Tokenizer;

Tokenizer *init_tokenizer(FILE *file) {
	Tokenizer *t = calloc(1, sizeof(Tokenizer));
	if (!t) {
		return NULL;
	}
	t->file = file;
	t->curr_char = fgetc(file);
	t->line = 1;
	t->col = 1;
	t->buff_pos = 0; // Explicit initialization
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
	token->col = t->col - t->buff_pos;
	token->type = type;
	token->next = NULL;
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
	TokenType type = TKN_IDENT;
	if (strcmp(t->buffer, "exit") == 0) {
		type = TKN_EXIT;
	}
	return create_token(t, type);
}

Token *create_num_token(Tokenizer *t) {
	while (isdigit(t->curr_char)) {
		append_buffer(t, t->curr_char);
		consume(t);
	}
	return create_token(t, TKN_INT_LIT);
}
Token *create_single_char_token(Tokenizer *t) {
	TokenType type = TKN_UNKNOWN;

	switch (t->curr_char) {
	case '(':
		type = TKN_LPAREN;
		break;
	case ')':
		type = TKN_RPAREN;
		break;
	case ';':
		type = TKN_SEMI;
		break;
	case '{':
		type = TKN_LBRACE;
		break;
	case '}':
		type = TKN_RBRACE;
		break;
	}
	clear_buffer(t);
	if (type != TKN_UNKNOWN) { // Only store valid tokens
		append_buffer(t, t->curr_char);
		consume(t);
		return create_token(t, type);
	}
	return NULL;
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
		} else if (isdigit(t->curr_char)) {
			current = create_num_token(t);
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
	Token *eof = calloc(1, sizeof(Token));
	if (eof) {
		eof->type = TKN_EOF;
		eof->value = strdup("");
		eof->line = t->line;
		eof->col = t->col;
		eof->next = NULL;

		if (prev) {
			prev->next = eof;
		} else {
			head = eof;
		}
	}
	return head;
}
void print_tokens(Token *head) {
	const char *type_names[] = {
		"IDENT", "INT_LIT", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "SEMI", "EXIT", "EOF", "UNKNOWN"
	};

	for (Token *current = head; current != NULL; current = current->next) {
		printf("%d:%d Token(%s, '%s')\n",
			   current->line,
			   current->col,
			   type_names[current->type],
			   current->value);
	}
}
int main(int argv, char **argc) {
	if (argv < 2) {
		perror("Incorrect usage, too few argumentes... \n Correct Usage: bsc <flags> filename");
		return 1;
	}
	FILE *file = fopen(argc[1], "r");
	if (file == NULL) {
		perror("Error opening file");
	}
	Tokenizer *t = init_tokenizer(file);
	Token *tokens = tokenize(t);
	print_tokens(tokens);
	free_tokens(tokens);
	free_tokenizer(t);
	return 0;
}
