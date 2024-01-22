#include <stdio.h>

#include "files_handling.h"

fc_control_t* read_file(char* path)
{
	FILE* f = fopen(path, "r");
	if (f == NULL)
	{
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
	}
	return NULL;
}