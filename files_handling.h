#ifndef FILES_HANDLING_H
#define FILES_HANDLING_H

#define FILE_BUFF_SIZE 512

typedef struct
{
	char buffer[FILE_BUFF_SIZE];
	struct file_content_t* next;
} file_content_t;

typedef struct
{
	size_t nb_buffers;
	file_content_t* head;
} fc_control_t;

fc_control_t* read_file(char* path);

#endif