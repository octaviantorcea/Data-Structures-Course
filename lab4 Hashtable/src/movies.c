#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Hashtable.h"

#define MAXLENGHT 30
#define NRFILME 5

struct InfoMovie {
    char *nume;
    char *regizor;
    int an;
    int rating;
};

void print_movie(struct InfoMovie *p_info) {
    if (p_info == NULL) {
        printf("Film inexistent!\n");
        return;
    }
    printf("* Nume: %s | ", p_info->nume);
    printf("Regizor: %s | ", p_info->regizor);
    printf("An: %d | ", p_info->an);
    printf("Rating: %d\n", p_info->rating);
}

/*
 * Functie ce afiseaza informatiile din intregul hashtable
 */
void print_table(struct Hashtable *ht) {
    
    for (int i = 0; i < ht->hmax; i++) {
        struct Node *curr = ht->buckets[i].head;

        while (curr != NULL) {
            struct info *info = curr->data;
            struct InfoMovie *movie = info->value;
            print_movie(movie);
            curr = curr->next;
        }
    }
}

/*
 * Cerinta b
 */
void printSpecificMovie(struct Hashtable *ht) {
    struct InfoMovie *p_infoMovie;
    char movie_query[MAXLENGHT];
    printf(" \nNumele filmului: ");
    fgets(movie_query, MAXLENGHT, stdin);
    movie_query[strlen(movie_query) - 1] = '\0';
    p_infoMovie = get(ht, movie_query);
    print_movie(p_infoMovie);
}

/*
 * Cerinta c
 * Ma folosesc deja de "baza de date" pe care o am cu filme si caut in ea
 * numele regizorului ca sa pot sa iau numele filmului ca sa pot sa-l
 * folosesc ca key.
 */
void printSpecificDirector(struct Hashtable *ht, struct InfoMovie *informatii_filme) {
    struct InfoMovie *p_infoMovie;
    char regizor[MAXLENGHT];
    printf(" \nNumele regizorului: ");
    fgets(regizor, MAXLENGHT, stdin);
    regizor[strlen(regizor) - 1] = '\0';

    for (int i = 0; i < NRFILME; i++) {
        if (strcmp(informatii_filme[i].regizor, regizor) == 0) {
            p_infoMovie = get(ht, informatii_filme[i].nume);
            print_movie(p_infoMovie);
            return;
        }
    }

    printf("Never heard about him\n");
}

/*
 * Cerinta d
 */
void deleteMoviesUnder5(struct Hashtable *ht) {
    for (int i = 0; i < ht->hmax; i++) {
        struct Node *curr = ht->buckets[i].head;

        while (curr != NULL) {
            struct Node *prev = curr;
            curr = curr->next;
            struct info *info = prev->data;
            struct InfoMovie *movie = info->value;
            
            if (movie->rating < 5) {
                printf("Se elimina filmul: %s\n", movie->nume);
                remove_ht_entry(ht, info->key);
            }
        }
    }
}

int main() {
    struct Hashtable *ht_movies;
    struct InfoMovie informatii_filme[NRFILME];

    // Initializare structuri informatii filme.
    informatii_filme[0].nume = "Being Human";
    informatii_filme[0].regizor = "Bill Forsyth";
    informatii_filme[0].an = 1994;
    informatii_filme[0].rating = 4;

    informatii_filme[1].nume = "Being human";
    informatii_filme[1].regizor = "Toby Whithouse";
    informatii_filme[1].an = 2008;
    informatii_filme[1].rating = 7;

    informatii_filme[2].nume = "Avatar";
    informatii_filme[2].regizor =  "James Cameron";
    informatii_filme[2].an = 2009;
    informatii_filme[2].rating = 8;

    informatii_filme[3].nume = "The edge of seventeen";
    informatii_filme[3].regizor =  "Kelly Fremon Craig";
    informatii_filme[3].an = 2016;
    informatii_filme[3].rating = 7;

    informatii_filme[4].nume = "The edge of love";
    informatii_filme[4].regizor = "John Maybury";
    informatii_filme[4].an = 2008;
    informatii_filme[4].rating = 3;

    // Initializare ht
    ht_movies = malloc(sizeof(struct Hashtable));
    init_ht(ht_movies, 10, hash_function_string, compare_function_strings);

    /*
     * Cerinta a
     */
    printf("Cerinta a\n \n");
    put(ht_movies, "Being Human", strlen("Being Human") + 1, &informatii_filme[0]);
    put(ht_movies, "Being human", strlen("Being human") + 1, &informatii_filme[1]);
    put(ht_movies, "Avatar", strlen("Avatar") + 1, &informatii_filme[2]);
    put(ht_movies, "The edge of seventeen", strlen("The edge of seventeen") + 1, &informatii_filme[3]);
    put(ht_movies, "The edge of love", strlen("The edge of love") + 1, &informatii_filme[4]);
    print_table(ht_movies);
    
    /*
     * Cerinta b
     */
    printf("\n \nCerinta b\n \n");
    printSpecificMovie(ht_movies);

    /*
     * Cerinta c
     */
    printf("\n \nCerinta c\n \n");
    printSpecificDirector(ht_movies, informatii_filme);

    /*
     * Cerinta d
     */
    printf("\n \nCerinta d\n \n");
    deleteMoviesUnder5(ht_movies);
    print_table(ht_movies);
    free_ht(ht_movies);

    return 0;
}
