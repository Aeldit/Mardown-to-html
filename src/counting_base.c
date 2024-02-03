#include "counting_base.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "elts_type_llist.h"
#include "utils/custom_strings.h"
#include "utils/utils.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char *destroy_on_fail(str_contents_ts *nb_h, char *html_str)
{
    for (int i = 0; i < 6; i++)
    {
        free(nb_h[i].nb_char_each);
    }
    free(nb_h);
    free(html_str);
    return NULL;
}

char *get_html_str(fc_control_ts *fcc)
{
    str_contents_ts *nb_h = get_nb_chars_in_headers(fcc);
    size_t html_parts_lenght = 9
            * (nb_h[0].nb + nb_h[1].nb + nb_h[2].nb + nb_h[3].nb + nb_h[4].nb
               + nb_h[5].nb)
        + 1;

    char *html_str = calloc(html_parts_lenght, sizeof(char *));
    if (html_str == NULL)
    {
        return destroy_on_fail(nb_h, NULL);
    }

    link_ctrl_ts *llc = init_llist();
    if (llc == NULL)
    {
        return destroy_on_fail(nb_h, html_str);
    }

    size_t idx = 0;
    for (int i = 0; i < NB_HEADER; i++)
    {
        for (size_t j = 0; j < nb_h[i].nb; j++)
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
    size_t *nb_h_each = calloc(NB_HEADER, sizeof(size_t));
    if (nb_h_each == NULL)
    {
        return NULL;
    }

    // Can be 0 (no header), 1, 2, 3, 4, 5 or 6
    int h_level = 0;
    file_content_ts *current_fc = fcc->head;
    char prev_c = '\0';

    for (size_t i = 0; i < fcc->nb_buffers; i++)
    {
        char *buff = current_fc->buffer;

        for (size_t idx = 0; idx < FILE_BUFF_SIZE; idx++)
        {
            // Start of a header
            if ((prev_c == '\n' || prev_c == '\0') && buff[idx] == '#')
            {
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

str_contents_ts *get_nb_chars_in_headers(fc_control_ts *fcc)
{
    // Contains the number of header for each possible category (h1 to h6)
    str_contents_ts *nb_char_each = calloc(NB_HEADER, sizeof(str_contents_ts));
    if (nb_char_each == NULL)
    {
        return NULL;
    }

    size_t *nb_h_each = get_nb_headers(fcc);
    if (nb_h_each == NULL)
    {
        free(nb_char_each);
        return NULL;
    }

    for (int i = 0; i < NB_HEADER; i++)
    {
        nb_char_each[i].nb = nb_h_each[i];
    }

    // Can be 0 (no header), 1, 2, 3, 4, 5 or 6
    int h_level = 0;
    char prev_c = '\0';
    // Contains the number of non-element chars in the current element
    // (ex: '### This is a header' contains 16 characters)
    size_t current_nb_char = 0;
    // Contains the current index of the nb_char_each field
    int indexes[NB_HEADER] = { 0 };
    file_content_ts *current_fc = fcc->head;

    for (size_t i = 0; i < fcc->nb_buffers; i++)
    {
        char *buff = current_fc->buffer;

        for (size_t idx = 0; idx < FILE_BUFF_SIZE; idx++)
        {
            // Start of a header
            if ((prev_c == '\n' || prev_c == '\0') && buff[idx] == '#')
            {
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
                    nb_char_each[h_level - 1]
                        .nb_char_each[indexes[h_level - 1]++] = current_nb_char;
                }
                h_level = 0;
                current_nb_char = 0;
            }
            else
            {
                current_nb_char++;
            }
            prev_c = buff[idx];
        }

        if (current_fc->next == NULL)
        {
            break;
        }
        current_fc = current_fc->next;
    }
    return nb_char_each;
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
