#include "base.h"

#include <stdio.h>
#include <stdlib.h>

char *parse_h(fc_control_ts *fcc)
{
    int first_char = 1;
    size_t nb_h = 0;
    char h_level = 0; // Can be 0 (no header), 1, 2 or 3
    file_content_ts *current_fc = fcc->head;
    char *buff = current_fc->buffer;
    for (size_t i = 0; i < fcc->nb_buffers; i++)
    {
        char prev_c = '\0';
        size_t idx = 0;
        while (idx < FILE_BUFF_SIZE)
        {
            // <h1>
            if ((prev_c == '\n' || first_char) && buff[idx] == '#')
            {
                first_char = 0;
                h_level = 1;
                nb_h++;
            }
            else if (h_level > 0 && buff[idx] == '#')
            {
                h_level++;
            }
            else if (h_level > 0 && buff[idx] != '#')
            {
                h_level = 0;
            }

            idx++;
            prev_c = buff[idx];
        }

        if (current_fc->next == NULL)
        {
            break;
        }
        current_fc = current_fc->next;
    }
    printf("%lu\n", nb_h);
    // TODO -> generate the html string
    return NULL;
}
