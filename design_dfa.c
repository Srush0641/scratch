#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100

int dfa(char *input){
    int state=0;

    for(int i=0; input[i] != '\0' ;i++){
        char ch = input[i];

        switch(state){
            case 0:
                if(ch == 'a') state = 1;
                break;
            case 1:
                if(ch == 'a') state = 1;
                else if(ch == 'b') state =2;
                else state=0;
                break;
            case 2:
                if(ch == 'a') state = 1;
                else if(ch == 'b') state = 3;
                else state = 0;
                break;
            case 3:
                return 1;
        }
        
    }
    return (state == 3);
    
}
int main(){
    char input[MAX_LEN];

    printf("Enter s string:");
    scanf("%s",input);

    if(dfa(input)){
        printf("Accepted:\n");
    }
    else{
        printf("Rejected:\n");
    }
    return 0;
}