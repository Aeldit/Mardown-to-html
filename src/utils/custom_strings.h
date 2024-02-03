#ifndef CUSTOM_STRINGS_H
#define CUSTOM_STRINGS_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
/**
** \brief Stores a string splited based on a char separator
**
** \param splited It is a list containing each string after being splited (thes
**                strings are not null terminated to simplify operations on them
**                when building the complete path)
**
** \param nb_char_each It is a list containing the number of character contained
**                     by each string
**
** \param nb_strings It contains the number of string contained by splited
*/
typedef struct splited_string_ts
{
    char **splited;
    unsigned *nb_char_each;
    unsigned nb_strings;
} splited_string_ts;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \returns An allocated string that is the strings 'a' and 'b' concatenated
**          (ex: if a = "test" and b = "ing", the output will be "testing")
*/
char *str_concat(char *a, char *b);

/**
** \brief Splits the given string using the given separation
**        (ex : the string "tests/yes/no" separated using '/' would give use an
**         array like so : [ "tests", "yes", "no" ])
** \param s The string to split
** \param sep The character to use as separator
** \returns A splited_string structure containing the splited string, the number
**          of chars in each of these strings and the number of different
**          strings
*/
splited_string_ts *split_string(char *s, char sep);

/**
** \brief Frees the memory space allocated to the given structure and its
**        components
*/
void destroy_splited_string(splited_string_ts *sps);

/**
** \brief Prints the given splited string on the sdtout
*/
void print_splited_string(splited_string_ts *sps);

/**
** Copy every character from s into dest starting at index dest_idx
** dest will always have enough space in memory to accept s
*/
void string_append(char *dest, char *s, size_t dest_idx);

#endif // !CUSTOM_STRINGS_H
