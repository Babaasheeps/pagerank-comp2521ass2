#ifndef SEARCHDATA_H
#define SEARCHDATA_H
 
void freeTokens(char **tokens);
void freeTokensArray(char ***tokens);
void printTokens(char **tokens, bool is_comma_seperated);
char **tokenize (char *str, char *sep);
char ***reTokenize(char **tokens, char *sep);
char **fileToLineTokens(char *filename);
long fileNumLines(char *filename);
long getFileLength(char *filename);
char *bufferFile(char *filename, long file_len, bool alnum_only);
size_t countTokens(char **tokens);


#endif // !SEARCHDATA_H