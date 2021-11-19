// COMP2521 Assignment 2

// Written by: Hussain Nawaz (z5361515)
// Date: November 2021

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "searchPagerank.h"
#include "tokens.h"
#include "file_utility.h"


int main(int argc, char *argv[])
{
    // Read inverted index - line_tokens is a set of tokens where each index
    // is a string represeting a line
    char **line_tokens = fileToLineTokens("invertedIndex.txt");
    // We want to retokenize to a 2d array of strings so, for index[i][j]
    // `i` allows access to a particular line ands `j` allows for words in that
    // lines to be accesssed. [i][0] will represent search terms
    char ***index = reTokenize(line_tokens, " ");

    // Build data for each url
    UrlData *data = initUrlRank("pagerankList.txt");
    int num_urls = (int)fileNumLines("pagerankList.txt");
    addSearchMatchesToData(index, data, argc, argv);
    
    // Sort: Precedence: #matches, weighting, name
    sortUrlData(data, num_urls, compareUrlNames);
    sortUrlData(data, num_urls, compareUrlWeights);
    sortUrlData(data, num_urls, compareUrlMatches);
    
    printUrlMatchingOnly(data);
    
    // Free associated memory
    freeTokens(line_tokens);
    freeTokensArray(index);
    freeUrlDataArray(data);
    return 0;    
}

void addSearchMatchesToData(char ***index, UrlData *data, int argc, char *argv[])
{
    // For all search terms, find the urls that contain that term and
    // increment their num_matches
    for (int i = 1; i < argc; i++)
    {
        // Find the index of the current search term in data
        int src_idx = findWordSearchIndex(index, argv[i]);
        // For all urls that hold the current term, incrememnt their matches found
        for (int j = 1; index[src_idx][j] != NULL; j++)
        {
            int dest_idx = findUrlIndex(data, index[src_idx][j]);
            data[dest_idx]->matching_searches += 1;
        }
    }
}

int findUrlIndex(UrlData *data, char *url)
{
    for (int i = 0; data[i] != NULL; i++)
    {
        if (!strcmp(url, data[i]->url))
            return i;
    }
    assert(false);
    return -1; // Will never be reached since assumed input valid
}


int findWordSearchIndex(char ***index, char *target)
{
    for (int i = 0; index[i] != NULL; i++)
    {
        if (!strcmp(index[i][0], target))
            return i;
    }
    return -1;
}



void freeUrlDataArray(UrlData *data)
{
    for (int i = 0; data[i] != NULL; i++)
        freeUrlData(data[i]);
    free(data);
}

void freeUrlData(UrlData data)
{
    free(data->url);
    free(data);
}


UrlData *initUrlRank(char *filename)
{
    // We want to open the file, then create a token for each line.
    char **line_tokens = fileToLineTokens(filename);
    // Then, we can retokenise so each field of data is its own token such that
    // for all `i`: raw_data[i][0] = str(URL), raw_data[i][1] = str(out_degree),
    // raw_data[i][2] = str(weighted page rank)
    char ***raw_data = reTokenize(line_tokens, " ,");

    // From tokenised data, create urls
    int num_urls = (int)countTokens(line_tokens);
    UrlData *urls = malloc(sizeof(*urls) * (num_urls + 1));
    
    for (int i = 0; i < num_urls; i++)
    {
        // Convert strings to correct types for url_data fields
        // No error checking. Assume all inputs are valid
        char *url = raw_data[i][0];
        long out_degree = strtol(raw_data[i][1], NULL, 10);
        double weight = strtod(raw_data[i][2], NULL);
        urls[i] = createUrlData(url, out_degree, weight, 0);
    }
    // Null terminate the array
    urls[num_urls] = NULL;

    // Free associated tokens
    freeTokens(line_tokens);
    freeTokensArray(raw_data);

    return urls;
}

UrlData createUrlData(char *url, long out_degree, double weight, long matches)
{
    UrlData data = malloc(sizeof(*data));
    assert (data != NULL);
    
    data->url = copyString(url);
    data->matching_searches = matches;
    data->out_degree = out_degree;
    data->weighted_rank = weight;

    return data;
}



void printUrlData(UrlData *urls)
{
    for (int i = 0; urls[i] != NULL; i++)
    {
        printf("%s: %ld, %lf, %ld\n",
                urls[i]->url,
                urls[i]->out_degree,
                urls[i]-> weighted_rank,
                urls[i]->matching_searches);
    }
}


void sortUrlData(UrlData *data, int n, int (*compare)(UrlData, UrlData))
{
    bool is_sorted = false;
    while (!is_sorted)
    {
        is_sorted = true;
        for (int i = 0; i < n - 1; i++)
        {
            if ((*compare)(data[i], data[i + 1]))
            {
                // TODO: add swap
                swapUrlData(data, i, i + 1);
                is_sorted = false;
            }
        }
    }
}

void swapUrlData(UrlData *data, int i, int j)
{
    UrlData tmp = data[i];
    data[i] = data[j];
    data[j] = tmp;
}

int compareUrlMatches(UrlData a, UrlData b)
{
    return a->matching_searches < b->matching_searches;
}

int compareUrlWeights(UrlData a, UrlData b)
{
    return (a->weighted_rank > b->weighted_rank);
}

int compareUrlNames(UrlData a, UrlData b)
{
    return strcmp(a->url, b->url) > 0;
}

void printUrlMatchingOnly(UrlData *data)
{
    for (int i = 0; data[i] != NULL && i < 30; i++)
    {
        if (!data[i]->matching_searches)
            return;
        printf("%s\n", data[i]->url);
    }
}

