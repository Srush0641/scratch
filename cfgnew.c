#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RULES 10
#define MAX_LEN 100

char rules[MAX_RULES][MAX_LEN]; // Store user-defined rules
int numRules;
char *input;
int index = 0;

// Function to find a production for a given non-terminal
char* findProduction(char nonTerminal) {
    for (int i = 0; i < numRules; i++) {
        if (rules[i][0] == nonTerminal && rules[i][1] == '=') {
            return &rules[i][2];  // Return the production part
        }
    }
    return NULL;
}

// Recursive function to parse input string
void parseNonTerminal(char nonTerminal) {
    char *production = findProduction(nonTerminal);
    
    if (!production) {
        printf("Error: No rule for %c\n", nonTerminal);
        exit(1);
    }

    for (int i = 0; production[i] != '\0'; i++) {
        if (production[i] == '|') continue;  // Handle multiple productions later

        if (production[i] >= 'A' && production[i] <= 'Z') {
            // Recursively parse non-terminal
            parseNonTerminal(production[i]);
        } else if (input[index] == production[i]) {
            index++;  // Consume terminal
        } else if (production[i] == 'e') {
            return;  // ε transition
        } else {
            printf("Error: Unexpected character '%c' at position %d\n", input[index], index);
            exit(1);
        }
    }
}

// Function to start parsing
void parse() {
    parseNonTerminal(rules[0][0]);  // Start with first non-terminal
    if (input[index] != '\0') {
        printf("Error: Unexpected character '%c' at position %d\n", input[index], index);
        exit(1);
    } else {
        printf("Valid string: %s\n", input);
    }
}

int main() {
    char str[MAX_LEN];

    printf("Enter number of rules: ");
    scanf("%d", &numRules);
    getchar();  // Consume newline

    printf("Enter grammar rules (Format: S=aSb|e):\n");
    for (int i = 0; i < numRules; i++) {
        fgets(rules[i], MAX_LEN, stdin);
        rules[i][strcspn(rules[i], "\n")] = '\0';  // Remove newline
    }

    printf("Enter a string to parse: ");
    scanf("%s", str);

    input = str;
    index = 0;

    parse();  // Start parsing

    return 0;
}
