#include "custom_strings.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char *replace_extension(char *s, char *ext, char *new_ext)
{
    if (s == NULL || ext == NULL || new_ext == NULL)
    {
        return NULL;
    }

    size_t len_s = strlen(s);
    size_t len_ext = strlen(ext);
    size_t len_new_ext = strlen(new_ext);

    if (len_ext > len_s || len_new_ext > len_s)
    {
        return NULL;
    }

    size_t final_len = len_s;
    if (len_new_ext > len_ext)
    {
        final_len = len_s + (len_new_ext - len_ext);
    }

    char *new_string = calloc(final_len, sizeof(char *));
    if (new_string == NULL)
    {
        return NULL;
    }

    memcpy(new_string, s, len_s - len_ext);

    size_t idx = 0;
    for (size_t i = len_s - len_ext; i < final_len; i++, idx++)
    {
        new_string[i] = new_ext[idx];
    }
    return new_string;
}
