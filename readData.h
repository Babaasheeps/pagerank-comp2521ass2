#ifndef READ_DATA
#define READ_DATA

char **getLinkCollection(char *filename);
long getFileLength(char *filename);
char *fileToString(char *filename, long file_len);
char **tokenize(char *str);
void freeTokens(char **tokens);
void printTokens(char **tokens, bool is_comma_seperated);
#endif // !READ_DATA