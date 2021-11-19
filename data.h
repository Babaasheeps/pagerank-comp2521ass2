/**
 * @file data.h
 * @author Hussain Nawaz - z5361515
 * @brief Header file for `data.c`. These functions relate to data
 * management. Non-generic file-reading for specific webpage formate
 * as per spec.
 * @date November 2021
 * 
 */

#ifndef READ_DATA
#define READ_DATA

#define MAX_URL_LEN 100

/**
 * @brief Read a collectiosn file and return tokens of all the urls
 * int the given file
 * 
 * @param filename  - file to read
 * @return char**   - array of strings containing urls
 */
char **getLinkCollection(char *filename);

/**
 * @brief Read the given file and return tokens of all outgoing links
 * 
 * @param file  - file to read
 * @return char**  - tokens of outgoingLinks
 */
char **getOutgoingLinks(char *file);


/**
 * @brief Read e given file and return tokens of all outgoing links
 * 
 * @param filename - file to read
 * @return char** - tokens out outgoing links
 */
char **doReadOutgoingLinks(char *filename);

/**
 * @brief Create a string, initialised all as '\0'. May seem like benign
 * abstraction but, rationale is:
 *         - If you just malloc an empty string and don't initialise
 *         its values to anything, then `strcpy` and `strcat` will have 
 *         mixed presets behaviours depending on OS / compiler.
 *          - Clang suppresses the issue and is smart enough to work around.
 *          - gcc will have mixed behaviours. If run through valgrind,
 *          valgrind can supress err and run file till end but otherwise,
 *          esp if compiled w/ gdb flag, there may be a SIGABRT that is
 *          triggered.
 * TLDR: C string library functions are (ostensibly) poorly standardise
 * 
 * @param n - size of string to create
 * @return char* 
 */
char *createString(int n);

#endif // !READ_DATA