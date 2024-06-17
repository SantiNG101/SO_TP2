//
// Created by ptorl on 5/15/2023.
//

#ifndef KERNEL_STRINGS_H
#define KERNEL_STRINGS_H

#include <stddef.h>

//section define
// #define NULL ((void*)0)
// typedef unsigned int size_t;
// end section define

/*
 * retorna un size_t (unsigned int) con la cantidad de caracteres hasta el '\0'
 */
size_t strlen(const char * str);
/*
 * Igual que en la lib std, NO verifica que el input sea nulo, por lo tanto en caso de que lo sea
 * su comportamiento NO esta definido.
 *  retorna < 0 si s1 < s2
 *  retorna 0 si s1 = s2
 *  retorna > 0 si s1 > s2
 */
int strcmp(const char * str1, const char * str2);
/* Idem strcmp, solo que a lo sumo compara hasta los primeros n caracteres (n no incluido)
 * Se considera que n es un numero entero positivo. Si n = 0, compara el primer caracter de str1 y str2.
 * Mismos valores de retorno que strcmp
 */
int strncmp(const char *str1, const char *str2, size_t n);
/*
 * separa el string en tokens utilizando el delimitador como separador de tokens
 * retorna un puntero al primer token, retorna nulo si no hay tokens por hacer
 */
char * strtok(char * str, const char * delim);
/*
 * retorna un puntero a dest si logra copiarlo
 * retorna NULL si src o dest es nulo
 */
char * strcpy(const char * src, char * dest);
/*
 * retorna NULL si dest o src son NULL
 * retorna un puntero a dest si se copia exitosamente
 */
char * strcat(const char * src, char * dest);


#endif //KERNEL_STRINGS_H
