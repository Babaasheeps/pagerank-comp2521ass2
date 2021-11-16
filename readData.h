#ifndef READ_DATA
#define READ_DATA

#define MAX_URL_LEN

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
char *fileToString(char *filename, long file_len);


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
char **outgoingLinks(char *file);


/**
 * @brief Read the given file and return tokens of all outgoing links
 * 
 * @param filename - file to read
 * @return char** - tokens out outgoing links
 */
char **doReadOutgoingLinks(char *filename);


#endif // !READ_DATA