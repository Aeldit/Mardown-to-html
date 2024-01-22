#include <stdio.h>
#include <stdlib.h>

#include "files_handling.h"

fc_control_t* read_file(char* path)
{
	FILE* f = fopen(path, "r");
	if (f == NULL)
	{
		return NULL;
	}

	fc_control_t* fcc = calloc(1, sizeof(fc_control_t));
	if (fcc == NULL)
	{
		return NULL;
	}

	file_content_t* fc = calloc(1, sizeof(file_content_t));
	if (fc == NULL)
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
	while ((c = fgetc(f)) != EOF)
	{
		if (fseek(f, lines++, SEEK_SET) != 0)
		{
			break;
		}

		if (buff_idx < FILE_BUFF_SIZE)
		{
			fc->buffer[buff_idx++] = c;
		}
		else
		{
			buff_idx = 0;
		}
	}
	return NULL;
}