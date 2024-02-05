#include "translation.h"

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

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void translate_to_html(char buff[BUFF_SIZE], char dest_buff[BUFF_SIZE],
                       enum ELEMENTS elt)
{
    // Can be -1 (no header), 0, 1, 2, 3, 4 or 5
    int h_level = -1;
    char prev_c = '\0';
    char is_in_header = IS_IN_HEADER(elt);
    int dest_buff_idx = 0;

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
                dest_buff[dest_buff_idx++] = elements[h_level][0][j];
            }
        }
        else if (is_in_header)
        {
            if (buff[i] == '\n')
            {
                // Adds the '</hx>' element end to the dest_buff
                for (int j = 0; j < NB_CHAR_H_END; j++)
                {
                    dest_buff[dest_buff_idx++] = elements[h_level][1][j];
                }
                h_level = -1;
                is_in_header = 0;
            }
            else
            {
                dest_buff[dest_buff_idx++] = buff[i];
            }
        }
        prev_c = buff[i];
    }
}
