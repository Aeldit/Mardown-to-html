#include "translation.h"

#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
**                                  CONSTANTS                                 **
*******************************************************************************/
const char *elements[][5] = {
    { "<h1>", "</h1>" }, { "<h2>", "</h2>" }, { "<h3>", "</h3>" },
    { "<h4>", "</h4>" }, { "<h5>", "</h5>" }, { "<h6>", "</h6>" },
};

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
size_t get_nb_headers(char buff[BUFF_SIZE])
{
    // Can be -1 (no header), 0, 1, 2, 3, 4 or 5
    int h_level = -1;
    char prev_c = '\0';
    size_t nb_h = 0;

    for (size_t idx = 0; idx < BUFF_SIZE; idx++)
    {
        // Start of a header
        if ((prev_c == '\n' || prev_c == '\0') && buff[idx] == '#')
        {
            h_level = 0;
        }
        // Gets the number of '#' that are for the same header
        else if (h_level != -1 && buff[idx] == '#')
        {
            h_level++;
        }
        // When the header stops
        else if (h_level != -1 && buff[idx] != '#')
        {
            // If the header has between 1 and 6 '#', we add it to the list
            // of headers
            if (0 <= h_level && h_level <= 5)
            {
                nb_h++;
            }
            h_level = -1;
        }
        prev_c = buff[idx];
    }
    return nb_h;
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void translate_write_to_html(char buff[BUFF_SIZE], enum ELEMENTS elt,
                             FILE *out_f)
{
    // Can be -1 (no header), 0, 1, 2, 3, 4 or 5
    int h_level = -1;
    char prev_c = '\0';
    char is_in_header = IS_IN_HEADER(elt);
    int dest_buff_idx = 0;

    // 5 is because we may have started a header in the previous buffer
    // and 9 * because a header element contains 9 cahracters in total in html
    char *html_buff =
        calloc(5 + 9 * get_nb_headers(buff) + BUFF_SIZE, sizeof(char *));

    for (size_t i = 0; i < BUFF_SIZE; i++)
    {
        // Start of a header
        if ((prev_c == '\n' || prev_c == '\0') && buff[i] == '#')
        {
            h_level = 0;
        }
        // Gets the number of '#' that are for the same header
        else if (h_level != -1 && buff[i] == '#')
        {
            h_level++;
        }
        // When the header stops
        else if (h_level != -1 && buff[i] == ' ' && !is_in_header)
        {
            is_in_header = 1;
            // Adds the '<hx>' element start to the dest_buff
            for (int j = 0; j < NB_CHAR_H_START; j++)
            {
                html_buff[dest_buff_idx++] = elements[h_level][0][j];
            }
        }
        else if (is_in_header)
        {
            if (buff[i] == '\n')
            {
                // Adds the '</hx>' element end to the dest_buff
                for (int j = 0; j < NB_CHAR_H_END; j++)
                {
                    html_buff[dest_buff_idx++] = elements[h_level][1][j];
                }
                h_level = -1;
                is_in_header = 0;
            }
            else
            {
                html_buff[dest_buff_idx++] = buff[i];
            }
        }
        else
        {
            html_buff[dest_buff_idx++] = buff[i];
        }
        prev_c = buff[i];
    }
    fprintf(out_f, html_buff, NULL);
    free(html_buff);
}
