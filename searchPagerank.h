/**
 * @file searchPagerank.h
 * @author Hussain Nawaz (z5361515)
 * @brief Header file for `searchPagerank.c`. To implement part-2.
 * Allows ranking urls by searchterm.
 * @date November 2021
 * 
 */
#ifndef SEARCH_PAGERANK_H
#define SEARCH_PAGERANK_H


/**
 * @brief Struct to hold data relating to url for current search.
 * 
 */
typedef struct url_data
{
    char *url;                  // url name as string
    long out_degree;            // number of outgoing links to other urls
    double weighted_rank;       // weighted page rank as per part1
    long matching_searches;     // # of search terms that are on this page
} url_data;
typedef struct url_data *UrlData;


/**
 * @brief Malloc a url_data object with the fields as specified
 * 
 * @param url               - url
 * @param out_degree        - out_degree
 * @param weight            - weighted_rank
 * @param matches           - matching_searches
 * @return UrlData  - Object made
 */
UrlData createUrlData(char *url, long out_degree, double weight, long matches);

/**
 * @brief - Open a file where each line contains data formatted as
 * <url>, <out_degree>, <weight>.
 * Initialease the above data into an array of UrlData objects
 * 
 * @param filename - File to read the data from
 * @return UrlData* - Array of UrlData objs
 */
UrlData *initUrlRank(char *filename);


/**
 * @brief Creates a copy of a string by value.
 * 
 * @param str  - string to copy
 * @return char* - copy of string
 */
char *copyString(char *str);


/**
 * @brief From a given array of UrlData, print its fields comma-seperated.
 * One url oject per line.
 * 
 * @param urls - Array to print from
 */
void printUrlData(UrlData *urls);


/**
 * @brief Free all memory associated with the given UrlData object
 * 
 * @param data - object to free
 */
void freeUrlData(UrlData data);


/**
 * @brief Free all memory associated with the given NULL-terminated
 * array of UrlData objects
 * 
 * @param data - array to free
 */
void freeUrlDataArray(UrlData *data);


/**
 * @brief For all the search terms in `argv`, find all the urls that
 * contain that search term and increment the `matching_searches` field
 * for the respective url's entry in UrlData
 * 
 * @param index  - A matrix of strings where element [i][0] is the search
 * term for all i. The rest of the entries on the given row are all the
 * urls that contain that search term
 * @param data   - UrlData object. Where url data to increment is stored
 * @param argc   - Number of search terms
 * @param argv   - Contains command line arguments. Slot `0` will be the 
 * program name. The rest are terms to search for
 */
void addSearchMatchesToData(char ***index, UrlData *data, int argc, char *argv[]);


/**
 * @brief Find the index of the given url from an array of UrlData objects
 * 
 * @param data  - UrlData object array
 * @param url   - url to find. 
 * @return int  - Index of url if found. -1 if url does not exist
 */
int findUrlIndex(UrlData *data, char *url);


/**
 * @brief Given a target word, find it's position in the given index of
 * strings. Note: the search terms will all be in the first column
 * 
 * @param index  - matrix of string to look through for target
 * @param target - what string to look for
 * @return int   - Row of the target string. -1 if not found
 */
int findWordSearchIndex(char ***index, char *target);


/**
 * @brief Given an array of url data, run a bubble-sort w/ the given
 * comparison function
 * 
 * @param data  - array to sort
 * @param n     - number of elements in the array
 * @param compare  - Function pointer. Must be able to take in two
 * url-data objects and then return an int after comparing them in some
 * manner.
 */
void sortUrlData(UrlData *data, int n, int (*compare)(UrlData, UrlData));


/**
 * @brief Given an array of UrlData objects, swap two elements in the 
 * array at the given indexes
 * 
 * @param data - array of UrlData objects
 * @param i    - index to swap
 * @param j    - indiex to swap
 */
void swapUrlData(UrlData *data, int i, int j);

/**
 * @brief Compare urls by the number of matches they have. 
 * 
 * @param a     - url to compare
 * @param b     - url to compare
 * @return int - true if a < b
 */
int compareUrlMatches(UrlData a, UrlData b);


/**
 * @brief Compare urls by the number of matches they have. Aims for
 * a descending order
 * 
 * @param a     - url to compare
 * @param b     - url to compare
 * @return int - true if a > b
 */
int compareUrlWeights(UrlData a, UrlData b);


/**
 * @brief compare urls by their name. Does comparison w/ strcmp
 * 
 * @param a     - url to compare
 * @param b     - url to compare
 * @return int  - true if strcmp result > 0
 */
int compareUrlNames(UrlData a, UrlData b);


/**
 * @brief Given an array of UrlData objects sorted by number of matching
 * terms, print out the names of up to the 30 top objects that have
 * atleast one match
 * 
 * @param data  - sorted array of UrlDat objects to print from
 */
void printUrlMatchingOnly(UrlData *data);


#endif // !SEARCH_PAGERANK_H