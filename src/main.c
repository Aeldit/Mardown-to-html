#include "files_handling.h"

int main(void)
{
    struct fc_control *fcc = read_file("README.md");
    print_file_content(fcc);
    destroy_fc_control(fcc);
    return 0;
}
