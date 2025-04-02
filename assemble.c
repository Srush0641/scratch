#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

// Stack Structure
typedef struct {
    char items[MAX][MAX];
    int top;
} Stack;

// Initialize Stack
void init(Stack *s) {
    s->top = -1;
}

// Push to Stack
void push(Stack *s, char *value) {
    if (s->top < MAX - 1) {
        strcpy(s->items[++(s->top)], value);
    } else {
        printf("Stack Overflow!\n");
    }
}

// Pop from Stack
char *pop(Stack *s) {
    if (s->top == -1) {
        return "";
    }
    return s->items[(s->top)--];
}

// Peek from Stack
char peek(Stack *s) {
    return (s->top == -1) ? '\0' : s->items[s->top][0];
}

// Operator Precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Convert Infix to Postfix
void infixToPostfix(char *infix, char *postfix) {
    Stack s;
    init(&s);
    int j = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        if (isalnum(infix[i])) {  // If operand (letter or number)
            while (isalnum(infix[i])) {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
            i--;  
        } else if (infix[i] == '(') {
            char op[2] = {infix[i], '\0'};
            push(&s, op);
        } else if (infix[i] == ')') {
            while (s.top != -1 && peek(&s) != '(') {
                postfix[j++] = pop(&s)[0];
                postfix[j++] = ' ';
            }
            pop(&s); // Remove '('
        } else {  
            while (s.top != -1 && precedence(peek(&s)) >= precedence(infix[i])) {
                postfix[j++] = pop(&s)[0];
                postfix[j++] = ' ';
            }
            char op[2] = {infix[i], '\0'};
            push(&s, op);
        }
    }
    
    while (s.top != -1) {  // Pop remaining operators
        postfix[j++] = pop(&s)[0];
        postfix[j++] = ' ';
    }
    postfix[j] = '\0';
}

// Generate Assembly from Postfix
void generateAssembly(char *postfix) {
    Stack operands;
    init(&operands);
    int reg = 0;
    char op1[MAX], op2[MAX], temp[MAX];

    printf("\nGenerated Assembly Code:\n");

    for (int i = 0; postfix[i] != '\0'; i++) {
        if (isalnum(postfix[i])) {  
            char operand[MAX] = "";
            int k = 0;
            while (isalnum(postfix[i])) {
                operand[k++] = postfix[i++];
            }
            operand[k] = '\0';
            push(&operands, operand);
        } else if (postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/') {  
            if (operands.top < 1) {
                printf("Error: Insufficient operands!\n");
                return;
            }
            strcpy(op2, pop(&operands));
            strcpy(op1, pop(&operands));

            printf("MOV R%d, %s\n", reg, op1);

            if (postfix[i] == '+') printf("ADD R%d, %s\n", reg, op2);
            if (postfix[i] == '-') printf("SUB R%d, %s\n", reg, op2);
            if (postfix[i] == '*') printf("MUL R%d, %s\n", reg, op2);
            if (postfix[i] == '/') printf("DIV R%d, %s\n", reg, op2);

            sprintf(temp, "R%d", reg);
            push(&operands, temp);
            reg++;
        }
    }

    printf("Result stored in: %s\n", pop(&operands));
}

int main() {
    char infix[MAX], postfix[MAX];

    printf("Enter the arithmetic expression (e.g., a+b*5): ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = '\0';  // Remove newline

    infixToPostfix(infix, postfix);
    printf("Postfix Expression: %s\n", postfix);
    generateAssembly(postfix);

    return 0;
}
