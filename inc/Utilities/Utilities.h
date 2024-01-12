/**
 * @file Utilities.h
 * @author Bottini, Franco Nicolas.
 * @brief Define funciones y caracteristicas de proposito general.
 * @version 1.5
 * @date Octubre de 2022.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <stdlib.h>
#include <string.h>

/** Define los codigos para cambiar el color del texto en la terminal **/
#ifndef TERMINAL_TEXT_COLORS
#define TERMINAL_TEXT_COLORS
    #define KDEF  "\x1B[0m"     /** Por defecto **/
    #define KRED  "\x1B[31m"    /** Rojo **/
    #define KGRN  "\x1B[32m"    /** Verde **/
    #define KYEL  "\x1B[33m"    /** Amarillo **/
    #define KBLU  "\x1B[34m"    /** Azul **/
    #define KMAG  "\x1B[35m"    /** Magenta **/
    #define KCYN  "\x1B[36m"    /** Cyan **/
    #define KWHT  "\x1B[37m"    /** Blanco **/
#endif

/** Define distintos caracteres ASCII para hacer mas legible el codigo **/
#ifndef ASCII_CHARACTERS
#define ASCII_CHARACTERS
    #define ASCII_END_OF_STRING 0       /** '\0' **/
    #define ASCII_MONEY_SIGN    36      /** '$'  **/
    #define ASCII_MIDDLE_DASH   45      /** '-'  **/
    #define ASCII_SPACE         32      /** ' '  **/
    #define ASCII_LINE_BREAK    10      /** '\n' **/
    #define ASCII_AMPERSAND     38      /** '&'  **/
    #define ASCII_PLECA         124     /** '|'  **/
    #define ASCII_PERCENT       37      /** '%'  **/
    #define ASCII_GREATER_THAN  62      /** '>'  **/
    #define ASCII_LESS_THAN     60      /** '<'  **/
#endif

/**
 * @brief Obtiene la subcadena previa a la aparicion de cualquiera de los dos caracteres dados.
 * 
 * @param str Cadena sobre la cual operar.
 * @param c1 Caracter de corte 1.
 * @param c2 Caracter de corte 2.
 * @return char* Sub cadena resultante.
 */
char* get_substr_before_chars(char* str, char c1, char c2);

/**
 * @brief Obtiene la palabra que se encuentra a continuacion de la primera aparicion del caracter dado.
 * 
 * @param str Cadena sobre la cual operar.
 * @param character Caracter luego del cual obtener la palabra.
 * @return char* Palabra obtenida. NULL en caso de que el caracter dado no se encuentre en la cadena.
 */
char* get_word_after_char(char* str, char character);

/**
 * @brief Elimina los espacios en blanco al comienzo y final de una cadena.
 * 
 * @param str Cadena sobre la cual operar.
 * @return char* Puntero a la cadena resultante.
 */
char* trim_white_space(char* str);

/**
 * @brief Genera un array bidimensional segmentando una cadena en sus espacios. Se agrega NULL como ultimo elemento del array.
 * 
 * @param str Cadena a segmentar en formato de array.
 * @param n Puntero a donde se va a almacenar el numero de elementos del array.
 * @return char** Puntero al array bidimensional resultante.
 */
char** str_to_array(char* str, int* n);

/**
 * @brief Libera la memoria alocada por un array bidimensional.
 * 
 * @param array Arrray a liberar.
 * @param n numero de lementos del array.
 */
void free_array(char** array, int n);

#endif //__UTILITIES_H__