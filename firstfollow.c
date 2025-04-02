#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_RULES 100
#define MAX_SYMBOL 100

char nonTerminal[MAX_SYMBOL];
char rhs[MAX_RULES][MAX_SYMBOL][MAX_RULES];
char first[MAX_SYMBOL][MAX_SYMBOL];
char followsets[MAX_SYMBOL][MAX_SYMBOL];
int numRules;

int isnonTerminal(char symbol) {
    return (symbol >= 'A' && symbol <= 'Z');
}

void findFirst(int num, int n) {
    if(first[num][0] != '\0') return;

    int first_ite = 0;
    for(int idx = 0; rhs[num][idx][0] != '\0'; idx++) {
        char *prod = rhs[num][idx];
        int prodsz = strlen(prod);

        if(!(prod[0] >= 'A' && prod[0] <= 'Z')) first[num][first_ite++] = prod[0];
        else {
            int req_idx = 0;
            for(int i = 0; i < n; i++) if(nonTerminal[i] == prod[0]) req_idx = i;
            findFirst(req_idx, n);

            int ti = 0;
            while(first[req_idx][ti] != '\0') {
                first[num][first_ite++] = first[req_idx][ti++];
            }
        }
    }

    return;
}

void addFollow(int index, char symbol) {
    if (strchr(followsets[index], symbol)) return;
    int len = strlen(followsets[index]);
    followsets[index][len] = symbol;
    followsets[index][len + 1] = '\0';
}

void findFollow() {
    addFollow(0, '$'); 

    int change = 1;
    while (change) {
        change = 0;
        for (int i = 0; i < numRules; i++) {
            char lhs = nonTerminal[i];

            for (int j = 0; rhs[i][j][0] != '\0'; j++) {
                char *production = rhs[i][j];
                int length=strlen(production);

                for (int k = 0; production[k] != '\0'; k++) {
                    char curr = production[k];

                    if (isnonTerminal(curr)) {
                        int curridx = -1;

                        for (int m = 0; m < numRules; m++) {
                            if (nonTerminal[m] == curr) {
                                curridx = m;
                                break;
                            }
                        }

                        if(curridx<0) break;
                        
                        int posn=k+1;
                        bool alleps=true;

                        while(posn<length && alleps){
                            alleps=false;

                            char next=production[posn];

                            if(!isnonTerminal(next)){
                                if(!strchr(followsets[curridx],next)){
                                    addFollow(curridx,next);
                                    change=true;
                                }
                                break;
                            }
                            else{
                                int nextidx=-1;
                                for(int m=0;m<numRules;m++){
                                    if(nonTerminal[m]==next){
                                        nextidx=m;
                                        break;
                                    }
                                }

                                if(nextidx<0) break;
                                bool hasepsilon=false;
                                for(int m=0;first[nextidx][m]!='\0';m++){
                                    if( first[nextidx][m]!='e' && !strchr(followsets[curridx],first[nextidx][m])){
                                        addFollow(curridx,first[nextidx][m]);
                                        change=true;
                                    }else{
                                        hasepsilon=true;
                                    }
                                }
                                if(hasepsilon){
                                    alleps=true;
                                }
                            }
                            posn++;
                        }
                        if(posn >= length && alleps) {
                            for(int m = 0; followsets[i][m] != '\0'; m++){
                                if(!strchr(followsets[curridx], followsets[i][m])) {
                                    addFollow(curridx, followsets[i][m]);
                                    change = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void printFirstFollow() {
    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < numRules; i++) {
        printf("FOLLOW(%c) = { ", nonTerminal[i]);
        for (int j = 0; followsets[i][j] != '\0'; j++) {
            printf("%c ", followsets[i][j]);
        }
        printf("}\n");
    }
}

int main() {
    printf("Enter number of rules: ");
    scanf("%d", &numRules);
    getchar();

    for (int i = 0; i < numRules; i++) {
        char input[MAX_SYMBOL * 2];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        nonTerminal[i] = input[0];
        int i1 = 0, i2 = 0;
        for (int j = 3; input[j] != '\0'; j++) {
            if (input[j] == '|') {
                rhs[i][i1][i2] = '\0';
                i1++;
                i2 = 0;
            } else {
                rhs[i][i1][i2++] = input[j];
            }
        }
        rhs[i][i1][i2] = '\0';
    }

    for (int i = 0; i < numRules; i++) {
        findFirst(i, numRules);
    }

    printf("FIRST sets:\n");
    for (int i = 0; i < numRules; i++) {
        printf("FIRST(%c) = { ", nonTerminal[i]);
        for (int j = 0; first[i][j] != '\0'; j++) {
            printf("%c ", first[i][j]);
        }
        printf("}\n");
    }

    findFollow();

    printFirstFollow();

    return 0;
}