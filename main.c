#include "files_handling.h"

int main(void)
{
	fc_control_t* fcc = read_file("./main.c");
	print_file_content(fcc);
	return 0;
}