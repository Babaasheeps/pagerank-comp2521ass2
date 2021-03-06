/**
 * @file file_utility.h
 * @author Hussain Nawaz - (z5361515)
 * @brief Header file for `file_utility.c`. These are general purpose
 * utility functions for file(s) reading and, sanitisation of file data.
 * @date November 2021
 * 
 */
#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "tokens.h"

/**
 * @brief Open the file at `filename`. Load it into a buffer and then return
 * a version of the buffer tokenised at newlines
 * 
 * @param filename - file to open
 * @return char**   - The tokenised contents at filename
 */
char **fileToLineTokens(char *filename);


/**
 * @brief Returns the number of lines of a given file (counted by number of 
 * '\n' characters)
 * 
 * @param filename  - file to read
 * @return long     - number of lines
 */
long fileNumLines(char *filename);


/**
 * @brief Get the length of a given file in bytes
 * 
 * @param filename  - file to read
 * @return long     - length of file
 */
long getFileLength(char *filename);


/**
 * @brief Load the given number of bytes from a file into a buffer and return
 * that buffer.
 * 
 * @param filename  - file to read
 * @param file_len  - Length of file // Upper bound on bytes to read
 * @param alnum_only  - Whether to store alpanumeric bytes. If true, all non-
 * alphanumeric characters are replaces with whitespace: ' ';
 * @return char*    - Contents of the file, as a string
 */
char *bufferFile(char *filename, long file_len, bool alnum_only);


/**
 * @brief Reads multiple files to create a matrix of strings where, each
 * row contains tokenized contents of a single file.
 * 
 * @param num_files     - Number of files to read
 * @param files         - paths of files to open
 * @return char***      - matrix of strings containing file data
 */
char ***filesToTokenMatrix(int num_files, char *files[]);


#endif // !FILE_UTILITY_H