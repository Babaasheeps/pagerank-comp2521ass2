#ifndef READ_DATA
#define READ_DATA

#define MAX_URL_LEN

char **getLinkCollection(char *filename);
long getFileLength(char *filename);
char *fileToString(char *filename, long file_len);
char **tokenize(char *str);
void freeTokens(char **tokens);
void printTokens(char **tokens, bool is_comma_seperated);
size_t countTokens(char **tokens);
char **outgoingLinks(char *file);
char **doReadOutgoingLinks(char *filename);


#endif // !READ_DATA