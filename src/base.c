#include "base.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
size_t *get_nb_headers(fc_control_ts *fcc)
{
    // Contains the number of header for each possible category (h1 to h6)
    size_t *nb_h_each = calloc(6, sizeof(size_t));
    if (nb_h_each == NULL)
    {
        return NULL;
    }

    // To know if a '#' is for a header, we use the '\n' of the previous line as
    // a mark, but hen we are at the first character of the file there is no
    // '\n' before, so this is only to store this state
    int first_char = 1;
    int h_level = 0; // Can be 0 (no header), 1, 2, 3, 4, 5 or 6
    file_content_ts *current_fc = fcc->head;

    for (size_t i = 0; i < fcc->nb_buffers; i++)
    {
        char *buff = current_fc->buffer;
        char prev_c = '\0';
        size_t idx = 0;

        while (idx < FILE_BUFF_SIZE)
        {
            // Start of a header
            if ((prev_c == '\n' || first_char) && buff[idx] == '#')
            {
                first_char = 0;
                h_level = 1;
            }
            // Gets the number of '#' that are for the same header
            else if (h_level > 0 && buff[idx] == '#')
            {
                h_level++;
            }
            // When the header stops
            else if (h_level > 0 && buff[idx] != '#')
            {
                // If the header has between 1 and 6 '#', we add it to the list
                // of headers
                if (1 <= h_level && h_level <= 6)
                {
                    nb_h_each[h_level - 1] += 1;
                }
                h_level = 0;
            }
            prev_c = buff[idx];
            idx++;
        }

        if (current_fc->next == NULL)
        {
            break;
        }
        current_fc = current_fc->next;
    }
    return nb_h_each;
}
