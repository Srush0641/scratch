#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_STATES 10
#define MAX_SYMBOLS 10

typedef struct {
    char lexeme[50];
    char type[20];
} Token;

Token tokens[MAX_TOKENS];
int token_count = 0;

// Symbol Table Structure
typedef struct {
    char name[50];
    char type[20];
    int scope;
} Symbol;

Symbol symbol_table[MAX_TOKENS];
int symbol_count = 0;

void add_token(char *lexeme, char *type) {
    strcpy(tokens[token_count].lexeme, lexeme);
    strcpy(tokens[token_count].type, type);
    token_count++;
}

void add_symbol(char *name, char *type, int scope) {
    strcpy(symbol_table[symbol_count].name, name);
    strcpy(symbol_table[symbol_count].type, type);
    symbol_table[symbol_count].scope = scope;
    symbol_count++;
}

void tokenize(char *code) {
    char *keywords[] = {"int", "float", "char", "return", "if", "else", "while"};
    char buffer[50];
    int i = 0, j = 0;
    
    while (code[i] != '\0') {
        if (isalpha(code[i])) {
            j = 0;
            while (isalnum(code[i])) {
                buffer[j++] = code[i++];
            }
            buffer[j] = '\0';
            
            int is_keyword = 0;
            for (int k = 0; k < 7; k++) {
                if (strcmp(buffer, keywords[k]) == 0) {
                    is_keyword = 1;
                    break;
                }
            }
            
            if (is_keyword)
                add_token(buffer, "KEYWORD");
            else {
                add_token(buffer, "IDENTIFIER");
                add_symbol(buffer, "UNKNOWN", 0);
            }
        } else if (isdigit(code[i])) {
            j = 0;
            while (isdigit(code[i])) {
                buffer[j++] = code[i++];
            }
            buffer[j] = '\0';
            add_token(buffer, "NUMBER");
        } else if (strchr("+-*/=;(){}", code[i])) {
            buffer[0] = code[i];
            buffer[1] = '\0';
            add_token(buffer, "SYMBOL");
            i++;
        } else {
            i++;
        }
    }
}

void print_tokens() {
    printf("\nTokens:\n");
    for (int i = 0; i < token_count; i++) {
        printf("%s - %s\n", tokens[i].lexeme, tokens[i].type);
    }
}

void print_symbol_table() {
    printf("\nSymbol Table:\n");
    printf("Name\tType\tScope\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s\t%s\t%d\n", symbol_table[i].name, symbol_table[i].type, symbol_table[i].scope);
    }
}

int main() {
    char code[500];
    printf("Enter the C code:\n");
    fgets(code, 500, stdin);
    
    tokenize(code);
    print_tokens();
    print_symbol_table();
    return 0;
}
