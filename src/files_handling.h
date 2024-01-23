#ifndef FILES_HANDLING_H
#define FILES_HANDLING_H

#include <stddef.h>

#define FILE_BUFF_SIZE 512

struct file_content
{
    char buffer[FILE_BUFF_SIZE];
    struct file_content *next;
};

struct fc_control
{
    size_t nb_buffers;
    char *file_name;
    struct file_content *head;
    struct file_content *tail;
};

struct fc_control *read_file(char *path);

void print_file_content(struct fc_control *fcc);

void destroy_fc_control(struct fc_control *fcc);

#endif
