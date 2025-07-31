#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 256

typedef enum {
// Keywords
	TKN_FN_DEC,    // fn 
	TKN_VAR_DEC,   // let 
	TKN_RTN,       // back
	TKN_IF,        // if 
	TKN_UNLESS,    // unless 
	TKN_ELSE,      // else 
	TKN_WHILE,     // while
	TKN_FOR,       // for 
	TKN_FOR_INTER, // in
	TKN_TRUE,      // true
	TKN_FALSE,     // false
	TKN_AND,       // and
	TKN_OR,        // or
// Operators
	TKN_ASSIGN,      // =
	TKN_EQUAL,       // ==
	TKN_INEQUAL,     // !=
	TKN_LESS_THAN,   // < 
	TKN_GREAT_THAN,  // >
	TKN_LESS_EQUAL,  // <=
	TKN_GREAT_EQUAL, // >=
	TKN_ADD,         // +
	TKN_SUB,         // - 
	TKN_MULT,        // *
	TKN_DIVI,        // / 
	TKN_MODULO,      // % 
	TKN_LOG_NOT,     // !
// Delimiters
    TKN_LPAREN,   // (
    TKN_RPAREN,   // )
    TKN_LBRACE,   // {
    TKN_RBRACE,   // }
	TKN_LBRACK,   // [
	TKN_RBRACK,   // ]
	TKN_COMMA,    // ,
	TKN_SEMI,     // ; 
	TKN_DB_QUOTE, // "
	TKN_QUOTE,    // ' 
// Literals
	TKN_NUM_LIT,  // 
	TKN_STR_LIT,
// Identifiers 
	TKN_IDENT,
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
