#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int map_size;
char* first;
char* last;


typedef struct {
    int* num_tails;
    char* head;
    char** tails;
} pair_t;

typedef struct {
    // just a regular array for now
    pair_t* pairs;
} word_map_t;


int main(int, char**);

int position_of(pair_t*, word_map_t*);

char** wordify(char*, int*);

void insert_pair(word_map_t*, pair_t*);

void print_words(char**,int);

void print_map(word_map_t*);

void print_generation(word_map_t*);

int find_word(word_map_t*, char*);
