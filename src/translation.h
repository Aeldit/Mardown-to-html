#ifndef COUNTING_BASE_H
#define COUNTING_BASE_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>
#include <stdio.h>

#include "utils/utils.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define NB_CHAR_H_START 4
#define NB_CHAR_H_END 5

// 'elt' must be a member of the ELEMENTS enum
#define IS_IN_HEADER(elt)                                                      \
    ((elt) == (H1) || (elt) == (H2) || (elt) == (H3) || (elt) == (H4)          \
     || (elt) == (H5) || (elt) == (H6))

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void translate_write_to_html(char buff[BUFF_SIZE], enum ELEMENTS elt,
                             FILE *out_f);

#endif
