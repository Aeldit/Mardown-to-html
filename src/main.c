#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "files_handling.h"

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

    size_t *nb_h = get_nb_headers(fcc);
    printf("%lu", nb_h[0]);
    printf("%lu", nb_h[1]);
    printf("%lu", nb_h[2]);
    printf("%lu", nb_h[3]);
    printf("%lu", nb_h[4]);
    printf("%lu\n", nb_h[5]);
    free(nb_h);

    print_file_content(fcc);
    destroy_fc_control(fcc);
    return 0;
}
