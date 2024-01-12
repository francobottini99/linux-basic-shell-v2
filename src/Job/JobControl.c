/**
 * @file JobControl.h
 * @author Bottini, Franco Nicolas.
 * @brief Implementacion del control de trabajos simple.
 * @version 1.5
 * @date Octubre de 2022.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../inc/Job/JobControl.h"

const char* PROCESS_STATUS_STRING[] = {
    "running",
    "done",
    "suspended",
    "continued",
    "new",
    "terminated",
    "quit",  
    "ready"
};

int flag_work_tube_printed = 0;

void job_control_init()
{
    pid_t shell_pgid;

    while (tcgetpgrp(STDIN_FILENO) != (shell_pgid = getpgrp()))
        kill (- shell_pgid, SIGTTIN);

    struct sigaction sigint_action = {
        .sa_handler = childend_handler,
        .sa_flags = 0
    };
    sigemptyset(&sigint_action.sa_mask);
    sigaction(SIGCHLD, &sigint_action, NULL);

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    shell_pgid = getpid ();
    if (setpgid(shell_pgid, shell_pgid) < 0)
    {
        perror(KRED"\nCouldn't put the shell in its own process group\n"KDEF);
        exit(EXIT_FAILURE);
    }

    tcsetpgrp(STDIN_FILENO, shell_pgid);
}

void childend_handler()
{
    int status;
    pid_t pid;
    process *p;

    while ((pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) 
    {
        p = get_process_by_pid(pid);

        if(!p)
        {
            fprintf (stderr, KRED"\nNo child process %d !\n"KDEF, pid);
            break;
        }

        update_process_status(p, status);
    }

    if(pid < 0 && errno != ECHILD)
        fprintf(stderr, KRED"\nwaitpid: %s !\n\n"KDEF, strerror(errno));
    
    if(!is_any_job_in_foreground())
        clean_done_job(SOURCE_HANDLER);
}

void clean_done_job(SOURCE_CLEAN source)
{
    job* j = first_job;
    job* aux;

    while (j)
    {
        aux = j;
        j = j->next;

        if (is_job_completed(aux))
        {
            if(aux->mode == BACKGROUND_EXECUTION || source == SOURCE_KILL || source == SOURCE_REANUDE_FG)
            {
                if(source == SOURCE_HANDLER)
                    fprintf(stdout, "\n\n");
                else
                    fprintf(stdout, "\n");

                print_job_status(aux);

                if(source == SOURCE_HANDLER || source == SOURCE_KILL || source == SOURCE_REANUDE_FG)
                    fprintf(stdout, "\n");
            }
            
            print_job_pipe(aux);  
            remove_job(aux);
        }
    }
}

void update_process_status(process *p, PROCESS_STATUS status)
{
    job* j = get_job_by_pid(p->pid);

    if (WIFSTOPPED(status))
    {
        set_process_status(p, STATUS_SUSPENDED);

        if(is_job_stoped(j))
        {
            fprintf(stdout, "\n\n");
            print_job_status(j);
            fprintf(stdout, "\n\n");
        }
    }
    else if (WIFCONTINUED(status))
    {
        set_process_status(p, STATUS_CONTINUED);
        
        if(is_job_runnig(j))
        {
            fprintf(stdout, "\n");
            print_job_status(j);
            fprintf(stdout, "\n");
        }
    }
    else if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGINT)
        {
            set_process_status(p, STATUS_TERMINATED);
            
            fprintf(stdout, "\n");
        }
        else if (WTERMSIG(status) == SIGQUIT)
        {
            set_process_status(p, STATUS_QUIT);

            if(is_job_completed(j))
            {
                fprintf(stdout, "\n\n");
                print_job_status(j);
                fprintf(stdout, "\n");
            }
        }
        else if (WTERMSIG(status) == SIGKILL)
            set_process_status(p, STATUS_TERMINATED);
    }
    else if (WIFEXITED(status))
        set_process_status(p, STATUS_DONE);
}

void wait_for_job(job *j, int catch_stoped)
{
    int end_while = 0;
    int status;
    pid_t pid;
    process *p;

    do
    {
        if((pid = waitpid(-j->pgid, &status, WUNTRACED)) < 1)
        {
            if(errno == EINTR)
                continue;
            else
                break;
        }

        p = get_process_by_pid(pid);

        if(!p)
        {
            fprintf (stderr, KRED"\nNo child process %d !\n"KDEF, pid);
            break;
        }
    
        update_process_status(p, status);

        if(catch_stoped)
            end_while = is_job_stoped(j);
        else
            end_while = is_job_completed(j);
    } while(!end_while);

    if(pid < 0 && errno != ECHILD)
        fprintf(stderr, KRED"\nwaitpid: %s !\n\n"KDEF, strerror(errno));
}

void launch_job(job *j, EXECUTION_MODES mode) 
{
    int proc_io_pipe[2];
    int infile, outfile;

    insert_job(j, mode);

    if (pipe(j->io_fd) < 0 || pipe(j->err_fd) < 0)
    {
        perror(KRED"\npipe\n"KDEF);
        exit(EXIT_FAILURE);
    }

    if(!j->first_process->input_path)
    {
        if(mode == BACKGROUND_EXECUTION)
            infile = j->io_fd[0];
        else
            infile = STDIN_FILENO;
    }
    else
        infile = open(j->first_process->input_path, O_RDONLY);

    for (process *p = j->first_process; p; p = p->next)
    {
        if(infile < 0)
        {
            fprintf(stderr, KRED"\nNo such file or directory: %s !\n"KDEF, p->input_path);
            set_process_status(p, STATUS_TERMINATED);
            continue;
        }

        if(p->output_path && *p->output_path == ASCII_LINE_BREAK)
        {
            fprintf(stderr, KRED"\nParse error near '\\n' !\n"KDEF);
            set_process_status(p, STATUS_TERMINATED);
            continue;   
        }

        if(!p->output_path)
        {
            if (p->next)
            {
                if (pipe(proc_io_pipe) < 0)
                {
                    perror(KRED"\npipe\n"KDEF);
                    exit (EXIT_FAILURE);
                }

                outfile = proc_io_pipe[1];
            }
            else
            {
                if(mode == BACKGROUND_EXECUTION)
                    outfile = j->io_fd[1];
                else
                    outfile = STDOUT_FILENO;
            }
        }
        else
            outfile = open(p->output_path, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
        
        set_process_status(p, STATUS_RUNNING);

        pid_t pid = fork ();
        if (pid == 0)
        {
            if (p->next)
                launch_process(p, j->pgid, infile, outfile, j->err_fd[1], PIPELINE_EXECUTION);
            else
                launch_process(p, j->pgid, infile, outfile, j->err_fd[1], j->mode);
        }    
        else if (pid < 0)
        {
            perror(KRED"\nfork\n"KDEF);
            exit (EXIT_FAILURE);
        }
        else
        {
            p->pid = pid;
            
            if (!j->pgid)
                j->pgid = pid;
            
            setpgid (pid, j->pgid);
        }

        if (infile != STDIN_FILENO && infile != j->io_fd[0])
            close(infile);

        if (outfile != STDOUT_FILENO && outfile != j->io_fd[1])
            close(outfile);
        
        infile = proc_io_pipe[0];

        if(p->next)
            if(p->next->input_path)
                infile = open(p->next->input_path, O_RDONLY);
    }

    if (j->mode == FOREGROUND_EXECUTION)
    {
        tcsetpgrp(STDIN_FILENO, j->pgid);
        wait_for_job(j, 1);
        tcsetpgrp(STDIN_FILENO, getpid());

        clean_done_job(SOURCE_FOREGROUND_EXECUTION);
    }
    else if (j->mode == BACKGROUND_EXECUTION)
        print_job_process(j);
}

void launch_process(process *p, pid_t pgid, int in_fd, int out_fd, int err_fd, EXECUTION_MODES mode) 
{
    pid_t childpid = getpid ();

    if (pgid == 0)
        pgid = childpid;
    
    setpgid(childpid, pgid);

    if (mode == FOREGROUND_EXECUTION)
    {
        fprintf(stdout, KYEL"\n");
        tcsetpgrp(STDIN_FILENO, pgid);
    }

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    
    if (in_fd != STDIN_FILENO)
    {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    
    if (out_fd != STDOUT_FILENO)
    {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }

    if (err_fd != STDERR_FILENO)
    {
        dup2(err_fd, STDERR_FILENO);
        close(err_fd);
    }

    if (execvp(p->argv[0], p->argv) < 0) 
    {
        fprintf(stderr, "Command not found!\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}

void reanude_job_fg(int id_job)
{
    job* j = get_job_by_id(id_job);

    if (!j)
        fprintf(stderr, KRED"\nJob not found!\n\n"KDEF);
    else
    {
        if (kill(-j->pgid, SIGCONT) < 0)
            fprintf(stderr, KRED"\nJob not found!\n\n"KDEF);
        else
        {
            tcsetpgrp(STDIN_FILENO, j->pgid);
            wait_for_job(j, 1);
            tcsetpgrp(STDIN_FILENO, getpid());

            clean_done_job(SOURCE_REANUDE_FG);
        }
    }
}

void kill_all_jobs()
{
    job *j = first_job;
    job *aux;

    while (j)
    {
        aux = j;
        j = j->next;
        kill_job(aux->id);
    }
}

void kill_job(int id_job)
{
    job* j = get_job_by_id(id_job);

    if (!j)
        fprintf(stderr, KRED"\nJob not found!\n\n"KDEF);
    else
    {
        if (kill(-j->pgid, SIGKILL) < 0)
            fprintf(stderr, KRED"\nJob not found!\n\n"KDEF);
        else
        {
            wait_for_job(j, 0);
            clean_done_job(SOURCE_KILL);
        }
    }
}

void print_job_all_status(void)
{
    fprintf(stdout, "\n");

    for (job* j = first_job; j; j = j->next)
    {
        print_job_status(j);
        fprintf(stdout, "\n");
    }

    fprintf(stdout, "\n");
}

void print_job_status(job *j) 
{
    fprintf(stdout, KBLU"[%d]"KDEF, j->id);

    for (process* p = j->first_process; p; p = p->next) {
        fprintf(stdout, KBLU" %d %s %s"KDEF, p->pid, PROCESS_STATUS_STRING[p->status], p->argv[0]);

        if (p->next)
            fprintf(stdout, KBLU" |\n   "KDEF); 
    }
}

void print_job_process(job *j) 
{
    fprintf(stdout, KBLU"\n[%d]"KDEF, j->id);

    for (process* p = j->first_process; p; p = p->next)
    {
        fprintf(stdout, KBLU" %d %s"KDEF, p->pid, p->argv[0]);

        if(p->next)
            fprintf(stdout, KBLU" |\n   "KDEF);
    }

    fprintf(stdout, "\n\n");
}

void print_job_pipe(job *j)
{
    close(j->io_fd[1]);
    close(j->err_fd[1]);

    fcntl(j->io_fd[0], F_SETFL, fcntl(j->io_fd[0], F_GETFL) | O_NONBLOCK);
    fcntl(j->err_fd[0], F_SETFL, fcntl(j->err_fd[0], F_GETFL) | O_NONBLOCK);

    fprintf(stdout, "\n");

    char c; int w = 0;
    while (read(j->err_fd[0], &c, sizeof(c)) > 0)
    {
        fprintf(stdout, KRED"%c"KDEF, c);
        w = 1;
    }
           
    while (read(j->io_fd[0], &c, sizeof(c)) > 0)
    {
        fprintf(stdout, KYEL"%c"KDEF, c);
        w = 1;
    }

    close(j->io_fd[0]);
    close(j->err_fd[0]);

    flag_work_tube_printed = 1;

    if(w)
        fprintf(stdout, "\n");
}