#ifndef FILES_HANDLING_H
#define FILES_HANDLING_H

#include <stddef.h>

#define FILE_BUFF_SIZE 512

typedef struct
{
	char buffer[FILE_BUFF_SIZE];
	struct file_content_t* next;
} file_content_t;

typedef struct
{
	size_t nb_buffers;
	char* file_name;
	file_content_t* head;
	file_content_t* tail;
} fc_control_t;

fc_control_t* read_file(char* path);

void print_file_content(fc_control_t* fcc);

#endif