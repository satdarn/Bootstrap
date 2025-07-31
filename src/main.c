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
	char curr_char;
	int line;
	int col;
	char buffer[BUFFER_SIZE];
	int buff_pos;
} Tokenizer;

Tokenizer *init_tokenizer(FILE *file) {
	Tokenizer *t = malloc(sizeof(Tokenizer));
	t->file = file;
	t->curr_char = fgetc(file);
	t->line = 1;
	t->col = 1;
	return t;
}
//Token* create_token(Tokenizer *t){
//	Token* token = malloc(sizeof(Token));
//	token->line = t->line;
//	token->col = t->col;
//	token->value[0]
//}
//
void consume(Tokenizer *t) {
	if (t->curr_char == '\n') {
		t->line++;
		t->col = 1;
	} else {
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
	}
}
void clear_buffer(Tokenizer *t) {
	t->buff_pos = 0;
	t->buffer[0] = '\0';
}

Token *tokenize(Tokenizer *t) {
	while (t->curr_char != EOF) {
		if (isalpha(t->curr_char)) {
			while (isalnum(t->curr_char) || t->curr_char == '_') {
				append_buffer(t, t->curr_char);
				consume(t);
			}
			append_buffer(t, '\0');
			printf("%s\n", t->buffer);
			continue;
		}
		if (isdigit(t->curr_char)) {
			while (isdigit(t->curr_char)) {
				append_buffer(t, t->curr_char);
				consume(t);
			}
			append_buffer(t, '\0');
			printf("%s\n", t->buffer);
			continue;
		}
		else {
			printf("%c\n", t->curr_char);
			consume(t);
		}
		clear_buffer(t);
	}
	return NULL;
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
	tokenize(t);
	fclose(file);
	return 0;
}
