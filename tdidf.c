#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h> // from terminal add "-lm": gcc main.c -lm

enum { WORD_LEN = 25 };

struct word {
    char word [WORD_LEN];
    int count;
    int where_exists;
    double importance;
};

int scanner (FILE *file, char *buff)
{
    int i=0, c;
    char *word = (char *) malloc(WORD_LEN*sizeof(char));

    while (1) {

        c = fgetc(file);

        if( feof(file) && i == 0) {
            free(word);
            return 0;

        } else if ( feof(file) || ispunct(c) || isspace(c) ) {
            if ( i != 0 ) break;
            else continue;

        } else {
            word[i++] = c;
            word[i] = '\0';
        }
    }

    strcpy(buff, word);
    free(word);

    return 1;
}

int find_word(char *word, struct word *words_array, int uniq_word_counter)
{
    int i;
    for ( i=0; i<uniq_word_counter; i++) {
        if ( strcmp(words_array[i].word, word) == 0 ) return i;
    }
    return -1;
}

void calculate_weighting (struct word *words_array,
                           int SOURCES_AMOUNT,
                           int uniq_word_counter,
                           int all_word_counter)
{
    int  i;
    double tf, idf, where_exists, normalizer = 0.0;

    // For every word find tf and idf, miltiple them
    for ( i=0; i<uniq_word_counter; i++) {

        tf = (double) words_array[i].count/all_word_counter;
        where_exists = (double)words_array[i].where_exists;
        idf = log10(SOURCES_AMOUNT/where_exists);

        words_array[i].importance = tf*idf;
    }

    // Find normalizer
    for ( i=0; i<uniq_word_counter; i++) {
        normalizer += pow(words_array[i].importance, 2);
    }
    normalizer = sqrt(normalizer);

    // Find importance
    for ( i=0; i<uniq_word_counter; i++) {
        words_array[i].importance /= normalizer;
    }
}

void initialize (struct word *words_array, int i, char *buff)
{
    words_array[i].where_exists = words_array[i].count = 1;
    strcpy(words_array[i].word, buff);
}

void check_another_files (struct word *words_array,
                          int SOURCES_AMOUNT,
                          FILE *other_files [],
                          int uniq_word_counter)
{
    int i=0;
    char buff [WORD_LEN];

    for (i=0; i<SOURCES_AMOUNT-1; i++) {

        while ( scanner(other_files[i], buff) != 0 ) {

            if ( strcmp(buff, words_array[uniq_word_counter].word) == 0 ) {
                words_array[uniq_word_counter].where_exists++;
                break;
            }

        }

        rewind(other_files[i]);
    }

}

void insertion_sort (struct word *words_array, int uniq_word_counter)
{
    int in, out;
    struct word temp;

    for (out=1; out<uniq_word_counter; out++) {
        temp = words_array[out];

        in = out;

        while ( in>0 && words_array[in-1].importance <= temp.importance ) {
            words_array[in] = words_array[in-1];
            --in;
        }

        words_array[in] = temp;
    }
}

void show (struct word *words_array, int uniq_word_counter)
{
    int i;

    for (i=0; i<uniq_word_counter; i++) {
        printf("%-22s %3d %3d %10.6f \n", words_array[i].word,
                                 words_array[i].count,
                                 words_array[i].where_exists,
                                 words_array[i].importance );
    }
}


int main(int argc, char *argv[])
{
    if ( argc >= 3 ) {

        const int SOURCES_AMOUNT = argc-1;
        int i, uniq_word_counter = 0, all_word_counter = 0, ALLOWED_ELEM = 256;
        char buff [WORD_LEN];
        struct word *words_array = (struct word *) malloc(ALLOWED_ELEM*sizeof(struct word));
        FILE *main_file, *other_files [SOURCES_AMOUNT-1];


        // Open files
        main_file = fopen(argv[1], "r");

        for (i=2; i<=SOURCES_AMOUNT; i++) {
            other_files[i-2] = fopen(argv[i], "r");
        }

        // Parse the main document
        while ( scanner(main_file, buff) != 0 ) {

            int existing_word_index = find_word (buff, words_array, uniq_word_counter); // Search word in the words array

            if ( existing_word_index == -1 ) {          // If there is no this word then

                // Add this word
                initialize (words_array, uniq_word_counter, buff);

                // Check other files for this word
                check_another_files (words_array, SOURCES_AMOUNT, other_files, uniq_word_counter);

                uniq_word_counter++;

            } else {                                   // If there is this word then
                words_array[existing_word_index].count++;  // Increment the amount of this word
            }

            if (uniq_word_counter == ALLOWED_ELEM) {
                ALLOWED_ELEM = ALLOWED_ELEM+256;
                words_array = (struct word *) realloc(words_array,ALLOWED_ELEM*sizeof(struct word));
            }

            all_word_counter++;
        }


        // Close the files
        fclose(main_file);

        for (i=0; i<SOURCES_AMOUNT-1; i++) {
            fclose(other_files[i]);
        }


        calculate_weighting (words_array, SOURCES_AMOUNT, uniq_word_counter, all_word_counter);

        insertion_sort (words_array, uniq_word_counter);


        // Show the words
        printf("\n == START == \n\n");
        show(words_array, uniq_word_counter);
        printf("\n == END == \n");

        free(words_array);

    } else {
        printf("At least two files expected.\n");
    }

    return 0;
}
