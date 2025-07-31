#ifndef TOKENIZER_H
#define TOKENIZER_H

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

// Initialization/cleanup
Tokenizer *init_tokenizer(FILE *file);
void free_tokenizer(Tokenizer *t);
void free_tokens(Token *head);

// Token creation
Token *tokenize(Tokenizer *t);

// Utility functions
void print_tokens(Token *head);

#endif // TOKENIZER_H
