#include "files_handling.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/custom_strings.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
int destroy_read_file(FILE *in_f, FILE *out_f, int failure)
{
    fclose(in_f);
    fclose(out_f);
    return failure;
}

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
    char buff[FILE_BUFF_SIZE] = "";
    while ((c = fgetc(in_f)) != EOF)
    {
        if (fseek(in_f, lines, SEEK_SET) != 0)
        {
            break;
        }

        // When the buffer is full, we write it to the file
        if (buff_idx == FILE_BUFF_SIZE - 1)
        {
            fprintf(out_f, buff, NULL);
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
