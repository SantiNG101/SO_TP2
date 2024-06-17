#include "include/strings.h"

int strcmp(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] && (str1[i] == str2[i])) i++;

    return (int)(str1[i] - str2[i]);
}

int strncmp(const char *str1, const char * str2, size_t n){
    int i = 0;
    while(n-- && str1[i] && str2[i] && str1[i] == str2[i]) i++;
    return (int)(str1[i] - str2[i]);
}

size_t strlen(const char * str){
    size_t len = 0;
    while(str[len]) len++;
    return len;
}

char * strcpy(const char * src, char * dest){
    int i = 0;
    while ((dest[i] = src[i])) i++;
    
    return dest;
}

char * strcat(const char * src, char * dest){
    size_t dest_len = strlen(dest);
    int i = 0;
    while(src[i] != '\0'){
        dest[dest_len++] = src[i++];
    }
        dest[dest_len] = '\0';
        return dest;
}

char * strtok(char * str, const char * delim){
    static char * last_ptr = NULL;

    //si el str es distinto de nulo => le estoy pasando un nuevo string para tokenizar
    if(str != NULL){
        last_ptr = str;
    }

    if(last_ptr == NULL || *last_ptr == '\0'){
        return NULL;
    }

    char * token = last_ptr;
    while(*last_ptr){
        int i;
        for(i = 0; delim[i]; i++){
            if (*last_ptr == delim[i]) {
                *last_ptr = '\0';
                last_ptr++;
                return token;
            }
        }
        last_ptr++;
    }
    return token;
}

