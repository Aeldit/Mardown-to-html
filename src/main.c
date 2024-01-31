#include <stdio.h>

#include "files_handling.h"
#include "parsing/base.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    fc_control_ts *fcc = read_file(argv[1]);
    if (fcc == NULL)
    {
        return 1;
    }

    parse_h(fcc);

    print_file_content(fcc);
    destroy_fc_control(fcc);
    return 0;
}
