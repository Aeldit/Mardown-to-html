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
char *str_concat(char *a, char *b)
{
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    size_t total_size = len_a + len_b + 1;
    char *new = malloc(total_size * sizeof(char *));

    memcpy(new, a, len_a);
    memcpy(new + len_a, b, len_b);

    new[total_size - 1] = '\0';
    return new;
}

/**
** \brief Destroys the given splited_string and returns NULL, to save some lines
*/
void *destroy_splited_string_on_fail(splited_string_ts *sps)
{
    destroy_splited_string(sps);
    return NULL;
}

splited_string_ts *init_splited_string(char *s, char sep)
{
    splited_string_ts *sps = calloc(1, sizeof(splited_string_ts));
    if (sps == NULL)
    {
        return NULL;
    }

    size_t len = strlen(s);
    sps->nb_strings = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (s[i] == sep)
        {
            sps->nb_strings++;
        }
    }
    // If the last character is not a separator, we have 1 more char array
    if (s[len - 1] != sep)
    {
        sps->nb_strings++;
    }

    sps->nb_char_each = calloc(sps->nb_strings, sizeof(unsigned));
    if (sps->nb_char_each == NULL)
    {
        return destroy_splited_string_on_fail(sps);
    }
    return sps;
}

splited_string_ts *split_string(char *s, char sep)
{
    splited_string_ts *sps = init_splited_string(s, sep);
    if (sps == NULL)
    {
        return NULL;
    }

    size_t len = strlen(s);

    // Counts the number of characters in each string separated by a 'sep'
    // ex : 'testing/yes' would be [ 7, 3 ]
    int ctr = 0;
    int idx = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (s[i] == sep)
        {
            sps->nb_char_each[idx++] = ctr;
            ctr = 0;
        }
        else
        {
            ctr++;
        }
    }
    // If there was more characters after the last '/', we add their number
    if (ctr != 0)
    {
        sps->nb_char_each[idx] = ctr;
    }

    sps->splited = calloc(sps->nb_strings, sizeof(char *));
    if (sps->splited == NULL)
    {
        return destroy_splited_string_on_fail(sps);
    }

    // Allocates arrays of chars of the correct size for each char array that
    // was splited
    for (unsigned i = 0; i < sps->nb_strings; i++)
    {
        sps->splited[i] = calloc(sps->nb_char_each[i], sizeof(char *));
        if (sps->splited[i] == NULL)
        {
            return destroy_splited_string_on_fail(sps);
        }
    }

    idx = 0;
    int char_idx = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (s[i] == sep)
        {
            idx++;
            char_idx = 0;
        }
        else
        {
            sps->splited[idx][char_idx++] = s[i];
        }
    }
    return sps;
}

void print_splited_string(splited_string_ts *sps)
{
    if (sps == NULL || sps->splited == NULL || sps->nb_char_each == NULL)
    {
        return;
    }

    for (size_t i = 0; i < sps->nb_strings; i++)
    {
        printf("%s", sps->splited[i]);
        printf("\n");
    }
}

void destroy_splited_string(splited_string_ts *sps)
{
    if (sps == NULL)
    {
        return;
    }

    for (size_t i = 0; i < sps->nb_strings; i++)
    {
        free(sps->splited[i]);
    }
    free(sps->splited);
    free(sps->nb_char_each);
    free(sps);
}

void string_append(char *dest, char *s, size_t dest_idx)
{
    if (dest == NULL || s == NULL)
    {
        return;
    }

    for (size_t i = 0; i < strlen(s); i++)
    {
        dest[dest_idx++] = s[i];
    }
}
