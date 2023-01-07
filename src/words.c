#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "extratypes.h"
#include "extrafuns.h"

extern int errno;

void write_word(char** crossword, Word node, char* word) {
    if (node.orientation) { /* Vertical */
        for (int i = node.begin, j = 0 ; i <= node.end ; i++, j++) {
            crossword[i][node.constant] = word[j];
        }
    }
    else { /* Horizontal */
        for (int i = node.begin, j = 0 ; i <= node.end ; i++, j++) {
            crossword[node.constant][i] = word[j];
        }
    }
}

void delete_word(char** crossword, Word node, char* word) {
    if (node.orientation) { /* Vertical */
        for (int i = node.begin, j = 0 ; i <= node.end ; i++, j++) {
            if (word[j] != '?') crossword[i][node.constant] = '-';
        }
    }
    else { /* Horizontal */
        for (int i = node.begin, j = 0 ; i <= node.end ; i++, j++) {
            if (word[j] != '?') crossword[node.constant][i] = '-';
        }
    }
}

//TODO change to actual values
void print_words(Wordnode words, int wordnode_count) {
    for (int i = 0 ; i < wordnode_count ; ++i) {
        if (words[i].orientation) {
            printf("%d. col: %d begin: %d end: %d score: %d\n", i + 1, words[i].constant, words[i].begin, words[i].end, words[i].score);
        }
        else {
            printf("%d. row: %d begin: %d end: %d score: %d\n", i + 1, words[i].constant, words[i].begin, words[i].end, words[i].score);
        }
    }
}

char* word_written(char* word, char* filter) {
    int size = strlen(word);
    char* written = malloc((size + 1) * sizeof(char));
    for (int i = 0 ; i < size ; i++) {
        written[i] = filter[i] == '?' ? word[i] : '?';
    }
    written[size] = '\0';
    return written;
}

Wordnode map_words(char** crossword, int crossword_size, int* wordnode_count) {
    int hor_size = 0;
    int ver_size = 0;
    int ver_count = 0;
    int hor_count = 0;
    for (int i = 0 ; i < crossword_size ; i++) {
        for (int j = 0 ; j < crossword_size ; j++) {
            if (crossword[i][j] != '#') hor_size++;
            if (crossword[i][j] == '#') {
                if (hor_size > 1) hor_count++;
                hor_size = 0;
            }
            if (crossword[j][i] != '#') ver_size++;
            if (crossword[j][i] == '#') {
                if (ver_size > 1) ver_count++;
                ver_size = 0;
            }
        }
        if (hor_size > 1) hor_count++;
        if (ver_size > 1) ver_count++;
        hor_size = 0;
        ver_size = 0;
    }
    Wordnode* words = malloc(2 * sizeof(Wordnode));
    if (words == NULL) { /* Malloc error handling */
        fprintf(stderr, "Error while allocating memory: %s", strerror(errno));
        return NULL;
    }
    words[0] = malloc(hor_count * sizeof(Word));
    words[1] = malloc(ver_count * sizeof(Word));
    if (words[0] == NULL || words[1] == NULL) { /* Malloc error handling */
        fprintf(stderr, "Error while allocating memory: %s", strerror(errno));
        return NULL;
    }
    int begin_hor, begin_ver;
    int hor_index = 0, ver_index = 0;
    int hor_score = 0, ver_score = 0; //TODO word score
    for (int i = 0 ; i < crossword_size ; i++) {
        for (int j = 0 ; j < crossword_size ; j++) {
            if (crossword[i][j] != '#') {
                if (hor_size == 0) {
                    begin_hor = j;
                }
                if ((i > 0 && crossword[i - 1][j] != '#') || (i < crossword_size - 1 && crossword[i + 1][j] != '#'))
                    hor_score++;
                hor_size++;
            }
            if (crossword[i][j] == '#') {
                if (hor_size > 1) {  
                    words[0][hor_index].orientation = 0;
                    words[0][hor_index].score = hor_score;
                    words[0][hor_index].constant = i;
                    words[0][hor_index].begin = begin_hor;
                    words[0][hor_index].end = j - 1;
                    hor_index++;
                }
                hor_score = 0;
                hor_size = 0;
            }
            if (crossword[j][i] != '#') {
                if (ver_size == 0) {
                    begin_ver = j;
                }
                if ((i > 0 && crossword[j][i - 1] != '#') || (i < crossword_size - 1 && crossword[j][i + 1] != '#'))
                    ver_score++;
                ver_size++;
            }
            if (crossword[j][i] == '#') {
                if (ver_size > 1) {
                    words[1][ver_index].orientation = 1;
                    words[1][ver_index].score = ver_score;
                    words[1][ver_index].constant = i;
                    words[1][ver_index].begin = begin_ver;
                    words[1][ver_index].end = j - 1;
                    ver_index++;
                }
                ver_score = 0;
                ver_size = 0;
            }
        }
        if (hor_size > 1) {
            words[0][hor_index].orientation = 0;
            words[0][hor_index].score = hor_score;
            words[0][hor_index].constant = i;
            words[0][hor_index].begin = begin_hor;
            words[0][hor_index].end = crossword_size - 1;
            hor_index++;
        }
        if (ver_size > 1) {
            words[1][ver_index].orientation = 1;
            words[1][ver_index].score = ver_score;
            words[1][ver_index].constant = i;
            words[1][ver_index].begin = begin_ver;
            words[1][ver_index].end = crossword_size - 1;
            ver_index++;
        }
        hor_size = 0;
        hor_score = 0;
        ver_size = 0;
        ver_score = 0;
    }
    Wordnode words_cj = malloc((hor_count + ver_count) * sizeof(Word));
    *wordnode_count = hor_count + ver_count;
    while (hor_count || ver_count) {
        if (hor_count) {
            words_cj[hor_count + ver_count - 1] = words[0][hor_count - 1];
            --hor_count;
        }
        if (ver_count) {
            words_cj[hor_count + ver_count - 1] = words[1][ver_count - 1];
            --ver_count;
        }
    }
    return words_cj;
}

void prop_word(Wordnode words, int last, char** crossword, Bitmaps maps, int* map_sizes) {
    if (!last) return;
    int index = 0;
    char* filter = create_filter(crossword, words[0]);
    int* map = create_map(maps, map_sizes, filter);
    int min = sum_bit(map, map_sizes[strlen(filter) - 1]);
    free(map);
    free(filter);
    for (int i = 1 ; i <= last ; ++i) {
        filter = create_filter(crossword, words[i]);
        map = create_map(maps, map_sizes, filter);
        int temp = sum_bit(map, map_sizes[strlen(filter) - 1]);
        if (temp < min) {
            min = temp;
            index = i;
        }
        free(map);
        free(filter);
    }
    Word temp = words[last];
    words[last] = words[index];
    words[index] = temp;
}