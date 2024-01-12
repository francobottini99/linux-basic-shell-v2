/**
 * @file MyShell.c
 * @author Bottini, Franco Nicolas.
 * @brief Implementacion de MyShell.
 * @version 1.5
 * @date Octubre de 2022.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../inc/MyShell.h"

int main(int argc, char* argv[])
{
    FILE* source;

    myshell_validate_execution(argc);
    job_control_init();

    source = command_source(argc, argv);

    if (source == stdin)
        print_welcome();
        
    myshell_loop(source);

    return EXIT_SUCCESS;
}

void print_welcome()
{
    fprintf(stdout, KCYN);
    fprintf(stdout, "\nWelcome to MyShell !\n");
    fprintf(stdout, "Built-in Commands:\n");
    fprintf(stdout, "   * cd  : change current working directory\n");
    fprintf(stdout, "   * quit: terminate the shell\n");
    fprintf(stdout, "   * jobs: list all current jobs\n");
    fprintf(stdout, "   * fg  : reanude foreground job execution\n");
    fprintf(stdout, "   * kill: terminate job or process\n");
    fprintf(stdout, "   * clr : clear terminal\n");
    fprintf(stdout, "   * echo: print in terminal message or enviroment variable value\n");
    fprintf(stdout, "Implement job control\n");
    fprintf(stdout, "Run externed programs either in foreground or background (&)\n");
    fprintf(stdout, "Create pipeline using pipe operator (|)\n");
    fprintf(stdout, "Ctrl-C, Ctrl-Z and Ctrl-\\ signal handling in jobs\n");
    fprintf(stdout, "Standard input/output redirection operators: < and >\n\n");
    fprintf(stdout, KDEF);
}

void myshell_validate_execution(int argc)
{
    if(argc > 2)
    {
        fprintf(stderr, KRED"\nOnly one input argument is allowed !\n"KDEF);
        fprintf(stderr, KBLU"Input argument: batchfile.\n\n"KDEF);
        exit(EXIT_FAILURE);
    }
}

void myshell_loop(FILE* input_source)
{
    READ_INPUT_RESULT read_result = INP_NULL;

    while (1)
    {
        char* input_buffer = malloc(MAX_LEN_INPUT * sizeof(char));

        if(input_source == stdin && (read_result != INP_END || flag_work_tube_printed))
        {
            print_prompt();
            flag_work_tube_printed = 0;
        }    
        
        read_result = get_input(input_buffer, MAX_LEN_INPUT, input_source);

        if (read_result == INP_READ)
        {
            if(input_source != stdin)
                fprintf(stdout, KDEF"> %s\n", input_buffer);

            input_decode(input_buffer);
        }
        else
        {
            if(input_source != stdin)
            {
                if(read_result != INP_EMPTY_LINE)
                {
                    fclose(input_source);

                    if(read_result == INP_END)
                    {
                        while(first_job);
                        exit(EXIT_SUCCESS);
                    }
                        
                    if(read_result == INP_TO_LONG)
                        exit(EXIT_FAILURE);
                }
            }
            else if(read_result == INP_EMPTY_LINE)
                fprintf(stdout, "\n");
        }

        free(input_buffer);
    }  
}

void print_prompt(void)
{
    fprintf(stdout, KGRN"%s@%s~$ "KDEF, getenv("USER"), getenv("PWD"));
}

FILE* command_source(int argc, char* argv[])
{
    FILE* fp;

    if (argc == 2)
    {
        fp = fopen(argv[1], "r");
  	    
        if(fp == NULL)
        {
            fprintf(stderr, KRED"\n%s\n\n"KDEF, strerror(errno));  
            exit(EXIT_FAILURE);
        }
    }
    else
        fp = stdin;

    return fp;
}

READ_INPUT_RESULT get_input(char* buffer, int buffer_size, FILE* fp)
{
    if(fgets(buffer, buffer_size + 1, fp) == NULL)
        return INP_END;
    
    char* endOfBuffer = buffer + strlen(buffer) - 1;

    if((*endOfBuffer != ASCII_LINE_BREAK && fp == stdin) || (*endOfBuffer != ASCII_LINE_BREAK && *endOfBuffer != EOF && strlen(buffer) - 1 >= buffer_size && fp != stdin))
    {
        while(getchar() != ASCII_LINE_BREAK);
        fprintf(stderr, KRED"\nExceeded max input length !\n\n"KDEF);
        return INP_TO_LONG;
    }

    if(trim_white_space(buffer) == NULL)
        return INP_EMPTY_LINE;

    return INP_READ;
}

void input_decode(char* input)
{
    COMMANDS_FLAGS flag;
    char* command;
    char* args;
    char* input_cpy = malloc(sizeof(char) * strlen(input));

    strcpy(input_cpy, input);

    command = strtok_r(input, " ", &args);

    for(flag = CONST_STR_ARR_SIZE(CMM_VALIDS) - 1; flag >= -1; flag--)
        if(flag == CMM_EXTERN || !strcmp(CMM_VALIDS[flag], command)) 
            break;

    if (flag != CMM_EXTERN)
        command_handler(flag, args);
    else
        command_handler(flag, input_cpy);

    free(input_cpy);
}

void command_handler(COMMANDS_FLAGS cmm, char* args)
{
    switch (cmm)
    {
        case CMM_CD:
            execute_cd(args);
            break;

        case CMM_ECHO:
            execute_echo(args);
            break;

        case CMM_CLR:
            execute_clr(args);
            break;

        case CMM_JOBS:
            execute_jobs(args);
            break;

        case CMM_KILL:
            execute_kill(args);
            break;
        
        case CMM_FG:
            execute_fg(args);
            break;

        case CMM_QUIT:
            execute_quit(args);
            break;
    
        default:
            execute_extern(args);
            break;
    }
}