#include "translation.h"

#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
**                                  CONSTANTS                                 **
*******************************************************************************/
const char *elements[][5] = {
    { "<h1>", "</h1>" },
    { "<h2>", "</h2>" },
    { "<h3>", "</h3>" },
    { "<h4>", "</h4>" },
    { "<h5>", "</h5>" },
    { "<h6>", "</h6>" },
    { "<span style=\"font-weight: bold;\">", "</span>" },
    { "<span style=\"font-style: italic;\">", "</span>" },
    { "<code style=\"background-color: #444444; border-radius: 8px; padding: "
      "2px;\">",
      "</code>" }
};

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
size_t get_nb_headers(char buff[BUFF_SIZE])
{
    // Can be -1 (no header), 0, 1, 2, 3, 4 or 5
    char h_level = -1;
    char prev_c = '\0';
    size_t nb_h = 0;

    for (size_t i = 0; i < BUFF_SIZE; i++)
    {
        if (buff[i] == '#')
        {
            // Start of a header
            if (prev_c == '\0' || prev_c == '\n')
            {
                h_level = 0;
            }
            // Gets the number of '#' that are for the same header
            else
            {
                h_level++;
            }
        }
        // When the header stops
        else if (h_level != -1 && buff[i] != '#')
        {
            // If the header has between 1 and 6 '#', we add it to the list
            // of headers
            if (buff[i] == ' ' && 0 <= h_level && h_level <= 5)
            {
                nb_h++;
            }
            h_level = -1;
        }
        prev_c = buff[i];
    }
    return nb_h;
}

decorations_ts get_nb_decorations(char buff[BUFF_SIZE])
{
    decorations_ts res =
        (decorations_ts){ .nb_bold = 0, .nb_italic = 0, .nb_inline_code = 0 };

    // 0 => None | 1 => bold | 2 => italic | 3 => code
    // See the 'TYPE_...' defines in the header
    char current_type = 0;
    size_t nb_stars = 0;

    for (size_t i = 0; i < BUFF_SIZE; i++)
    {
        // Bold or italic
        if (buff[i] == '*')
        {
            nb_stars++;
        }
        // Even number of stars => 1 bold
        // Odd number of stars < 2 => 1 italic
        // Odd number of stars > 2 => 1 bold & 1 italilc
        else if (nb_stars != 0)
        {
            if (nb_stars % 2 == 0)
            {
                res.nb_bold++;
            }
            else
            {
                res.nb_italic++;
                if (nb_stars > 2)
                {
                    res.nb_bold++;
                }
            }
            nb_stars = 0;
        }
        // Inline code
        else if (buff[i] == '`')
        {
            if (current_type == TYPE_INLINE_CODE)
            {
                current_type = TYPE_NONE;
                res.nb_inline_code++;
            }
            else
            {
                current_type = TYPE_INLINE_CODE;
            }
        }
    }
    res.nb_bold /= 2;
    res.nb_italic /= 2;
    return res;
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void translate_write_to_html(char buff[BUFF_SIZE], enum ELEMENTS elt,
                             FILE *out_f)
{
    // TODO -> Use a structure of structures to store the number of stars from a
    // buffer to another Can be -1 (no header), 0, 1, 2, 3, 4 or 5
    int h_level = -1;
    char prev_c = '\0';
    char is_in_header = IS_IN_HEADER(elt);
    char is_in_bold = elt == BOLD;
    char is_in_italic = elt == ITALIC;
    int html_idx = 0;

    // 0 => None | 1 => bold | 2 => italic | 3 => code
    // See the 'TYPE_...' defines in the header
    char current_deco_type = 0;
    size_t nb_stars = 0;
    size_t nb_stars_end = 0;

    decorations_ts nb_deco = get_nb_decorations(buff);

    // 5 is because we may have started a header in the previous buffer
    char *html_buff = calloc(5 + LEN_HEADER * get_nb_headers(buff)
                                 + GET_NB_CHAR_FROM_DECOS(nb_deco) + BUFF_SIZE,
                             sizeof(char *));

    for (size_t i = 0; i < BUFF_SIZE; i++)
    {
        // TODO -> use a function that returns the current element
        //======================================================================
        // Headers
        //======================================================================
        if (buff[i] == '#')
        {
            // Start of a header
            if (prev_c == '\0' || prev_c == '\n' || h_level == -1)
            {
                h_level = 0;
            }
            // Gets the number of '#' that are for the same header
            else
            {
                h_level++;
            }
        }
        // When the header stops
        else if (h_level != -1 && buff[i] == ' ' && !is_in_header)
        {
            is_in_header = 1;
            // Adds the '<hx>' element start to the dest_buff
            for (int j = 0; j < NB_C_H_START; j++)
            {
                html_buff[html_idx++] = elements[h_level][0][j];
            }
        }
        else if (is_in_header)
        {
            if (buff[i] == '\n')
            {
                // Adds the '</hx>' element end to the dest_buff
                for (int j = 0; j < NB_C_H_END; j++)
                {
                    html_buff[html_idx++] = elements[h_level][1][j];
                }
                h_level = -1;
                is_in_header = 0;
            }
            else
            {
                html_buff[html_idx++] = buff[i];
            }
        }
        //======================================================================
        // Bold or italic
        //======================================================================
        else if (buff[i] == '*')
        {
            nb_stars++;
        }
        // Even number of stars => 1 bold
        // Odd number of stars < 2 => 1 italic
        // Odd number of stars > 2 => 1 bold & 1 italic
        //
        // Writes the element start
        else if (nb_stars != 0)
        {
            // Bold
            if (nb_stars % 2 == 0)
            {
                is_in_bold = 1;
                for (size_t i = 0; i < NB_C_BOLD_START; i++)
                {
                    html_buff[html_idx++] = elements[IDX_BOLD][0][i];
                }
            }
            // Italic
            else
            {
                for (size_t i = 0; i < NB_C_ITALIC_START; i++)
                {
                    html_buff[html_idx++] = elements[IDX_ITALIC][0][i];
                }
                is_in_italic = 1;

                if (nb_stars > 2)
                {
                    for (size_t i = 0; i < NB_C_SPAN_CODE_END; i++)
                    {
                        html_buff[html_idx++] = elements[IDX_BOLD][1][i];
                    }
                    is_in_bold = 1;
                }
            }
            nb_stars_end = nb_stars;
            nb_stars = 0;
        }
        // Writes the element end
        else if ((is_in_bold || is_in_italic) && nb_stars == 0)
        {
            if (buff[i] == '*')
            {
                // Bold end
                if (nb_stars_end % 2 == 0)
                {
                    for (size_t i = 0; i < NB_C_SPAN_CODE_END; i++)
                    {
                        html_buff[html_idx++] = elements[IDX_BOLD][1][i];
                    }
                    is_in_bold = 0;
                    nb_stars_end -= 2;
                }
                else
                {
                    // Bold end
                    if (nb_stars > 2)
                    {
                        for (size_t i = 0; i < NB_C_SPAN_CODE_END; i++)
                        {
                            html_buff[html_idx++] = elements[IDX_BOLD][1][i];
                        }
                        is_in_bold = 0;
                        nb_stars_end -= 2;
                    }

                    // Italic end
                    for (size_t i = 0; i < NB_C_SPAN_CODE_END; i++)
                    {
                        html_buff[html_idx++] = elements[IDX_ITALIC][1][i];
                    }
                    is_in_italic = 0;
                    nb_stars_end--;
                }
            }
            html_buff[html_idx++] = buff[i];
        }
        // Inline code
        else if (buff[i] == '`')
        {
            if (current_deco_type == TYPE_INLINE_CODE)
            {
                current_deco_type = TYPE_NONE;
                // add chars
            }
            else
            {
                current_deco_type = TYPE_INLINE_CODE;
            }
        }
        else
        {
            html_buff[html_idx++] = buff[i];
        }
        prev_c = buff[i];
    }
    fprintf(out_f, html_buff, NULL);
    free(html_buff);
}
