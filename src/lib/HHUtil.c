#include "HHUtil.h"

char *trim(const char *str)
{
    int len = strlen(str);
    int start = -1;
    int end = len;

    // Find the start index of non-whitespace characters
    while (isspace(str[++start]))
        ;

    // Find the end index of non-whitespace characters
    while (--end >= start && isspace(str[end]))
        ;

    // Calculate the length of the trimmed string
    int trimmedLen = end - start + 1;

    // Allocate memory for the new trimmed string
    char *trimmedStr = malloc(trimmedLen + 1); // +1 for null terminator

    // Copy the trimmed characters to the new string
    strncpy(trimmedStr, str + start, trimmedLen);

    // Null-terminate the new string
    trimmedStr[trimmedLen] = '\0';

    return trimmedStr;
}

char **delimit(char *input, char *delimiter, int *num_items)
{
    char *token;
    char **items = malloc(32 * sizeof(char *)); // Assuming a maximum of 32 items
    *num_items = 0;

    // Set default `delimiter` if necessary
    delimiter = strlen(delimiter) != 0 ? delimiter : ",";

    // Start tokenizing the input...
    token = strtok(input, delimiter);
    while (token != NULL) // NOLINT
    {
        token = trim(token);
        items[*num_items] = malloc(strlen(token) + 1);
        strcpy(items[*num_items], token);
        (*num_items)++;

        // Continue tokenizing (aka, use `NULL` to keep iterating `input` string)
        token = strtok(NULL, delimiter);
    }

    return items;
}

NSHHUtil HHUtil = {
    .delimit = delimit,
    .trim = trim
    //
};