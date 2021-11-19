/**
 * @file data.h
 * @author Hussain Nawaz - z5361515
 * @brief Implementation file for `data.c`. These functions relate to data
 * management. Non-generic file-reading for specific webpage formate
 * as per spec.
 * @date November 2021
 * 
 */
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utility.h"
#include "tokens.h"
#include "data.h"


char **getLinkCollection(char *filename)
{
    // Get length of file to know how much space to allocate in buffer
    long file_length = getFileLength(filename);
    char *buffer = bufferFile(filename, file_length, true);
    char **urls = tokenize(buffer, " \n");
    free(buffer);
    return urls;
}


char **getOutgoingLinks(char *file)
{
    // Add ".txt" to file name. Create space for extension and null 
    char *filename = createString(MAX_URL_LEN + 6);
    strcpy(filename, file);
    char *file_ext = ".txt";
    strcat(filename, file_ext);

    // Open the file and, tokenise all outgoing urls
    char **outgoing_urls = doReadOutgoingLinks(filename);
    free(filename);
    return outgoing_urls;
}

char **doReadOutgoingLinks(char *filename)
{
    // Create space to be able to buffer the file 
    long file_length = getFileLength(filename);
    char *file_buffer = createString(file_length + 1);

    FILE *f = fopen(filename, "r");
    assert(f != NULL);
    
    // Can ignore opening line.
    char *opening_line = createString(file_length + 1);
    opening_line = fgets(opening_line, file_length, f);
    free(opening_line);

    // Read input till the "#" to signify end of section 1 is reached.
    for (int i = 0; true; i++)
    {
        int c = fgetc(f);
        if (c == EOF || c == '#')
        {
            file_buffer[i] = '\0';
            break;
        }
        file_buffer[i] = (isalnum(c)) ? c : ' ';
    }
    // Turn the buffer holding `section 1` into tokens w/ url names
    char **urls = tokenize(file_buffer, " \n");
    // Free buffer and close file 
    fclose(f);
    free(file_buffer);
    return urls;
}


char *createString(int n)
{
    char *s = malloc(sizeof(char) * n);
    assert(s != NULL);
    for (int i = 0; i < n; i++)
        s[0] = '\0';
    return s;
}
