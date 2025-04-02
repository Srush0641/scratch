#include<stdio.h>
#include<string.h>
#include <ctype.h>

#define MAX_LEN 100
int iskeyword(char *str){
    char keywords [][10] = {"if" , "else" , "int" , "float", "return" , "while"};

    for(int i=0;i<6;i++){
        if(strcmp(str ,keywords[i])==0) return 1;
    }
    return 0;
}
void lexer(char *input){
    char buffer[50];
    int i=0,j=0;

    while(input[i] != '\0'){
        if(isalpha(input[i])){
            j=0;
            while(isalnum(input[i])) buffer[j++] = input[i++];
            buffer[j] = '\0';

            if(iskeyword(buffer)){
                printf("Keyword: %s\n",buffer);
            }
            else{
                printf("IdENtifier: %s\n",buffer);
            }
        }
        else if(isdigit(input[i])){
            j=0;
            while(isdigit(input[i])) buffer[j++] = input[i++];
            buffer[j] = '\0';
            printf("NUmber: %s\n",buffer);
        }
        else if(isspace(input[i])){
            i++;
        }
        else{
            printf("Opertor :%c\n",input[i]);
            i++;
        }
    }
}

int main(){
    char input[MAX_LEN];

    printf("Enter a statement:");
    fgets(input , MAX_LEN , stdin);

    input[strcspn(input,"\n")] = '\0';

    lexer(input);

    return 0;
}