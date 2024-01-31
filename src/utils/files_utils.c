#include "files_utils.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../files_handling.h"
#include "custom_strings.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/***************************************
**     FC_CONTROL / FILE_CONTENT      **
***************************************/
file_content_ts *init_file_content(void)
{
    file_content_ts *fc = calloc(1, sizeof(file_content_ts));
    if (fc == NULL)
    {
        return NULL;
    }

    fc->next = NULL;
    return fc;
}

fc_control_ts *init_fc_control(char *file_name)
{
    fc_control_ts *fcc = calloc(1, sizeof(fc_control_ts));
    if (fcc == NULL)
    {
        return NULL;
    }

    file_content_ts *fc = init_file_content();
    if (fc == NULL)
    {
        free(fcc);
        return NULL;
    }

    fcc->nb_buffers = 1;
    fcc->head = fc;
    fcc->tail = fc;

    fcc->file_name = malloc((strlen(file_name) + 1) * sizeof(char *));
    if (fcc->file_name == NULL)
    {
        free(fcc);
        free(fc);
        return NULL;
    }
    memcpy(fcc->file_name, file_name, strlen(file_name) + 1);
    return fcc;
}

void add_fc_to_fc_control(fc_control_ts *fcc, file_content_ts *fc)
{
    if (fcc == NULL || fc == NULL)
    {
        return;
    }

    fcc->tail->next = fc;
    fcc->tail = fc;
    fcc->nb_buffers++;
}

/***************************************
**    FILES & DIRECTORIES CREATION    **
***************************************/
void create_dir_if_absent(char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) == -1)
    {
        mkdir(path, 0700);
    }
}

void create_subdirs_if_absent(char *path)
{
    splited_string_ts *s = split_string(path, '/');
    if (s == NULL || s->nb_char_each == NULL || s->splited == NULL)
    {
        return;
    }

    unsigned total_size = s->nb_strings - 1; // s->nb_strings <=> number of '/'
    for (unsigned i = 0; i < s->nb_strings - 1; i++)
    {
        total_size += s->nb_char_each[i];
    }

    char *full_path = calloc(total_size, sizeof(char *));
    unsigned full_path_idx = 0;
    // s->nb_strings - 1 because the last string of the path is the file and not
    // a dir
    for (unsigned i = 0; i < s->nb_strings - 1; i++)
    {
        string_append(full_path, s->splited[i], full_path_idx);
        full_path_idx += strlen(s->splited[i]);
        string_append(full_path, "/", full_path_idx++);
        create_dir_if_absent(full_path);
    }
    destroy_splited_string(s);
    free(full_path);
}

int write_file_destroy(char *path_in_dir, splited_string_ts *sps, char error)
{
    free(path_in_dir);
    destroy_splited_string(sps);
    return error;
}
