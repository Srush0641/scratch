#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STATES 10
#define MAX_SYMBOLS 10

int nfa[MAX_STATES][MAX_SYMBOLS][MAX_STATES], dfa[MAX_STATES][MAX_SYMBOLS];
int nfa_final_states[MAX_STATES], dfa_final_states[MAX_STATES];
int nfa_state_count, nfa_symbol_count, dfa_state_count = 0;

// Function to find closure
void find_closure(int state, int closure[], int *size) {
    int i;
    closure[*size] = state;
    (*size)++;
    for (i = 0; i < MAX_STATES; i++) {
        if (nfa[state][0][i] && !closure[i]) { // Epsilon transition
            find_closure(i, closure, size);
        }
    }
}

// Function to convert NFA to DFA
void convert_nfa_to_dfa() {
    int queue[MAX_STATES][MAX_STATES], front = 0, rear = 0;
    int state_map[MAX_STATES] = {0};
    int i, j, k, symbol, size;
    
    int start_closure[MAX_STATES] = {0};
    size = 0;
    find_closure(0, start_closure, &size);
    
    memcpy(queue[rear++], start_closure, sizeof(start_closure));
    state_map[0] = dfa_state_count++;
    
    while (front < rear) {
        int current[MAX_STATES];
        memcpy(current, queue[front++], sizeof(current));
        
        for (symbol = 1; symbol <= nfa_symbol_count; symbol++) {
            int new_state[MAX_STATES] = {0}, new_size = 0;
            
            for (i = 0; i < MAX_STATES; i++) {
                if (current[i]) {
                    for (j = 0; j < MAX_STATES; j++) {
                        if (nfa[i][symbol][j]) {
                            find_closure(j, new_state, &new_size);
                        }
                    }
                }
            }
            
            int found = -1;
            for (i = 0; i < rear; i++) {
                if (memcmp(queue[i], new_state, sizeof(new_state)) == 0) {
                    found = state_map[i];
                    break;
                }
            }
            
            if (found == -1) {
                memcpy(queue[rear], new_state, sizeof(new_state));
                state_map[rear] = dfa_state_count;
                dfa_state_count++;
                found = state_map[rear];
                rear++;
            }
            
            dfa[state_map[front - 1]][symbol - 1] = found;
        }
    }
}

void print_dfa() {
    printf("\nDFA Transition Table:\n");
    printf("State\t");
    for (int i = 0; i < nfa_symbol_count; i++) {
        printf("%d\t", i + 1);
    }
    printf("\n");
    
    for (int i = 0; i < dfa_state_count; i++) {
        printf("%d\t", i);
        for (int j = 0; j < nfa_symbol_count; j++) {
            printf("%d\t", dfa[i][j]);
        }
        printf("\n");
    }
}

int main() {
    printf("Enter the number of states in NFA: ");
    scanf("%d", &nfa_state_count);
    printf("Enter the number of input symbols: ");
    scanf("%d", &nfa_symbol_count);
    
    printf("Enter the NFA transition table (-1 for no transition):\n");
    for (int i = 0; i < nfa_state_count; i++) {
        for (int j = 0; j <= nfa_symbol_count; j++) {
            printf("From state %d using symbol %d: ", i, j);
            int state;
            while (scanf("%d", &state) && state != -1) {
                nfa[i][j][state] = 1;
            }
        }
    }
    
    convert_nfa_to_dfa();
    print_dfa();
    return 0;
}