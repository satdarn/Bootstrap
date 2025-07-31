#include "tokenizer.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    
    Tokenizer *t = init_tokenizer(file);
    if (!t) {
        fclose(file);
        return 1;
    }
    
    Token *tokens = tokenize(t);
    print_tokens(tokens);
    free_tokens(tokens);
    free_tokenizer(t);
    
    return 0;
}
