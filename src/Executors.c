/**
 * @file Executors.h
 * @author Bottini, Franco Nicolas.
 * @brief Implementacion de los ejecutores para los comandos de MyShell.
 * @version 1.5
 * @date Octubre de 2022.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../inc/Executors.h"

void execute_cd(char* args)
{
    if(*args == ASCII_MIDDLE_DASH)
        args = getenv("OLDPWD");
    
    if (chdir(args) != 0)
        fprintf(stderr, KRED"\n%s\n\n"KDEF, strerror(errno));  
    else
    {
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", getcwd(NULL, 1024), 1);
        fprintf(stdout, "\n");
    }
}

void execute_echo(char* args)
{
    if(strlen(args) == 0) 
        return;

    FILE* out_fp;
    char *outfile = get_word_after_char(args, ASCII_GREATER_THAN);
    char *text = get_substr_before_chars(args, ASCII_LESS_THAN, ASCII_GREATER_THAN);
    char *end_str;
    char *word = strtok_r(text, " ", &end_str);

    if(outfile && *outfile == ASCII_LINE_BREAK)
    {
        fprintf(stderr, KRED"\nParse error near '\\n' !\n\n"KDEF);
        return;
    }   

    if(!outfile)
        out_fp = stdout;
    else
        out_fp = fopen(outfile, "w");

    if(!out_fp)
    {
        fprintf(stderr, KRED"\n%s\n\n"KDEF, strerror(errno));
        return;
    }

    fprintf(stdout, "\n");

    while (word != NULL)
    {
        char *end_word;
        char *sub_word = strtok_r(word, "$", &end_word);

        if(*word != ASCII_MONEY_SIGN)
        {
            if(!outfile)
                fprintf(out_fp, KBLU"%s"KDEF, sub_word);
            else
                fprintf(out_fp, "%s", sub_word);
                
            sub_word = strtok_r(NULL, "$", &end_word);
        }
        
        while (sub_word != NULL)
        {
            char* envvar = getenv(sub_word);

            if(envvar)
            {
                if(!outfile)
                    fprintf(out_fp, KBLU"%s"KDEF, envvar);
                else
                    fprintf(out_fp, "%s", envvar);
            }

            sub_word = strtok_r(NULL, "$", &end_word);
        }

        fprintf(out_fp, " ");

        word = strtok_r(NULL, " ", &end_str);
    }

    if(!outfile)
        fprintf(stdout, "\n\n");
    else
        fclose(out_fp);

    free(text);
}

void execute_jobs(char* args)
{
    if (!(strlen(args) > 0))
        print_job_all_status();
    else
        fprintf(stderr, KRED"\nThe command does not allow parameters !\n\n"KDEF);
}

void execute_kill(char* args)
{
    if(*args == ASCII_PERCENT)
        kill_job(atoi(++args));
    else
        fprintf(stderr, KRED"\nKill process is not supported !\n\n"KDEF);
}

void execute_fg(char* args)
{
    if(*args == ASCII_PERCENT)
        reanude_job_fg(atoi(++args));
    else
        fprintf(stderr, KRED"\nReanude individual process is not supported !\n\n"KDEF);
}

void execute_clr(char* args)
{
    if (!(strlen(args) > 0))
        system("clear");
    else
        fprintf(stderr, KRED"\nThe command does not allow parameters !\n\n"KDEF);
}

void execute_quit(char* args)
{
    if (!(strlen(args) > 0))
    {
        kill_all_jobs();  
        exit(EXIT_SUCCESS);
    }

    fprintf(stderr, KRED"\nThe command does not allow parameters !\n\n"KDEF);
}

void execute_extern(char* args)
{
    job *j;
    char *end_cmm;
    char *cmm;
    EXECUTION_MODES mode = get_execution_mode(args);

    if(mode == BADMODE_EXECUTION)
    {
        fprintf(stderr, KRED"\nBad used of '&' and '|' in command !\n\n"KDEF);
        return;
    }
    else if(mode == BACKGROUND_EXECUTION)
    {
        args[strlen(args) - 1] = ASCII_SPACE;
        args[strlen(args) - 2] = ASCII_SPACE;

        args = trim_white_space(args);
    }

    j = new_job();

    cmm = strtok_r(args, "|", &end_cmm);   
    while (cmm)
    {   
        char *infile = get_word_after_char(cmm, ASCII_LESS_THAN);
        char *outfile = get_word_after_char(cmm, ASCII_GREATER_THAN);

        char *operation = get_substr_before_chars(cmm, ASCII_LESS_THAN, ASCII_GREATER_THAN);

        process *p = new_process(operation, infile, outfile);

        insert_process(j, p);
        
        cmm = strtok_r(NULL, "|", &end_cmm);

        free(operation);
    }

    if(j->first_process)
        launch_job(j, mode);
}

EXECUTION_MODES get_execution_mode(char* args)
{
    if(args[strlen(args) - 1] == ASCII_AMPERSAND)
        return BACKGROUND_EXECUTION;

    int match = 0;
    for (size_t i = 0; i < strlen(args); i++)
    {
        if(match && args[i] == ASCII_PLECA)
            return BADMODE_EXECUTION;

        if(match && args[i] != ASCII_SPACE)
            match = 0;

        if (args[i] == ASCII_AMPERSAND)
            match = 1;
    }

    return FOREGROUND_EXECUTION;
}