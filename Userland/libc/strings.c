//
// Created by ptorl on 5/15/2023.
//


#include <strings.h>

int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }

    return (int)(*str1 - *str2);
}

int strncmp(const char *str1, const char * str2, size_t n){
    while(n-- && *str1 && *str2 && *str1 == *str2){
        str1++;
        str2++;
    }
    return (int)(*str1 - *str2);
}

size_t strlen(const char * str){
    size_t len = 0;
    while(str[len]) len++;
    return len;
}

char * strcpy(const char * src, char * dest){
    char * temp = dest;
    while ((*dest++ = *src++));
    return temp;
}

char * strcat(const char *src, char * dest){
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

    while(*last_ptr != '\0'){
        int i;
        for(i = 0; delim[i] != '\0'; i++){
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

