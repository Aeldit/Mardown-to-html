#include <stdio.h>

#include "files_handling.h"
#include <malloc.h>

struct file_content* init_file_content(void)
{
	file_content_t* fc = calloc(1, sizeof(file_content_t));
	if (fc == NULL)
	{
		return NULL;
	}

	fc->next = NULL;
	return fc;
}

fc_control_t* init_fc_control(char* file_name)
{
	fc_control_t* fcc = calloc(1, sizeof(fc_control_t));
	if (fcc == NULL)
	{
		return NULL;
	}

	struct file_content* fc = init_file_content();
	if (fc == NULL)
	{
		free(fcc);
		return NULL;
	}

	fcc->nb_buffers = 1;
	fcc->head = fc;
	fcc->tail = fc;

	fcc->file_name = malloc((strlen(file_name) + 1) * sizeof(char*));
	if (fcc->file_name == NULL)
	{
		free(fcc);
		free(fc);
		return NULL;
	}
	memcpy(fcc->file_name, file_name, strlen(file_name) + 1);
	return fcc;
}

void add_fc_to_fc_control(fc_control_t* fcc, file_content_t* fc)
{
	if (fcc == NULL || fc == NULL)
	{
		return;
	}

	fcc->tail->next = fc;
	fcc->tail = fc;
	fcc->nb_buffers++;
}

fc_control_t* read_file(char* path)
{
	printf("%s", path);
	FILE* f = fopen(path, "r");
	if (f == NULL)
	{
		return NULL;
	}

	fc_control_t* fcc = init_fc_control(path);
	if (fcc == NULL)
	{
		return NULL;
	}

	file_content_t* current_fc = calloc(1, sizeof(file_content_t));
	if (current_fc == NULL)
	{
		free(fcc);
		return NULL;
	}

	int lines = 0;
	if (fseek(f, lines++, SEEK_SET) != 0)
	{
		return NULL;
	}

	char c;
	int buff_idx = 0;
	// last_fc is used to get the last read file_content (which is not full)
	file_content_t* last_fc = NULL;
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
			file_content_t* new_fc = init_file_content();
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
	return NULL;
}

void print_file_content(fc_control_t* fcc)
{
	if (fcc == NULL)
	{
		return;
	}

	printf("\n#===========================================================\n");
	printf("# Printing the content of the file '%s'", fcc->file_name);
	printf("\n#===========================================================\n");
	file_content_t* tmp = fcc->head;
	while (tmp != NULL)
	{
		printf("%s", tmp->buffer);
		tmp = tmp->next;
	}
}
