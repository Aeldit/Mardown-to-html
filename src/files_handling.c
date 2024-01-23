#include "files_handling.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct file_content *init_file_content(void)
{
    struct file_content *fc = calloc(1, sizeof(struct file_content));
    if (fc == NULL)
    {
        return NULL;
    }

    fc->next = NULL;
    return fc;
}

struct fc_control *init_fc_control(char *file_name, size_t file_name_len)
{
    struct fc_control *fcc = calloc(1, sizeof(struct fc_control));
    if (fcc == NULL)
    {
        return NULL;
    }

    struct file_content *fc = init_file_content();
    if (fc == NULL)
    {
        free(fcc);
        return NULL;
    }

    fcc->nb_buffers = 1;
    fcc->head = fc;
    fcc->tail = fc;

    fcc->file_name = malloc((file_name_len + 1) * sizeof(char *));
    if (fcc->file_name == NULL)
    {
        free(fcc);
        free(fc);
        return NULL;
    }
    memcpy(fcc->file_name, file_name, file_name_len + 1);
    return fcc;
}

void add_fc_to_fc_control(struct fc_control *fcc, struct file_content *fc)
{
    if (fcc == NULL || fc == NULL)
    {
        return;
    }

    fcc->tail->next = fc;
    fcc->tail = fc;
    fcc->nb_buffers++;
}

struct fc_control *read_file(char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        return NULL;
    }

    struct fc_control *fcc = init_fc_control(path, strlen(path));
    if (fcc == NULL)
    {
        return NULL;
    }

    struct file_content *current_fc = calloc(1, sizeof(struct file_content));
    if (current_fc == NULL)
    {
        destroy_fc_control(fcc);
        return NULL;
    }
    add_fc_to_fc_control(fcc, current_fc);

    int lines = 0;
    if (fseek(f, lines++, SEEK_SET) != 0)
    {
        free(fcc);
        free(current_fc);
        return NULL;
    }

    char c;
    int buff_idx = 0;
    // last_fc is used to get the last read file_content (which is not full)
    struct file_content *last_fc = NULL;
    while ((c = fgetc(f)) != EOF)
    {
        if (fseek(f, lines++, SEEK_SET) != 0)
        {
            break;
        }

        if (buff_idx < FILE_BUFF_SIZE)
        {
            current_fc->buffer[buff_idx++] = c;
        }
        else
        {
            add_fc_to_fc_control(fcc, current_fc);
            struct file_content *new_fc = init_file_content();
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

void print_file_content(struct fc_control *fcc)
{
    if (fcc == NULL)
    {
        return;
    }

    printf("\n============================================================\n");
    printf("  Printing the content of the file '%s'", fcc->file_name);
    printf("\n============================================================\n");
    struct file_content *tmp = fcc->head;
    while (tmp != NULL)
    {
        printf("%s", tmp->buffer);
        tmp = tmp->next;
    }
}

void destroy_fc_control(struct fc_control *fcc)
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
        struct file_content *tmp = fcc->head;
        fcc->head = fcc->head->next;
        free(tmp);
    }
    free(fcc->file_name);
    free(fcc);
}
