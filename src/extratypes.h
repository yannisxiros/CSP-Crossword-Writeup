#ifndef EXTRATYPES_H_
#define EXTRATYPES_H_

typedef Dictword* Dictionary;

typedef struct {
    int value; /* Value of word calculated using better alphabet */
    char* word; /* Word string */
} Dictword;

typedef struct {
    int orientation; /* 0 for horizontal // 1 for vertical */
    int constant; /* Holds row or col of word (depends on orientation) */
    int size; /* Word size (end - begin + 1) */
    int begin; /* Begin row or col of word */
    int end; /* End row or col of word */
} Word;

// Stack 101
typedef struct {
    char** crossword;
    Word* words;
    int* map;
} State;

//TODO find better names maybe

#endif