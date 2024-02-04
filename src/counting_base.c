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
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
str_contents_ts **init_str_contents_array(fc_control_ts *fcc)
{
    // Contains the number of header for each category (h1 to h6), and the
    // number of character contained by each of them
    str_contents_ts **str_ctt_arr =
        calloc(NB_HEADER, sizeof(str_contents_ts *));
    if (str_ctt_arr == NULL)
    {
        return NULL;
    }

    // Allocates the six structures inside of the 'nb_char_each' array
    for (int i = 0; i < NB_HEADER; i++)
    {
        str_ctt_arr[i] = calloc(1, sizeof(str_contents_ts));
        if (str_ctt_arr[i] == NULL)
        {
            for (int j = j; j >= 0; j--)
            {
                free(str_ctt_arr[j]);
            }
            free(str_ctt_arr);
            return NULL;
        }
    }

    size_t *nb_h_each = get_nb_headers(fcc);
    if (nb_h_each == NULL)
    {
        free(str_ctt_arr);
        return NULL;
    }

    for (int i = 0; i < NB_HEADER; i++)
    {
        str_ctt_arr[i]->nb = nb_h_each[i];
        str_ctt_arr[i]->nb_char_each = calloc(nb_h_each[i], sizeof(size_t));
        if (str_ctt_arr[i]->nb_char_each == NULL)
        {
            // Frees the previously allocated arrays if the current one fails to
            // do so
            for (int j = i; j >= 0; j--)
            {
                free(str_ctt_arr[i]->nb_char_each);
            }
            return NULL;
        }
    }
    free(nb_h_each);
    return str_ctt_arr;
}

void destroy_str_contents_array(str_contents_ts **s)
{
    if (s == NULL)
    {
        return;
    }

    for (int i = 0; i < NB_HEADER; i++)
    {
        if (s[i] == NULL)
        {
            continue;
        }
        free(s[i]->nb_char_each);
        free(s[i]);
    }
    free(s);
}

char *destroy_on_fail(str_contents_ts **strct_arr, char *html_str)
{
    for (int i = 0; i < 6; i++)
    {
        free(strct_arr[i]->nb_char_each);
    }
    free(strct_arr);
    free(html_str);
    return NULL;
}

/**
** \brief Calculs the total size of the html string
*/
struct final_str_len get_final_str_len(fc_control_ts *fcc)
{
    str_contents_ts **strct_arr = get_nb_chars_in_headers(fcc);
    // We initialize at 1 because of the end of string character ('\0')
    size_t html_parts_lenght = 1;

    for (size_t i = 0; i < NB_HEADER; i++)
    {
        // Iterates over all the <hx> elements (1 <= x <= 6)
        for (size_t a = 0; a < strct_arr[i]->nb; a++)
        {
            html_parts_lenght += strct_arr[i]->nb_char_each[a];
        }
    }
    printf("headers total size : %lu\n", html_parts_lenght);
    return (struct final_str_len){ .len = html_parts_lenght,
                                   .strct_arr = strct_arr };
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char *get_html_str(fc_control_ts *fcc)
{
    struct final_str_len fsl = get_final_str_len(fcc);
    size_t html_parts_lenght = fsl.len;
    str_contents_ts **strct_arr = fsl.strct_arr;

    char *html_str = calloc(html_parts_lenght, sizeof(char *));
    if (html_str == NULL)
    {
        return destroy_on_fail(strct_arr, NULL);
    }

    link_ctrl_ts *llc = init_llist();
    if (llc == NULL)
    {
        return destroy_on_fail(strct_arr, html_str);
    }

    size_t idx = 0;
    for (int i = 0; i < NB_HEADER; i++)
    {
        for (size_t j = 0; j < strct_arr[i]->nb; j++)
        {
            string_append(html_str, "", idx);
            idx += LEN_HEADER;
        }
    }

    free(llc);
    destroy_str_contents_array(strct_arr);
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

    // Can be -1 (no header), 0, 1, 2, 3, 4 or 5
    int h_level = -1;
    char prev_c = '\0';
    file_content_ts *current_fc = fcc->head;

    for (size_t i = 0; i < fcc->nb_buffers; i++)
    {
        char *buff = current_fc->buffer;

        for (size_t idx = 0; idx < FILE_BUFF_SIZE; idx++)
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
                    nb_h_each[h_level]++;
                }
                h_level = -1;
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

str_contents_ts **get_nb_chars_in_headers(fc_control_ts *fcc)
{
    str_contents_ts **strct_arr = init_str_contents_array(fcc);
    if (strct_arr == NULL)
    {
        return NULL;
    }

    // Can be -1 (no header), 0, 1, 2, 3, 4 or 5
    int h_level = -1;
    char prev_c = '\0';
    char is_in_header = 0;
    // Contains the number of non-element chars in the current element
    // (ex: '### This is a header' contains 16 characters)
    size_t current_nb_char = 0;
    // Contains the current index of the nb_char_each field
    size_t indexes[NB_HEADER] = { 0 };
    file_content_ts *current_fc = fcc->head;

    for (size_t i = 0; i < fcc->nb_buffers; i++)
    {
        char *buff = current_fc->buffer;

        for (size_t idx = 0; idx < FILE_BUFF_SIZE; idx++)
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
            else if (h_level != -1 && prev_c == '#' && buff[idx] == ' ')
            {
                is_in_header = 1;
                // If the header has between 1 and 6 '#', we add it to the list
                // of headers
                if (0 <= h_level && h_level <= 5)
                {
                    strct_arr[h_level]->nb_char_each[indexes[h_level]] =
                        current_nb_char;
                    indexes[h_level]++;
                }
                h_level = -1;
                current_nb_char = 0;
            }
            else if (is_in_header && buff[idx] == '\n')
            {
                is_in_header = 0;
            }
            else if (is_in_header)
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
    return strct_arr;
}

void get_headers_contents(fc_control_ts *fcc, str_contents_ts **strct_arr)
{
    // Can be -1 (no header), 0, 1, 2, 3, 4 or 5
    int h_level = -1;
    char prev_c = '\0';
    char is_in_header = 0;
    // Contains the number of non-element chars in the current element
    // (ex: '### This is a header' contains 16 characters)
    size_t current_nb_char = 0;
    // Contains the current index of the nb_char_each field
    size_t indexes[NB_HEADER] = { 0 };
    file_content_ts *current_fc = fcc->head;

    for (size_t i = 0; i < fcc->nb_buffers; i++)
    {
        char *buff = current_fc->buffer;

        for (size_t idx = 0; idx < FILE_BUFF_SIZE; idx++)
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
            else if (h_level != -1 && prev_c == '#' && buff[idx] == ' ')
            {
                is_in_header = 1;
                // If the header has between 1 and 6 '#', we add it to the list
                // of headers
                if (0 <= h_level && h_level <= 5)
                {
                    strct_arr[h_level]->nb_char_each[indexes[h_level]] =
                        current_nb_char;
                    indexes[h_level]++;
                }
                h_level = -1;
                current_nb_char = 0;
            }
            else if (is_in_header && buff[idx] == '\n')
            {
                is_in_header = 0;
            }
            else if (is_in_header)
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
