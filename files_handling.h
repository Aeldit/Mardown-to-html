#ifndef FILES_HANDLING_H
#define FILES_HANDLING_H

typedef struct
{
	char buffer[512];
	struct file_content_t* next;
} file_content_t;

typedef struct
{
	size_t nb_buffers;
	file_content_t* head;
} fc_control_t;

#endif