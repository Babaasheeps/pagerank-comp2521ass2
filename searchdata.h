#ifndef SEARCHDATA_H
#define SEARCHDATA_H
 
char **fileToLineTokens(char *filename);
long fileNumLines(char *filename);
long getFileLength(char *filename);
char *bufferFile(char *filename, long file_len, bool alnum_only);
char **tokenize (char *str, char *sep);
char ***reTokenize(char **tokens, char *sep);
void printTokens(char **tokens, bool is_comma_seperated);
size_t countTokens(char **tokens);

#endif // !SEARCHDATA_H