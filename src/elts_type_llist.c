#include "elts_type_llist.h"

/*******************************************************************************
**                                  includes                                  **
*******************************************************************************/
#include <stdlib.h>

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
link_ctrl_ts *init_llist()
{
    link_ctrl_ts *llc = calloc(1, sizeof(link_ctrl_ts));
    if (llc == NULL)
    {
        return NULL;
    }

    llc->nb_links = 0;
    llc->head = NULL;
    return llc;
}

void add_elt(link_ctrl_ts *llc, unsigned type, char *contents,
             size_t contents_len)
{
    if (llc == NULL)
    {
        return;
    }

    link_ts *link = calloc(1, sizeof(link_ts));
    if (link == NULL)
    {
        return;
    }

    link->type = type;
    link->contents = contents;
    link->contents_len = contents_len;
    link->next = llc->head;

    llc->head = link;
    llc->nb_links++;
}

void destroy_llc(link_ctrl_ts *llc)
{
    if (llc == NULL)
    {
        return;
    }

    link_ts *link = llc->head;
    while (link->next != NULL)
    {
        link_ts *tmp = link;
        link = link->next;
        free(tmp->contents);
        free(tmp);
    }
    free(llc);
}
