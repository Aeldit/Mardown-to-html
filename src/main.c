#include <stdio.h>

#include "files_handling.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    struct fc_control *fcc = read_file(argv[1]);
    if (fcc == NULL)
    {
        return 1;
    }

    printf("%s", fcc->file_name);
    print_file_content(fcc);
    destroy_fc_control(fcc);
    return 0;
}
