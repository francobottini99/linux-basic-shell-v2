/**
 * @file JobList.h
 * @author Bottini, Franco Nicolas.
 * @brief Define funciones que permiten generar y administrar un listado de trabajos.
 * @version 1.5
 * @date Octubre de 2022.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __JOB_LIST_H__
#define __JOB_LIST_H__

#include <stdlib.h>
#include <string.h>

#include "../Utilities/Utilities.h"

/** Tipos de ejecuciones admitidas por los trabajos **/
typedef enum EXECUTION_MODES
{
    BACKGROUND_EXECUTION,   /** Ejecucion en segundo plano **/
    FOREGROUND_EXECUTION,   /** Ejecucion en primer plano **/
    PIPELINE_EXECUTION,     /** Ejecucion en una pipeline **/
    BADMODE_EXECUTION       /** No es posible ejecutar el trabajo **/
} EXECUTION_MODES;

/** Estados asignables a un proceso **/
typedef enum PROCESS_STATUS
{
    STATUS_RUNNING,     /** En ejecucion **/
    STATUS_DONE,        /** Finalizado **/
    STATUS_SUSPENDED,   /** Suspendido **/
    STATUS_CONTINUED,   /** Reanudado **/
    STATUS_NEW,         /** Nuevo proceso **/
    STATUS_TERMINATED,  /** Proceso terminado antes de completarse **/
    STATUS_QUIT,        /** Salgo del proceso antes de completarse **/
    STATUS_READY        /** Proceso agregado a un trabajo listo para correr **/
} PROCESS_STATUS;

/** Estructura de datos que define un proceso **/
typedef struct process 
{
    struct process *next;               /** Siguiente proceso en la lista **/
    int argc;                           /** Numero de argumentos para el proceso **/
    char **argv;                        /** Array de argumentos del proceso **/
    char *input_path, *output_path;     /** Paths de entrada y salida de los resultados**/
    pid_t pid;                          /** Process ID **/
    PROCESS_STATUS status;              /** Estado del proceso **/
} process;

/** Estructura de datos que define un trabajo **/
typedef struct job 
{
    struct job *next;               /** Siguiente trabajo en la lista **/
    int id;                         /** ID del trabajo **/
    struct process *first_process;  /** Primer proceso de la lista **/
    pid_t pgid;                     /** Process group ID **/
    EXECUTION_MODES mode;           /** Modo de ejecucion **/
    int io_fd[2], err_fd[2];        /** Pipes de comunicacion **/
} job;

extern job *first_job; /** Primer trabajo de la lista **/

/**
 * @brief Crea un nuevo trabajo.
 * 
 * @return job* Trabajo creado.
 */
job* new_job();

/**
 * @brief Crea un nuevo proceso.
 * 
 * @param command Comando de ejecucion para el nuevo proceso.
 * @param infile archivo de entrada del proceso.
 * @param outfile archivo de salida del proceso.
 * @return process* Proceso creado.
 */
process* new_process(char *command, char* infile, char* outfile);

/**
 * @brief Agrega un trabajo a la lista de trabajos.
 * 
 * @param j Trabajo a agregar en la lista.
 * @param mdoe Modo de ejecucion del trabajo.
 * @return int ID asignado al trabajo agregado.
 */
int insert_job(job *j, EXECUTION_MODES mode);

/**
 * @brief Agrega un proceso a la lista de procesos de un trabajo.
 * 
 * @param j Trabajo al cual agregarle el proceso.
 * @param p Proceso a agregar en la lista.
 */
void insert_process(job *j, process *p);

/**
 * @brief Elimina un trabajo de la lista y libera la memoria utilizada por el trabajo.
 * 
 * @param j Trabajo a eliminar del listado.
 */
void remove_job(job *j);

/**
 * @brief Obtiene el ultimo trabajo agregado a la lista.
 * 
 * @return job* Ultimo trabajo del listado. NULL en caso de que la lista este vacia.
 */
job* get_last_job();

/**
 * @brief Obtiene el trabajo al cual pertenece un proceso a partir de su Process ID.
 * 
 * @param pid Process ID del proceso a buscar dentro de los trabajos.
 * @return job* Trabajo al cual pertenece el proceso dado. NULL en caso de no existir ningun proceso con el PID dado.
 */
job* get_job_by_pid(int pid);

/**
 * @brief Obtiene un trabajo apartir de un ID.
 * 
 * @param id_job ID del trabajo que se quiere obtener.
 * @return job* Trabajo con el ID dadoo. NULL en caso de no existir ningun trabajo con el ID dado.
 */
job* get_job_by_id(int id_job);

/**
 * @brief Obtiene el trabajo padre de un trabajo.
 * 
 * @param j Trabajo del cual se quiere obtener el padre.
 * @return job* Trabajo padre del trabajo dado.
 */
job* get_job_parent(job *j);

/**
 * @brief Determina si un trabajo completo su ejecucion.
 * 
 * @param j Trabajo que se quiere consultar si esta terminado.
 * @return int 1 si el trabajo esta completo. 0 si el trabajo no esta completo.
 */
int is_job_completed(job *j);

/**
 * @brief Determina si el trabajo esta en ejecucion.
 * 
 * @param j Trabajo que se quiere consultar si esta en ejecucion.
 * @return int 1 si el trabajo esta en ejecucion. 0 si el trabajo no esta en ejecucion.
 */
int is_job_runnig(job* j);

/**
 * @brief Determina si el trabajo esta detenido.
 * 
 * @param j Trabajo que se quiere consultar si esta detenido.
 * @return int 1 si el trabajo esta detenido. 0 si el trabajo no esta detenido.
 */
int is_job_stoped(job* j);

/**
 * @brief Determina si hay algun trabajo corriendo en primer plano.
 * 
 * @return int 1 si hay un trabajo corriendo en primel plano. 0 si no lo hay.
 */
int is_any_job_in_foreground();

/**
 * @brief Obtiene el ultimo proceso del listado de procesos de un trabajo.
 * 
 * @param j Trabajo del cual se quiere obtener el ultimo proceso de su listado.
 * @return process* Ultimo proceso del listado de procesos del trabajo.
 */
process* get_last_process(job *j);

/**
 * @brief Obtiene un proceso a partir de su Process ID.
 * 
 * @param pid Process ID del proceso que se quiere obtener.
 * @return process* Proceso solicitado. NULL en caso de no existir ningun proceso con el PID dado.
 */
process* get_process_by_pid(int pid);

/**
 * @brief Determina si un proceso esta completo.
 * 
 * @param p Proceso que se quiere consultar si esta completo.
 * @return int 1 si el proceso esta completo. 0 si el proces no esta completo.
 */
int is_process_completed(process *p);

/**
 * @brief Determina si un proceso esta en ejecucion.
 * 
 * @param p Proceso que se quiere consultar si esta en ejecucion.
 * @return int 1 si el proceso esta en ejecucion. 0 si el proces no esta en ejecucion.
 */
int is_process_running(process *p);

/**
 * @brief Determina si un proceso esta detenido.
 * 
 * @param p Proceso que se quiere consultar si esta detenido.
 * @return int 1 si el proceso esta detenido. 0 si el proces no esta detenido.
 */
int is_process_stoped(process *p);

/**
 * @brief Establece el estado de un proceso.
 * 
 * @param p Proceso al cual establecer el estado.
 * @param status  Estado a establecer.
 */
void set_process_status(process* p, PROCESS_STATUS status);

/**
 * @brief Libera la memoria alocada por un trabajo.
 * 
 * @param j Trabajo a liberar.
 */
void free_job(job *j);

#endif //__JOB_LIST_H__