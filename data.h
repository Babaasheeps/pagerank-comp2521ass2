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
 * @brief Return the lenght of the file at the given filename
 * 
 * @param filename - file to read
 * @return long    - length of file
 */
long getFileLength(char *filename);


/**
 * @brief buffer a file into a string
 * 
 * @param filename - file to open
 * @param file_len - length of file to open
 * @return char* 
 */
char *bufferFile(char *filename, long file_len);


/**
 * @brief Tokenize the given string. That is, return an NULL termianted
 * array of strings broken by whitespace. Essentialy str.split()
 * 
 * @credit: Most of this function is starter code from COMP1521 UNSW
 * 
 * @param str -string to tokenize
 * @return char** - tokens
 */
char **tokenize(char *str);


/**
 * @brief Given a NULL-terminated array of strings, (tokens) free all
 * memory associated w/ the tokens
 * 
 * @param tokens - tokens to free
 */
void freeTokens(char **tokens);


/**
 * @brief Print out tokens
 * 
 * @param tokens - tokens to print
 * @param is_comma_seperated - if true, tokens are comma seperated, like a 
 * python string. Otherwise, each token is on its own string
 */
void printTokens(char **tokens, bool is_comma_seperated);


/**
 * @brief Given a set of tokens (null terminated array of strings)
 * return the number of tokens
 * 
 * @param tokens - tokens to count
 * @return size_t - lenght of tokens
 */
size_t countTokens(char **tokens);


/**
 * @brief Read the given file and return tokens of all outgoing links
 * 
 * @param file  - file to read
 * @return char**  - tokens of outgoingLinks
 */
char **getOutgoingLinks(char *file);


/**
 * @brief Read the given file and return tokens of all outgoing links
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