#ifndef FILES_UTILS_H
#define FILES_UTILS_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "../files_handling.h"
#include "custom_strings.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/***************************************
**     FC_CONTROL / FILE_CONTENT      **
***************************************/
/**
** \brief Allocates a file_content structure and returns it
*/
file_content_ts *init_file_content(void);

/**
** \brief Allocates a file_content control structure and returns it
*/
fc_control_ts *init_fc_control(char *file_name);

/**
** \brief Adds the element fc at the tail of fcc
*/
void add_fc_to_fc_control(fc_control_ts *fcc, file_content_ts *fc);

/***************************************
**    FILES & DIRECTORIES CREATION    **
***************************************/
/**
** \param path A path containing only directories
*/
void create_dir_if_absent(char *path);

/**
** \param path a path containing only directories
*/
void create_subdirs_if_absent(char *path);

/**
** \brief Destroys the allocated elements used when writing a file
*/
int write_file_destroy(char *path_in_dir, splited_string_ts *sps, char error);

#endif // !FILES_UTILS_H
