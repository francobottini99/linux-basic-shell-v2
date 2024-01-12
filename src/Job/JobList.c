/**
 * @file JobList.h
 * @author Bottini, Franco Nicolas.
 * @brief Implementacion de la lista de trabajos.
 * @version 1.5
 * @date Octubre de 2022.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../inc/Job/JobList.h"

job *first_job = NULL;

job* new_job()
{
    job* j = malloc(sizeof(job));

    j->id = 0;
    j->next = NULL;
    j->pgid = 0;
    j->first_process = NULL;

    return j;
}

process* new_process(char *command, char* infile, char* outfile)
{
    process *p = malloc(sizeof(process));

    p->next = NULL;
    p->argv = str_to_array(command, &p->argc);
    p->input_path = infile;
    p->output_path = outfile;
    p->status = STATUS_NEW;
    p->pid = -1;

    return p;
}

int insert_job(job *j,  EXECUTION_MODES mode) 
{
    job *last_job = get_last_job();

    j->mode = mode;
    
    if(!last_job)
    {
        j->id  = 1;
        first_job = j;
    }
    else
    {
        j->id = last_job->id + 1;
        last_job->next = j;
    }

    return j->id;
}

void insert_process(job *j, process *p)
{
    process *last_p = get_last_process(j);

    p->status = STATUS_READY;

    if(!last_p)
        j->first_process = p;
    else
        last_p->next = p;
}

void remove_job(job* j) 
{
    if (!first_job)
        return;

    job *parent = get_job_parent(j);

    if(!parent)
        first_job = j->next;
    else if(j->next == NULL)
        parent->next = NULL;
    else
        parent->next = j->next;
    
    free_job(j);
}

job* get_last_job() 
{
    job* last_j = first_job;

    if(!last_j)
        return NULL;
    
    while (last_j->next)
        last_j = last_j->next;

    return last_j;
}

job* get_job_by_id(int id_job)
{
    for (job* j = first_job; j; j = j->next)
        if (j->id == id_job)
            return j;
    
    return NULL;
}

job* get_job_by_pid(int pid)
{
    for (job* j = first_job; j; j = j->next) 
        for (process* p = j->first_process; p; p = p->next) 
            if (p->pid == pid) 
                return j;

    return NULL;
}

job* get_job_parent(job *j)
{
    job *parent = first_job;

    if(!parent || j == parent)
        return NULL;

    while (parent->next != j)
        parent = parent->next;
    
    return parent;
}

int is_job_completed(job *j) 
{
    for (process* p = j->first_process; p != NULL; p = p->next) 
        if (!is_process_completed(p))
            return 0;

    return 1;
}

int is_job_runnig(job *j) 
{
    for (process* p = j->first_process; p != NULL; p = p->next) 
        if (!is_process_running(p) && !is_process_completed(p))
            return 0;

    return 1;
}

int is_job_stoped(job *j) 
{
    for (process* p = j->first_process; p != NULL; p = p->next) 
        if (!is_process_stoped(p) && !is_process_completed(p))
            return 0;

    return 1;
}

int is_any_job_in_foreground()
{
    for (job* j = first_job; j; j = j->next)
        if (j->mode == FOREGROUND_EXECUTION)
            for (process* p = j->first_process; p; p = p->next)
                if(p->status == STATUS_RUNNING)
                    return 1;

    return 0;
}

process* get_last_process(job *j) 
{
    process* last_p = j->first_process;

    if(!last_p)
        return NULL;
    
    while (last_p->next)
        last_p = last_p->next;

    return last_p;
}

process* get_process_by_pid(int pid)
{
    job* j = get_job_by_pid(pid);

    if(!j)
        return NULL;

    process* p = j->first_process;

    if(!p)
        return NULL;

    while (p->pid != pid)
        p = p->next;

    return p;
}

int is_process_completed(process *p)
{
    return p->status == STATUS_DONE || p->status == STATUS_QUIT || p->status == STATUS_TERMINATED;
}

int is_process_running(process *p)
{
    return p->status == STATUS_RUNNING || p->status == STATUS_CONTINUED;
}

int is_process_stoped(process *p)
{
    return p->status == STATUS_SUSPENDED;
}

void set_process_status(process* p, PROCESS_STATUS status)
{
    p->status = status;
}

void free_job(job *j)
{
    process *aux;
    process *p = j->first_process;

    while(p)
    {
        free_array(p->argv, p->argc);
        free(p->input_path);
        free(p->output_path);
        aux = p;
        p = p->next;
        free(aux);
    }
    
    free(j);
}