/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
#ifndef _PHILPHIX_UNITTEST

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Specify a dictionary\n");
        return 1;
    }
    /*
     * Allocate a hash table to store the dictionary.
     */
    fprintf(stderr, "Creating hashtable\n");
    dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

    fprintf(stderr, "Loading dictionary %s\n", argv[1]);
    readDictionary(argv[1]);
    fprintf(stderr, "Dictionary loaded\n");

    fprintf(stderr, "Processing stdin\n");
    processInput();

    /*
     * The MAIN function in C should always return 0 as a way of telling
     * whatever program invoked this that everything went OK.
     */
    return 0;
}

#endif /* _PHILPHIX_UNITTEST */

/* Task 3 */
void readDictionary(char *dictName) {
    FILE *fp = fopen(dictName, "r");
    if (fp == NULL) {
        fprintf(stderr, "file doesn't exist");
        exit(61);
    }

    char *key;
    char *data;
    while (1) {
        // TODO: Read key that only contains alphanumeric characters
        // TODO: key and data string's length can be over than 60
        key = (char *) malloc(sizeof(char) * 61);
        data = (char *) malloc(sizeof(char) * 61);
        if (fscanf(fp, "%s%s", key, data) == EOF) {
            free(key);
            free(data);
            break;
        }
        insertData(dictionary, key, data);
    }
        
    fclose(fp);
}

/* Task 4 */
int isAlphanumeric(int ch) {
    return isalpha(ch) || (ch >= '0' && ch <= '9');
}

/*
 * return true if a word has one or more upper case character.
 */
int isFirstCharacterUpperCase(char *word) {
    return isupper(word[0]);
}

/*
 * return the word starting from ch, word reading may be terminated by a
 * non-alphanumeric character, the parameter ch will get this and return
 */
char *readWordFrom(int *ch) {
    int len = 20;
    char *word = malloc(sizeof(char) * len);
    word[0] = (char) *ch;
    word[1] = '\0';

    while ((*ch = getchar()) != EOF && isAlphanumeric(*ch)) {
        unsigned int s = strlen(word);
        if (s > len * 0.75) {
            len <<= 1;
            word = realloc(word, sizeof(char) * len);
        }
        word[s] = (char) *ch;
        word[s + 1] = '\0';
    }

    return word;
}

char *getDataFrom(char *word) {
    char *word_copy = malloc(sizeof(char) * strlen(word) + 1);
    char *data;
    strcpy(word_copy, word);

    int flag = isFirstCharacterUpperCase(word_copy);

    // the exact word.
    if (flag && (data = findData(dictionary, word_copy)) != NULL) {
        free(word_copy);
        return data;
    }

    // all the word except the first character to lower case.
    if (flag) {
        size_t len = strlen(word_copy);
        for (int i = 0; i < len; i++) {
            word_copy[i] = (char) tolower(word_copy[i]);
        }
        word_copy[0] = (char) toupper(word_copy[0]);
        if ((data = findData(dictionary, word_copy)) != NULL) {
            free(word_copy);
            return data;
        }
    }

    // the word all lower case.
    size_t len = strlen(word_copy);
    for (int i = 0; i < len; i++) {
        word_copy[i] = (char) tolower(word_copy[i]);
    }
    if ((data = findData(dictionary, word_copy)) != NULL) {
        free(word_copy);
        return data;
    }

    // not found in dictionary.
    free(word_copy);
    return word;
}

void processInput() {
    int *ch;
    ch = malloc(sizeof(int));

    while ((*ch = getchar()) != EOF) {
        if (isAlphanumeric(*ch)) {
            char *word = readWordFrom(ch);
            char *data = getDataFrom(word);
            printf("%s", data);

            if (!isAlphanumeric(*ch)) {
                printf("%c", *ch);
            }

            free(word);
        } else {
            printf("%c", *ch);
        }
    }

    free(ch);
}
