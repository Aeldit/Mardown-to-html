#ifndef ELTS_TYPE_LLIST_H
#define ELTS_TYPE_LLIST_H

/*******************************************************************************
**                                  includes                                  **
*******************************************************************************/
#include <stddef.h>

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct link_ts
{
    unsigned type;
    char *contents;
    size_t contents_len;
    struct link_ts *next;
} link_ts;

typedef struct link_ctrl_ts
{
    size_t nb_links;
    link_ts *head;
} link_ctrl_ts;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
link_ctrl_ts *init_llist();

void add_elt(link_ctrl_ts *llc, unsigned type, char *contents,
             size_t contents_len);

void destroy_llc(link_ctrl_ts *llc);

#endif
