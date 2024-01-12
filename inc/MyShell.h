/**
 * @file MyShell.h
 * @author Bottini, Franco Nicolas.
 * @brief Define las caracteristicas de una pequeña aplicacion de shell. 
 *        MyShell hace uso de un mecanismo simple de control de trabajos e
 *        Implementa un intérprete de línea de comandos simple al estilo Bourne shell.
 * @version 1.5
 * @date Octubre de 2022.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MYSHELL_H__
#define __MYSHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "Executors.h"
#include "Utilities/Utilities.h"

/** Longitud maxima de las entradas que admtide el programa **/
#define MAX_LEN_INPUT 256 

/** Macro para calcular el tamaño de un const array **/
#define CONST_STR_ARR_SIZE(arr) sizeof(arr) / sizeof(*arr)

/** Posibles resultados al leer las entradas de comandos **/
typedef enum READ_INPUT_RESULT
{
    INP_NULL = -3,      /** Equivalente a NULL **/
    INP_TO_LONG = -2,   /** La entrada supera la longitud maxima admitida **/
    INP_END = -1,       /** No hay entradas que leer **/
    INP_EMPTY_LINE = 0, /** Lectura de una linea vacia **/
    INP_READ = 1        /** Lectura exitosa de una entrada **/
} READ_INPUT_RESULT;

/** Flags de los comandos admitidos **/
typedef enum COMMANDS_FLAGS
{
    CMM_EXTERN = -1,    /** Comando externo **/
    CMM_QUIT = 0,       /** Comando quit **/
    CMM_CD = 1,         /** Comando cd **/
    CMM_CLR = 2,        /** Comando clear **/
    CMM_ECHO = 3,       /** Comando echo **/
    CMM_JOBS = 4,       /** Comando jobs **/
    CMM_KILL = 5,       /** Comando kill **/
    CMM_FG = 6          /** Comando fg **/
} COMMANDS_FLAGS;

/** Array de los comandos admitidos **/
const char* CMM_VALIDS[] = {
    "quit",
    "cd",
    "clr",
    "echo",
    "jobs",
    "kill",
    "fg"
};

/**
 * @brief Imprime el mensaje de bienvenida al progama.
 * 
 */
void print_welcome();

/**
 * @brief Valida que el numero de parametros introducido al ejecutar el programa sea valido.
 * 
 * @param argc Numero de argumentos de entrada.
 */
void myshell_validate_execution(int argc);

/**
 * @brief Ejecuta el loop principal de la shell de manera indefinida.
 * 
 * @param input_source Archivo desde el cual se van a tomar las entradas de comandos.
 */
void myshell_loop(FILE* input_source);

/**
 * @brief Imprime por consola el prompt.
 * 
 */
void print_prompt(void);

/**
 * @brief Dependiendo los parametros dados en la ejecucion del programa, obtiene el archivo fuente desde donde se van a leer los comandos entrantes.
 * 
 * @param argc Numero de argumentos de entrada.
 * @param argv Array de argumentos de entrada.
 * @return FILE* Puntero al archivo desde el cual se van a tomar los comandos entrantes.
 */
FILE* command_source(int argc, char* argv[]);

/**
 * @brief Interpreta y ejecuta un comando dado.
 * 
 * @param input cadena que se debe interpretar y ejecutar.
 */
void input_decode(char* input);

/**
 * @brief Ejecuta un comando a partir de su identificador y sus argumentos. 
 * 
 * @param cmm Identificador o flag del comando a ejecutar.
 * @param args Cadena con los argumentos necesarios para la ejecucion del comando.
 */
void command_handler(COMMANDS_FLAGS cmm, char* args);

/**
 * @brief Get the Input object
 * 
 * @param buffer Puntero al buffer donde se quiere almacenar la lectura.
 * @param buffer_size Tamaño del buffer donde se quiere almacenar la lectura.
 * @param fp Puntero al archivo desde donde se debe realizar la lectura.
 * @return READ_INPUT_RESULT Codigo que indica el resultado de la operacion.
 */
READ_INPUT_RESULT get_input(char* buffer, int buffer_size, FILE* fp);

#endif //__MYSHELL_H__