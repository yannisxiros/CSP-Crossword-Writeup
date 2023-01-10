#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extratypes.h"
#include "extrafuns.h"

void push_word(Actionnode actions, int* map, int index, Wordnode words, int crossword_size) {
    for (int i = 0 ; i < crossword_size ; i++) {
        memcpy(actions[index].crossword[i], actions[index + 1].crossword[i], crossword_size * sizeof(char));
    }
    actions[index].map = map;
    actions[index].wordnode = words + index;
}