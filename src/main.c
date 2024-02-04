#include "files_handling.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    read_file(argv[1]);

    /*size_t *nb_h = get_nb_headers(fcc);
    printf("%lu", nb_h[0]);
    printf("%lu", nb_h[1]);
    printf("%lu", nb_h[2]);
    printf("%lu", nb_h[3]);
    printf("%lu", nb_h[4]);
    printf("%lu\n", nb_h[5]);
    free(nb_h);

    size_t *nb_deco = get_nb_text_decorations(fcc);
    printf("%lu", nb_deco[0]);
    printf("%lu", nb_deco[1]);
    printf("%lu\n", nb_deco[2]);
    free(nb_deco);*/

    return 0;
}
