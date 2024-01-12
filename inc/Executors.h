/**
 * @file Executors.h
 * @author Bottini, Franco Nicolas.
 * @brief Define las funciones ejecutoras de los comandos de MyShell.
 * @version 1.5
 * @date Octubre de 2022.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __EXECUTORS_H__
#define __EXECUTORS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "Job/JobControl.h"
#include "Utilities/Utilities.h"

/**
 * @brief Cambia de directorio de trabajo.
 * 
 * @param args Argumentos de ejecucion del comando.
 */
void execute_cd(char* args);

/**
 * @brief Envia un mensaje o el valor de una variable de entorno a la terminal.
 * 
 * @param args Argumentos de ejecucion del comando.
 */
void execute_echo(char* args);

/**
 * @brief Muestra el estado de los trabajos lanzados.
 * 
 * @param args Argumentos de ejecucion del comando.
 * @param print_job_all_status Puntero a la funcion que imprime los estados por consola.
 */
void execute_jobs(char* args);

/**
 * @brief Finaliza un trabajo o un proceso.
 * 
 * @param args Argumentos de ejecucion del comando.
 */
void execute_kill(char* args);

/**
 * @brief Reanuda la ejecucion de un trabajo detenido en primer plano.
 * 
 * @param args Argumentos de ejecucion del comando.
 */
void execute_fg(char* args);

/**
 * @brief Finaliza la ejecucion del programa.
 *
 * @param args Argumentos de ejecucion del comando.
 */
void execute_quit(char* args);

/**
 * @brief Limpia la terminal.
 * 
 * @param args Argumentos de ejecucion del comando.
 */
void execute_clr(char* args);

/**
 * @brief Ejectura un comando externo al programa en un nuevo proceso.
 * 
 * @param args Argumentos de ejecucion del comando.
 */
void execute_extern(char* args);

/**
 * @brief Obtiene el modo de ejecucion en que se debe ejecutar un trabajo de acuerdo a partir de los argumentos dados.
 * 
 * @param args Argumentos de ejecucion para un comando.
 * @return EXECUTION_MODES Modo en el que se debe ejecutar. En caso de una combinacion invalida de ejecucion retorna BADMODE_EXECUTION.
 */
EXECUTION_MODES get_execution_mode(char* args);

#endif //__EXECUTORS_H__