#ifndef FILES_HANDLING_H
#define FILES_HANDLING_H

#include <stddef.h>

#define FILE_BUFF_SIZE 512

typedef struct file_content_ts
{
    char buffer[FILE_BUFF_SIZE];
    struct file_content_ts *next;
} file_content_ts;

typedef struct fc_control_ts
{
    size_t nb_buffers;
    char *file_name;
    file_content_ts *head;
    file_content_ts *tail;
} fc_control_ts;

fc_control_ts *read_file(char *path);

void print_file_content(fc_control_ts *fcc);

void destroy_fc_control(fc_control_ts *fcc);

#endif
