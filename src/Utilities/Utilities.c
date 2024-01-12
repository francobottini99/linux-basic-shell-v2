/**
 * @file Utilities.h
 * @author Bottini, Franco Nicolas.
 * @brief Implementacion de las funciones de proposito general.
 * @version 1.5
 * @date Octubre de 2022.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../inc/Utilities/Utilities.h"

char* get_substr_before_chars(char* str, char c1, char c2)
{
    int l = 0;
    char* substr = malloc(sizeof(char));

    for (size_t i = 0; (i < strlen(str) && str[i] != c1 && str[i] != c2); i++)
    {
        substr[l++] = str[i];
        substr = realloc(substr, (l + 1) * sizeof(char));
    }

    substr[l++] = ASCII_END_OF_STRING;

    return substr;
}

char* get_word_after_char(char* str, char c)
{
    int l = 0;
    char* word = malloc(sizeof(char));
    
    int match = 0;
    for (size_t i = 0; i < strlen(str); i++)
    {
        if(match)
        {
            if (str[i] == c)
                continue;
                
            if(str[i] != ASCII_SPACE)
            {
                word[l++] = str[i];
                word = realloc(word, (l + 1) * sizeof(char));
            }
            else if(l > 0)
                break;
        }
        else if (str[i] == c)
        {
            match = 1;
        }
    }

    if(match)
    {
        if (l < 1)
            word[l] = ASCII_LINE_BREAK;
        else
            word[l] = ASCII_END_OF_STRING;
        
        return word;
    }
    else
        return NULL;
}

char* trim_white_space(char* str)
{
    while(*str == ASCII_SPACE)
        str++;

    if(*str == 0 || *str == ASCII_LINE_BREAK)
        return NULL;

    char* endOfStr = str + strlen(str) - 1;

    while(endOfStr > str && (*endOfStr == ASCII_SPACE || *endOfStr == ASCII_LINE_BREAK))
        endOfStr--;

    endOfStr[1] = ASCII_END_OF_STRING;

    return str;
}

char** str_to_array(char* str, int* n)
{
    char** argv = malloc(sizeof(char*));

    *n = 0;
    
    char* token = strtok(str, " ");
    while (token != NULL)
    {
        argv[*n] = malloc(sizeof(char) * strlen(token));
        strcpy(argv[(*n)++], token);
        argv = realloc(argv, sizeof(char*) * (*n + 1));
        token = strtok(NULL, " ");
    }

    argv[*n] = NULL;

    return argv;
}

void free_array(char** array, int n)
{
    for (int i = 0; i < n; i++)
        free(array[i]);
    
    free(array);
}