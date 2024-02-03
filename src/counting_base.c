#include "counting_base.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "utils/custom_strings.h"
#include "utils/utils.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char *get_html_str(fc_control_ts *fcc)
{
    size_t *nb_h = get_nb_headers(fcc);
    size_t html_parts_lenght =
        9 * (nb_h[0] + nb_h[1] + nb_h[2] + nb_h[3] + nb_h[4] + nb_h[5]) + 1;

    printf("str len : %lu\n", html_parts_lenght);

    char *html_str = calloc(html_parts_lenght, sizeof(char *));
    if (html_str == NULL)
    {
        free(nb_h);
        return NULL;
    }

    size_t idx = 0;
    for (int i = 0; i < 6; i++)
    {
        for (size_t j = 0; j < nb_h[i]; j++)
        {
            char tmp[10];
            sprintf(tmp, "<h%d></h%d>", i + 1, i + 1);
            string_append(html_str, tmp, idx);
            idx += LEN_HEADER;
        }
    }

    free(nb_h);
    return html_str;
}

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
    // int first_char = 1;
    int h_level = 0; // Can be 0 (no header), 1, 2, 3, 4, 5 or 6
    file_content_ts *current_fc = fcc->head;
    char prev_c = '\0';

    for (size_t i = 0; i < fcc->nb_buffers; i++)
    {
        char *buff = current_fc->buffer;

        for (size_t idx = 0; idx < FILE_BUFF_SIZE; idx++)
        {
            // Start of a header
            // if ((prev_c == '\n' || first_char) && buff[idx] == '#')
            if ((prev_c == '\n' || prev_c == '\0') && buff[idx] == '#')
            {
                // first_char = 0;
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
                    nb_h_each[h_level - 1]++;
                }
                h_level = 0;
            }
            prev_c = buff[idx];
        }

        if (current_fc->next == NULL)
        {
            break;
        }
        current_fc = current_fc->next;
    }
    return nb_h_each;
}

size_t *get_nb_text_decorations(fc_control_ts *fcc)
{
    // Contains the number of 'bold' 'italic' and 'code' text
    size_t *nb_deco_each = calloc(3, sizeof(size_t));
    if (nb_deco_each == NULL)
    {
        return NULL;
    }

    file_content_ts *current_fc = fcc->head;
    char prev_c = '\0';
    char nb_star = 0;
    char is_in_deco = 0;

    for (size_t i = 0; i < fcc->nb_buffers; i++)
    {
        char *buff = current_fc->buffer;

        for (size_t idx = 0; idx < FILE_BUFF_SIZE; idx++)
        {
            if (nb_star == 0 && buff[idx] == '*')
            {
                nb_star = 1;
            }
            else if (nb_star == 1)
            {
                if (!is_in_deco)
                {
                    // Bold text
                    if (buff[idx] == '*')
                    {
                        nb_deco_each[0]++;
                    }
                    // Italic text
                    else
                    {
                        nb_deco_each[1]++;
                    }
                }
                is_in_deco = !is_in_deco;
                nb_star = 0;
            }
            // Code text (not the fenced type)
            else if (buff[idx] == '`' && prev_c != '`')
            {
                if (!is_in_deco)
                {
                    nb_deco_each[2]++;
                }
                is_in_deco = !is_in_deco;
            }
            prev_c = buff[idx];
        }

        if (current_fc->next == NULL)
        {
            break;
        }
        current_fc = current_fc->next;
    }
    return nb_deco_each;
}
