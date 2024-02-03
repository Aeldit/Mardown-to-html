#ifndef COUNTING_BASE_H
#define COUNTING_BASE_H

/*******************************************************************************
**                                  includes                                  **
*******************************************************************************/
#include "files_handling.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char *get_html_str(fc_control_ts *fcc);

/**
** \brief Counts the number of headers in the given file and returns it
** \param fcc The fc_control structure associated with the file we want to
**            translate
** \returns An array of 6 int containing the number of each header
**          The array is like so : [ nb_h1, nb_h2, nb_h3, nb_h4, nb_h5, nb_h6 ]
*/
size_t *get_nb_headers(fc_control_ts *fcc);

size_t *get_nb_text_decorations(fc_control_ts *fcc);

#endif
