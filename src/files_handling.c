#include "files_handling.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "translation.h"
#include "utils/custom_strings.h"
#include "utils/utils.h"

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
int destroy_read_file(FILE *in_f, FILE *out_f, int failure)
{
    fclose(in_f);
    fclose(out_f);
    return failure;
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
int read_file(char *path)
{
    FILE *in_f = fopen(path, "r");
    if (in_f == NULL)
    {
        return 1;
    }

    char *new_path = replace_extension(path, "md", "html");
    if (new_path == NULL)
    {
        free(new_path);
        return destroy_read_file(in_f, NULL, 1);
    }
    FILE *out_f = fopen(new_path, "w");
    free(new_path);
    if (out_f == NULL)
    {
        return destroy_read_file(in_f, NULL, 1);
    }

    int lines = 0;
    if (fseek(in_f, lines++, SEEK_SET) != 0)
    {
        return destroy_read_file(in_f, out_f, 1);
    }

    char c;
    int buff_idx = 0;
    char buff[BUFF_SIZE] = "";

    // Stores the element we where in before writing the preivous buffer
    int last_lement = NO_ELT;
    while ((c = fgetc(in_f)) != EOF)
    {
        if (fseek(in_f, lines, SEEK_SET) != 0)
        {
            break;
        }

        // When the buffer is full, we write it to the file
        if (buff_idx == BUFF_SIZE - 1)
        {
            translate_write_to_html(buff, last_lement, out_f);
            buff_idx = 0;
        }
        else
        {
            buff[buff_idx++] = c;
        }
        lines++;
    }
    return destroy_read_file(in_f, out_f, 0);
}
