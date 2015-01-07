#include "chainer.h"

int main(int argc, char* argv[]) {
    time_t t;
    srand((unsigned) time(&t));
    char* file_name = argv[1];

    if(argc == 1 ) {
      printf("No input file!\n");
      exit(1);
    };

    FILE* fp = fopen(file_name, "r");

    // gets input length (moves fp back to start)
    fseek(fp, 0L, SEEK_END);
    int input_length = ftell(fp);
    rewind(fp);

    // gets string from file, puts into file_contents
    char* file_contents = malloc(sizeof(char) * input_length);
    fread(file_contents, sizeof(char), input_length, fp);

    // this converts
    int num_words;
    char** words = wordify(file_contents, &num_words);
    free(file_contents);

    // sets first and last word for default, no word-limit function
    first = words[0];
    last = words[num_words-1];

    // initializes word map
    word_map_t* map = malloc(sizeof(word_map_t));
    map_size = 0;

    // initializes map's array of pairs
    map->pairs = malloc(sizeof(pair_t) * num_words);

    // temporary mould to format words into pairs
    pair_t temp_pair;

    int i;
    int a = 1; // required to form pairs.

    // this somehow doesnt work... takes \n separated words into one word...
    for (i = 0; i < num_words - 1; i++) {


        temp_pair = (pair_t) {&a,words[i],&words[i+1]};
        (temp_pair.num_tails)  = malloc(sizeof(int) * 1);
        *(temp_pair.num_tails)  = 1;

        // note that insert_pair COPIES the pair, so this is okay c:
        insert_pair(map,&temp_pair);
    }
    // printf("\n\n");
    // print_map(map);

    printf("\n");
    print_generation(map);
    printf("\n");


    free(map);

}

int find_word(word_map_t* map, char* word) {
    int i;
    for (i = 0; i < map_size; i++) {
        if ( strcmp((map->pairs)[i].head , word) == 0 ) {
            return i;
        }
    }
    return -1;
}

void print_generation(word_map_t* map) {
    int i = 0;
    char* word;
    char* tail;
    int num_words;
    while( strcmp(word=(map->pairs)[i].head , last)) {
        printf("%s ",word);
        num_words = *(map->pairs)[i].num_tails;
        tail = (map->pairs)[i].tails[rand() / (RAND_MAX / num_words + 1)];
        i = find_word(map, tail);
        if (i == -1) {
            break;
        }
    }
    printf("%s\n",last);
}

char** wordify(char* string, int* wordz) {
    // shoves words into new buffer

    int num_words = 0, cur_word_length = 0;
    int first = 0;

    // 'initializes' words array. (will be realloced later, dont worry)
    char** words = malloc(0);

    int cur_pos = 0;
    // goes through all characters
    do {
        if (string[cur_pos] == '\n' || string[cur_pos] == ' ') {

            words = realloc(words,sizeof(char*) * (num_words+1));
            words[num_words] = malloc(sizeof(char) * cur_word_length);
            strncpy(words[num_words], (string + first), cur_word_length);
            (words[num_words])[cur_word_length] = '\0';


            first = cur_pos+1;
            cur_word_length = 0;
            num_words++;
        } else {
            cur_word_length++;
        }

        cur_pos++;
    } while (string[cur_pos] != '\0');

    *wordz = num_words;

    return words;
}

void print_words(char** words, int num_words) {
    int i;
    printf("[");
    for (i = 0; i < num_words - 1; i++) {
        printf(" %s |",words[i]);
    }
    printf(" %s ]\n",words[num_words-1]);
}


void print_map(word_map_t* map) {
    int i;
    int t;
    int j;
    for (i = 0; i < map_size; i++) {
        printf("%s: ",(map->pairs)[i].head);
        t = *((map->pairs)[i].num_tails);
        for (j = 0; j < t; j++) {
            printf("| %s ", (map->pairs)[i].tails[j]);
        fflush(stdout);

        }
        printf("\n");

    }

}


void insert_pair(word_map_t* map, pair_t* pair) {
    int pos;
    int num_tails;
    pair_t* dest_pair;

    if ((pos = position_of(pair,map)) > -1) { // adds tail to the existing tails*
        dest_pair = &(map->pairs)[pos];
        num_tails = (*((map->pairs)[pos]).num_tails);
        dest_pair->tails = realloc(dest_pair->tails, sizeof(char*) * (num_tails + 1));
        dest_pair->tails[num_tails] = malloc(sizeof(char) * strlen(pair->tails[0]));

        strcpy(dest_pair->tails[num_tails] , pair->tails[0]);

        *dest_pair->num_tails = *dest_pair->num_tails + 1;


    } else { // creates new pair in map's pairs
        dest_pair = &(map->pairs)[map_size];
        dest_pair->num_tails = malloc(sizeof(int) * 1);
        *(dest_pair->num_tails) = 1;
        dest_pair->head = malloc(sizeof(char) * strlen(pair->head));
        strcpy(dest_pair->head , pair->head);

        dest_pair->tails = malloc(sizeof(char*) * 1);
        dest_pair->tails[0] = malloc(sizeof(char) * strlen(pair->tails[0]));
        strcpy(dest_pair->tails[0] , pair->tails[0]);


        map_size++;
    }
}

int position_of(pair_t* pair, word_map_t* map) {
    int i;
    for (i = 0; i < map_size; i++) {
        if ( strcmp( ((map->pairs)[i]).head , pair->head ) == 0) {
            return i;
        }
    }
    return -1;
}

int is_greater(char* a, char* b) { // TODO: fix this.
    int i = 0;
    while ( (a[i] != '\0') && (b[i] != '\0') ) {
        if (a[i] > b[i])
            return 1;
        else if (a[i] < b[i])
            return -1;
        i++;
    }
    if (a[i] != '\0')
        return 1;
    else if (b[i] != '\0')
        return -1;
    else
        return 0;
}


