#ifndef COUNTING_BASE_H
#define COUNTING_BASE_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

#include "files_handling.h"

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct str_contents_ts
{
    size_t nb; // The number of headers of the current type
    size_t *nb_char_each; // The number of characters in each of them
} str_contents_ts;

struct final_str_len
{
    size_t len;
    str_contents_ts **strct_arr;
};

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char *get_html_str(void);

/**
** \brief Counts the number of headers in the given file and returns it
** \param fcc The fc_control structure associated with the file we want to
**            translate
** \returns An array of 6 int containing the number of each header
**          The array is like so : [ nb_h1, nb_h2, nb_h3, nb_h4, nb_h5, nb_h6 ]
*/
size_t *get_nb_headers(void);

/**
** \brief Counts the number of characters that each header element contains
**        (ex: "### Testing" has 7 characters)
** \returns An array of 6 str_contents_ts structures each containing the number
**          of character contained by head header element
*/
str_contents_ts **get_nb_chars_in_headers(void);

void get_headers_contents(str_contents_ts **strct_arr);

#endif
