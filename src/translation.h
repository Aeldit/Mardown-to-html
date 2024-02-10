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
#define TYPE_NONE 0
#define TYPE_BOLD 1
#define TYPE_ITALIC 2
#define TYPE_INLINE_CODE 3

//==============================================================================
// Indexes in the elements array (in the source file)
#define IDX_BOLD 6
#define IDX_ITALIC 7
#define IDX_INLINE_CODE 8

//==============================================================================
// Number of characters in each html element
#define NB_C_H_START 4
#define NB_C_H_END 5

#define NB_C_BOLD_START 33
#define NB_C_ITALIC_START 34
#define NB_C_INLINE_CODE_START 75
#define NB_C_SPAN_CODE_END 7

//==============================================================================
// Total length of each html element
#define LEN_HEADER ((NB_C_H_START) + (NB_C_H_END)) // = 9
#define LEN_BOLD ((NB_C_BOLD_START) + (NB_C_SPAN_CODE_END)) // = 40
#define LEN_ITALIC ((NB_C_ITALIC_START) + (NB_C_SPAN_CODE_END)) // = 41
// = 82
#define LEN_INLINE_CODE ((NB_C_INLINE_CODE_START) + (NB_C_SPAN_CODE_END))

//==============================================================================
// MACROS
//==============================================================================
// 'elt' must be a member of the ELEMENTS enum
#define IS_IN_HEADER(elt)                                                      \
    ((elt) == (H1) || (elt) == (H2) || (elt) == (H3) || (elt) == (H4)          \
     || (elt) == (H5) || (elt) == (H6))

// 'elt' must be a member of the ELEMENTS enum
#define IS_IN_DECO(elt)                                                        \
    (((elt) == BOLD) || ((elt) == ITALIC) || ((elt) == INLINE_CODE))

// Obtains the total number of chars from the text decorations
// 'deco' is a decorations_ts structure
#define GET_NB_CHAR_FROM_DECOS(deco)                                           \
    (((deco.nb_bold) * (LEN_BOLD)) + ((deco.nb_italic) * (LEN_ITALIC))         \
     + ((deco.nb_inline_code) * (LEN_INLINE_CODE)))

/*******************************************************************************
**                                   ENUMS                                    **
*******************************************************************************/
enum ELEMENTS
{
    NO_ELT,
    H1,
    H2,
    H3,
    H4,
    H5,
    H6,
    BOLD,
    ITALIC,
    INLINE_CODE,
};

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
