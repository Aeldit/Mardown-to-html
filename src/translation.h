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

#define TYPE_NONE 0
#define TYPE_BOLD 1
#define TYPE_ITALIC 2
#define TYPE_INLINE_CODE 3

#define LEN_HEADER 9 // <hx></hx>
#define LEN_BOLD 40 // <span style="font-weight: bold;"></span>
#define LEN_ITALIC 41 // <span style="font-style: italic;"></span>
// <code style="background-color: #444444; border-radius: 8px; padding:
// 2px;"></code>
#define LEN_INLINE_CODE 82

// 'elt' must be a member of the ELEMENTS enum
#define IS_IN_HEADER(elt)                                                      \
    ((elt) == (H1) || (elt) == (H2) || (elt) == (H3) || (elt) == (H4)          \
     || (elt) == (H5) || (elt) == (H6))

// Obtains the total number of chars from the text decorations
#define GET_NB_CHAR_FROM_DECOS(deco)                                           \
    (((deco.nb_bold) * (LEN_BOLD)) + ((deco.nb_italic) * (LEN_ITALIC))         \
     + ((deco.nb_inline_code) * (LEN_INLINE_CODE)))

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct decorations_ts
{
    size_t nb_bold;
    size_t nb_italic;
    size_t nb_inline_code;
} decorations_ts;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void translate_write_to_html(char buff[BUFF_SIZE], enum ELEMENTS elt,
                             FILE *out_f);

#endif
