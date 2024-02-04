#ifndef FILES_HANDLING_H
#define FILES_HANDLING_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define FILE_BUFF_SIZE 512

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
/**
** \brief This is a link of a chained list containing the content of the file
*/
typedef struct file_content_ts
{
    char buffer[FILE_BUFF_SIZE];
    struct file_content_ts *next;
} file_content_ts;

/**
** \brief The list looks like this (head is the first element added):
**              NULL <- tail <- elt1 <- elt2 <- head
** \param nb_payloads The number of links in the chained list
*/
typedef struct fc_control_ts
{
    size_t nb_buffers;
    char *file_name;
    file_content_ts *head;
    file_content_ts *tail;
} fc_control_ts;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Reads the content of the given file if the file is valid,
**        and stores its content in a linked list of the structure file_content
** \returns The head of the file_content structure
*/
int read_file(char *path);

void print_file_content(fc_control_ts *fcc);

void destroy_fc_control(fc_control_ts *fcc);

#endif
