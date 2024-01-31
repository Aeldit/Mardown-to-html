#include "files_handling.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/custom_strings.h"
#include "utils/files_utils.h"

/*******************************************************************************
**                              READING
*******************************************************************************/
fc_control_ts *read_file(char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        return NULL;
    }

    fc_control_ts *fcc = init_fc_control(path);
    if (fcc == NULL)
    {
        return NULL;
    }

    file_content_ts *current_fc = fcc->head;

    int lines = 0;
    if (fseek(f, lines++, SEEK_SET) != 0)
    {
        destroy_fc_control(fcc);
        return NULL;
    }

    char c;
    int buff_idx = 0;
    // last_fc is used to get the last read file_content (which is not full)
    file_content_ts *last_fc = NULL;
    while ((c = fgetc(f)) != EOF)
    {
        if (fseek(f, lines++, SEEK_SET) != 0)
        {
            break;
        }

        // If there is still place in the payload buffer, we add the character
        // to it
        if (buff_idx < FILE_BUFF_SIZE - 1)
        {
            current_fc->buffer[buff_idx++] = c;
        }
        else
        {
            add_fc_to_fc_control(fcc, current_fc);
            // Creates a new link and assigns it to current_fc
            file_content_ts *new_fc = init_file_content();
            if (new_fc == NULL)
            {
                break;
            }
            buff_idx = 0;
            current_fc = new_fc;
            last_fc = new_fc;
        }
    }
    add_fc_to_fc_control(fcc, last_fc);
    return fcc;
}

void print_file_content(fc_control_ts *fcc)
{
    if (fcc == NULL)
    {
        return;
    }

    printf("============================================================\n");
    printf("  Printing the content of the file '%s'", fcc->file_name);
    printf("\n============================================================\n");
    file_content_ts *tmp = fcc->head;
    while (tmp != NULL)
    {
        printf("%s", tmp->buffer);
        tmp = tmp->next;
    }
}

void destroy_fc_control(fc_control_ts *fcc)
{
    if (fcc == NULL)
    {
        return;
    }

    if (fcc->head == NULL)
    {
        free(fcc->file_name);
        free(fcc);
        return;
    }

    while (fcc->head != NULL)
    {
        file_content_ts *tmp = fcc->head;
        fcc->head = fcc->head->next;
        free(tmp);
    }
    free(fcc->file_name);
    free(fcc);
}

/***************************************
**    FILES & DIRECTORIES CREATION    **
***************************************/
int write_file(char *path, fc_control_ts *fcc, char *date)
{
    if (fcc == NULL || fcc->head == NULL)
    {
        return 1;
    }

    // Creates the final dir string
    size_t dir_len = strlen(date);
    size_t path_len = strlen(path);
    char *path_in_dir = calloc(1, (dir_len + path_len + 1) * sizeof(char *));
    if (path_in_dir == NULL)
    {
        return 1;
    }

    size_t i = 0;
    // Creates the root folder of the current server with the date
    for (; i < dir_len; i++)
    {
        path_in_dir[i] = date[i];
    }

    // Creates the sub-directories contained by the path to the file
    splited_string_ts *sps = split_string(path, '/');
    if (sps == NULL)
    {
        return write_file_destroy(path_in_dir, NULL, 1);
    }

    // Add the path of the given file
    for (; i < dir_len + path_len; i++)
    {
        path_in_dir[i] = path[i - dir_len];
    }

    path_in_dir[dir_len + path_len] = '\0';

    create_subdirs_if_absent(path_in_dir);
    FILE *f = fopen(path_in_dir, "w");

    if (f == NULL)
    {
        return write_file_destroy(path_in_dir, sps, 1);
    }

    // Writes every received payload inside the file
    file_content_ts *fc = fcc->head;
    while (fc != NULL)
    {
        fprintf(f, fc->buffer, NULL);
        fc = fc->next;
    }

    fclose(f);
    return write_file_destroy(path_in_dir, sps, 0);
}
