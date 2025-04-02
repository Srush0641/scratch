#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_PROD 20
#define MAX_SYM 20
#define MAX_FIRST 10
#define MAX_FOLLOW 10
#define MAX_TABLE 10

typedef struct {
    char lhs;
    char rhs[MAX_SYM][MAX_SYM];
    int rhs_count;
} Production;

typedef struct {
    char symbol;
    char first[MAX_FIRST];
    int first_count;
    char follow[MAX_FOLLOW];
    int follow_count;
} SymbolInfo;

Production grammar[MAX_PROD];
SymbolInfo symbols[MAX_SYM];
int num_prods = 0;
int num_symbols = 0;
char terminals[MAX_SYM];
int num_terminals = 0;
char non_terminals[MAX_SYM];
int num_non_terminals = 0;
char parsing_table[MAX_TABLE][MAX_TABLE][MAX_SYM];
int table_rows = 0, table_cols = 0;

void add_symbol(char c) {
    if (c == '#' || c == ' ') return;
    
    for (int i = 0; i < num_symbols; i++) {
        if (symbols[i].symbol == c) return;
    }
    
    symbols[num_symbols].symbol = c;
    symbols[num_symbols].first_count = 0;
    symbols[num_symbols].follow_count = 0;
    num_symbols++;
    
    if (isupper(c)) {
        non_terminals[num_non_terminals++] = c;
    } else {
        terminals[num_terminals++] = c;
    }
}

void add_to_first(char sym, char val) {
    for (int i = 0; i < num_symbols; i++) {
        if (symbols[i].symbol == sym) {
            for (int j = 0; j < symbols[i].first_count; j++) {
                if (symbols[i].first[j] == val) return;
            }
            symbols[i].first[symbols[i].first_count++] = val;
            return;
        }
    }
}

void add_to_follow(char sym, char val) {
    for (int i = 0; i < num_symbols; i++) {
        if (symbols[i].symbol == sym) {
            for (int j = 0; j < symbols[i].follow_count; j++) {
                if (symbols[i].follow[j] == val) return;
            }
            symbols[i].follow[symbols[i].follow_count++] = val;
            return;
        }
    }
}

void compute_first() {
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < num_prods; i++) {
            char lhs = grammar[i].lhs;
            for (int j = 0; j < grammar[i].rhs_count; j++) {
                char *rhs = grammar[i].rhs[j];
                int k = 0;
                bool has_epsilon = true;
                
                while (rhs[k] != '\0' && has_epsilon) {
                    has_epsilon = false;
                    char sym = rhs[k];
                    
                    if (!isupper(sym) && sym != '#') { // Terminal
                        int old_count = symbols[0].first_count;
                        add_to_first(lhs, sym);
                        if (symbols[0].first_count > old_count) changed = true;
                        break;
                    } else if (sym == '#') { // Epsilon
                        int old_count = symbols[0].first_count;
                        add_to_first(lhs, '#');
                        if (symbols[0].first_count > old_count) changed = true;
                    } else { // Non-terminal
                        for (int m = 0; m < num_symbols; m++) {
                            if (symbols[m].symbol == sym) {
                                for (int n = 0; n < symbols[m].first_count; n++) {
                                    if (symbols[m].first[n] != '#') {
                                        int old_count = symbols[0].first_count;
                                        add_to_first(lhs, symbols[m].first[n]);
                                        if (symbols[0].first_count > old_count) changed = true;
                                    } else {
                                        has_epsilon = true;
                                    }
                                }
                                break;
                            }
                        }
                        k++;
                    }
                }
                
                if (has_epsilon) {
                    int old_count = symbols[0].first_count;
                    add_to_first(lhs, '#');
                    if (symbols[0].first_count > old_count) changed = true;
                }
            }
        }
    } while (changed);
}

void compute_follow() {
    add_to_follow(grammar[0].lhs, '$');
    
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < num_prods; i++) {
            char lhs = grammar[i].lhs;
            for (int j = 0; j < grammar[i].rhs_count; j++) {
                char *rhs = grammar[i].rhs[j];
                int len = strlen(rhs);
                
                for (int k = 0; k < len; k++) {
                    char B = rhs[k];
                    if (!isupper(B)) continue; // Skip terminals
                    
                    if (k < len - 1) {
                        char beta = rhs[k+1];
                        if (!isupper(beta)) { // Terminal
                            int old_count = symbols[0].follow_count;
                            add_to_follow(B, beta);
                            if (symbols[0].follow_count > old_count) changed = true;
                        } else { // Non-terminal
                            for (int m = 0; m < num_symbols; m++) {
                                if (symbols[m].symbol == beta) {
                                    for (int n = 0; n < symbols[m].first_count; n++) {
                                        if (symbols[m].first[n] != '#') {
                                            int old_count = symbols[0].follow_count;
                                            add_to_follow(B, symbols[m].first[n]);
                                            if (symbols[0].follow_count > old_count) changed = true;
                                        }
                                    }
                                    break;
                                }
                            }
                            
                            bool all_epsilon = true;
                            for (int m = k+1; m < len && all_epsilon; m++) {
                                char sym = rhs[m];
                                if (!isupper(sym)) {
                                    all_epsilon = false;
                                    break;
                                }
                                bool has_epsilon = false;
                                for (int n = 0; n < num_symbols; n++) {
                                    if (symbols[n].symbol == sym) {
                                        for (int p = 0; p < symbols[n].first_count; p++) {
                                            if (symbols[n].first[p] == '#') {
                                                has_epsilon = true;
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                if (!has_epsilon) all_epsilon = false;
                            }
                            
                            if (all_epsilon) {
                                for (int m = 0; m < num_symbols; m++) {
                                    if (symbols[m].symbol == lhs) {
                                        for (int n = 0; n < symbols[m].follow_count; n++) {
                                            int old_count = symbols[0].follow_count;
                                            add_to_follow(B, symbols[m].follow[n]);
                                            if (symbols[0].follow_count > old_count) changed = true;
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    } else {
                        for (int m = 0; m < num_symbols; m++) {
                            if (symbols[m].symbol == lhs) {
                                for (int n = 0; n < symbols[m].follow_count; n++) {
                                    int old_count = symbols[0].follow_count;
                                    add_to_follow(B, symbols[m].follow[n]);
                                    if (symbols[0].follow_count > old_count) changed = true;
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
}

void create_parsing_table() {
    // Initialize table with empty entries
    for (int i = 0; i < num_non_terminals; i++) {
        for (int j = 0; j < num_terminals + 1; j++) {
            strcpy(parsing_table[i][j], "");
        }
    }
    
    // Fill the table
    for (int i = 0; i < num_prods; i++) {
        char lhs = grammar[i].lhs;
        int lhs_index = -1;
        for (int k = 0; k < num_non_terminals; k++) {
            if (non_terminals[k] == lhs) {
                lhs_index = k;
                break;
            }
        }
        
        for (int j = 0; j < grammar[i].rhs_count; j++) {
            char *rhs = grammar[i].rhs[j];
            char first_sym = rhs[0];
            
            if (first_sym == '#') { // Epsilon production
                for (int k = 0; k < num_symbols; k++) {
                    if (symbols[k].symbol == lhs) {
                        for (int m = 0; m < symbols[k].follow_count; m++) {
                            char follow_sym = symbols[k].follow[m];
                            int sym_index = -1;
                            for (int n = 0; n < num_terminals; n++) {
                                if (terminals[n] == follow_sym || follow_sym == '$') {
                                    sym_index = n;
                                    break;
                                }
                            }
                            if (sym_index != -1) {
                                char prod[MAX_SYM];
                                sprintf(prod, "%c->%s", lhs, rhs);
                                strcpy(parsing_table[lhs_index][sym_index], prod);
                            }
                        }
                        break;
                    }
                }
            } else if (!isupper(first_sym)) { // Terminal
                int sym_index = -1;
                for (int k = 0; k < num_terminals; k++) {
                    if (terminals[k] == first_sym) {
                        sym_index = k;
                        break;
                    }
                }
                if (sym_index != -1) {
                    char prod[MAX_SYM];
                    sprintf(prod, "%c->%s", lhs, rhs);
                    strcpy(parsing_table[lhs_index][sym_index], prod);
                }
            } else { // Non-terminal
                for (int k = 0; k < num_symbols; k++) {
                    if (symbols[k].symbol == first_sym) {
                        for (int m = 0; m < symbols[k].first_count; m++) {
                            char first_val = symbols[k].first[m];
                            if (first_val != '#') {
                                int sym_index = -1;
                                for (int n = 0; n < num_terminals; n++) {
                                    if (terminals[n] == first_val) {
                                        sym_index = n;
                                        break;
                                    }
                                }
                                if (sym_index != -1) {
                                    char prod[MAX_SYM];
                                    sprintf(prod, "%c->%s", lhs, rhs);
                                    strcpy(parsing_table[lhs_index][sym_index], prod);
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}

void print_parsing_table() {
    printf("\nLL(1) Parsing Table:\n");
    printf("\t");
    for (int i = 0; i < num_terminals; i++) {
        printf("%c\t", terminals[i]);
    }
    printf("$\t\n");
    
    for (int i = 0; i < num_non_terminals; i++) {
        printf("%c\t", non_terminals[i]);
        for (int j = 0; j < num_terminals + 1; j++) {
            if (strlen(parsing_table[i][j]) > 0) {
                printf("%s\t", parsing_table[i][j]);
            } else {
                printf("\t");
            }
        }
        printf("\n");
    }
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &num_prods);
    getchar();
    
    printf("Enter productions (format: S->aB|bA, use # for epsilon):\n");
    for (int i = 0; i < num_prods; i++) {
        char input[50];
        printf("Production %d: ", i+1);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        
        grammar[i].lhs = input[0];
        grammar[i].rhs_count = 0;
        add_symbol(grammar[i].lhs);
        
        char *token = strtok(input + 3, "|");
        while (token != NULL) {
            strcpy(grammar[i].rhs[grammar[i].rhs_count++], token);
            for (int j = 0; token[j] != '\0'; j++) {
                add_symbol(token[j]);
            }
            token = strtok(NULL, "|");
        }
    }
    
    compute_first();
    compute_follow();
    create_parsing_table();
    print_parsing_table();
    
    return 0;
}