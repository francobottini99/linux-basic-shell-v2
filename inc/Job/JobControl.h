/**
 * @file JobControl.h
 * @author Bottini, Franco Nicolas.
 * @brief Define funciones que permiten implementar un control de trabajos simple.
 * @version 1.5
 * @date Octubre de 2022.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __JOB_CONTROL_H__
#define __JOB_CONTROL_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "JobList.h"
#include "../Utilities/Utilities.h"

/** Filtros admitidos para la busqueda de procesos **/
typedef enum PROCESS_FILTERS
{
    PROC_FILTER_ALL,        /** Todos los procesos **/
    PROC_FILTER_DONE,       /** Procesos finalizados **/
    PROC_FILTER_REMAINING   /** Procesos activos **/
} PROCESS_FILTERS;

typedef enum SOURCE_CLEAN
{
    SOURCE_HANDLER,
    SOURCE_FOREGROUND_EXECUTION,
    SOURCE_REANUDE_FG,
    SOURCE_KILL
} SOURCE_CLEAN;

extern const char* PROCESS_STATUS_STRING[]; /** String-array de los estados de un proceso **/

extern int flag_work_tube_printed; /** Se pone en uno cuando se imprime el contenido de una pipe por la terminal **/

/**
 * @brief Inicializa el control de trabajos.
 * 
 */
void job_control_init(void);

/**
 * @brief Se ejecuta al recibir la señal SIGCHLD.
 * 
 */
void childend_handler();

/**
 * @brief Remueve de la lista los trabajos completados.
 * 
 * @param by_signal 1 si la llamada a la funcion se realizo desde la interrupcion de una señal. 0 en caso contrario.
 */
void clean_done_job(SOURCE_CLEAN source);

/**
 * @brief Actualiza el estado de un proceso.
 * 
 * @param p Proceso al cual actualizar su estado.
 * @param status Nuevo estado del proceso.
 */
void update_process_status(process *p, PROCESS_STATUS status);

/**
 * @brief Espera a la finalizacion de todos los procesos de un trabajo.
 * 
 * @param j Trabajo al cual se debe esperar.
 */
void wait_for_job(job *j, int catch_stoped);

/**
 * @brief Ejecuta todos los procesos de un trabajo.
 * 
 * @param j Trabajo a ejecutar.
 * @param mode Modo de ejecucion del trabajo.
 * @return int Estadado del trbajo ejecutado.
 */
void launch_job(job *j, EXECUTION_MODES mode);

/**
 * @brief Ejecuta un nuevo proceso.
 * 
 * @param j Trabajo al cual pertenece el proceso.
 * @param p Proceso a ejecutar.
 * @return int Estado del proceso ejecutado.
 */
void launch_process(process *p, pid_t pgid, int in_fd, int out_fd, int err_fd, EXECUTION_MODES mode);

/**
 * @brief Reaunuda en primer plano la ejecucion de un trabajo detenido identificado por su ID.
 * 
 * @param id_job ID del trabajo que se quiere reaunudar.
 */
void reanude_job_fg(int id_job);

/**
 * @brief Fuerza la finalizacion de todos los trabajos del listado.
 * 
 */
void kill_all_jobs();

/**
 * @brief Fuerza la finalizacion de un trabajo identificado por su ID.
 * 
 * @param id_job ID del trabajo que se quiere finalizar.
 */
void kill_job(int id_job);

/**
 * @brief Imprime por consola el estado de todos los trabajos del listado.
 * 
 */
void print_job_all_status();

/**
 * @brief Imprime por consola el estado de un trabajo dado.
 * 
 * @param j Trabajo del que se quiere obtener el estado.
 */
void print_job_status(job *j);

/**
 * @brief Imprime por consola los procesos en un trabajo dado.
 * 
 * @param j Trabajo del que se obtienen los procesos.
 */
void print_job_process(job *j);

/**
 * @brief Imprime por consola el contenido de la pipeline de un trabajo.
 * 
 * @param j Trabajo del cual queremos imprimir la pipeline.
 */
void print_job_pipe(job *j);

#endif //__JOB_CONTROL_H__