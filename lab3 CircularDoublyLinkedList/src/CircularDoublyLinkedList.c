#include <stdio.h>
#include <stdlib.h>

#include "CircularDoublyLinkedList.h"

/*
 * Functie care trebuie apelata dupa alocarea unei liste, pentru a o initializa.
 * (Setare valori initiale pentru campurile specifice structurii LinkedList).
 */
void init_list(struct CircularDoublyLinkedList *list) {
    if (list == NULL) {
        printf("Nu exista lista.\n");
        return;
    }
    
    list->size = 0;
    list->head = NULL;
}

/*
 * Functia intoarce un pointer la nodul de pe pozitia n din lista.
 * Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe pozitia n=0).
 * Daca n >= nr_noduri, atunci se intoarce nodul de pe pozitia rezultata daca am "cicla" (posibil de mai multe ori) pe
 * lista si am trece de la ultimul nod, inapoi la primul si am continua de acolo. Cum putem afla pozitia dorita fara sa
 * simulam intreaga parcurgere?
 * Daca n < 0, eroare.
 */
struct Node* get_nth_node(struct CircularDoublyLinkedList *list, int n) {
    if (list == NULL) {
        printf("Nu exista lista.\n");
        return NULL;
    }

    if (n < 0) {
        printf("Nu exista nod cu nr de ordine negativ.\n");
        return NULL;
    }
    
    struct Node *curr = list->head;

    for (int i = 0; i < (n % list->size); i++) {
        curr = curr->next;
    }

    return curr;
}

void add_first(struct CircularDoublyLinkedList *list, void *new_data) {
    if (list == NULL) {
        printf("Nu exista lista.\n");
        return;
    }

    if (list->head == NULL) {
        struct Node *new = (struct Node *)malloc(sizeof(struct Node));
        new->data = new_data;
        list->head = new;
        new->next = new;
        new->prev = new;
        list->size++;
        return;
    }

    struct Node *new = (struct Node *)malloc(sizeof(struct Node));
    new->data = new_data;
    new->prev = list->head->prev;
    new->next = list->head;
    list->head->prev->next = new;
    list->head->prev = new;
    list->head = new;
    list->size++;
}

void add_last(struct CircularDoublyLinkedList *list, void *new_data) {
    if (list == NULL) {
        printf("Nu exista lista.\n");
        return;
    }

    if (list->head == NULL) {
        add_first(list, new_data);
        return;
    }

    struct Node *new = (struct Node *)malloc(sizeof(struct Node));
    new->data = new_data;
    new->next = list->head;
    new->prev = list->head->prev;
    list->head->prev->next = new;
    list->head->prev = new;
    list->size++;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e adaugat pe pozitia n a listei
 * reprezentata de pointerul list. Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla
 * pe pozitia n=0).
 * Cand indexam pozitiile nu "ciclam" pe lista circulara ca la get, ci consideram nodurile in ordinea de la head la
 * ultimul (adica acel nod care pointeaza la head ca nod urmator in lista). Daca n >= nr_noduri, atunci adaugam nodul
 * nou la finalul listei. Daca n < 0, eroare.
 */
void add_nth_node(struct CircularDoublyLinkedList *list, int n, void *new_data) {
    if (list == NULL) {
        printf("Nu exista lista.\n");
        return;
    }

    if (n < 0) {
        printf("Nu se poate adauga un nod cu nr de ordin negativ.\n");
        return;
    }

    struct Node *new = (struct Node *)malloc(sizeof(struct Node));
    new->data = new_data;

    if (list->size == 0) {
        list->head = new;
        new->next = list->head;
        new->prev = list->head;
        list->size++;
        return;
    }

    struct Node *curr = list->head;

    if (n < list->size) {
        if (n == 0) {
            new->prev = list->head->prev;
            new->next = list->head;
            list->head->prev->next = new;
            list->head->prev = new;
            list->head = new;
            list->size++;
            return;
        }
        for (int i = 0; i < n; i++) {
            curr = curr->next;
        }

        new->next = curr;
        new->prev = curr->prev;
        curr->prev->next = new;
        curr->prev = new;
        list->size++;
        return;
    } else {
        for (int i = 0; i < list->size - 1; i++) {
            curr = curr->next;
        }

        curr->next = new;
        new->prev = curr;
        new->next = list->head;
        list->head->prev = new;
        list->size++;
        return;
    }
}

/*
 * Functie care adauga un nod la locul lui DOAR INTR-O LISTA DEJA SORTATA
 * si care stim ca stocheaza inturi.
 */ 
void add_sorted(struct CircularDoublyLinkedList *list, void *new_data) {
    if (list == NULL) {
        printf("Nu exista lista.\n");
        return;
    }

    if (list->head == NULL) {
        add_first(list, new_data);
        return;
    }

    if ((*(int *)(list->head->prev->data)) < *((int *)new_data)) {
        add_last(list, new_data);
        return;
    }

    if ((*(int *)(list->head->data)) > *((int *)new_data)) {
        add_first(list, new_data);
        return;
    }

    struct Node *new = (struct Node *)malloc(sizeof(struct Node));
    new->data = new_data;
    struct Node *curr = list->head;

    while (*((int *)curr->data) < *((int *)new_data)) {
        curr = curr->next;
    }

    curr->prev->next = new;
    new->next = curr;
    new->prev = curr->prev;
    curr->prev = new;
    list->size++;
    return;
}

void remove_first(struct CircularDoublyLinkedList *list) {
    if (list == NULL) {
        printf("Nu exista lista.\n");
        return;
    }

    if (list->size == 0) {
        printf("Nu e niciun nod.\n");
        return;
    }

    struct Node *curr = list->head;
    list->head = curr->next;
    curr->next->prev = curr->prev;
    curr->prev->next = curr->next;
    list->size--;
    free(curr);
    return;
}

void remove_last(struct CircularDoublyLinkedList *list) {
    if (list == NULL) {
        printf("Nu exista lista.\n");
        return;
    }

    if (list->size == 0) {
        printf("Nu e niciun nod.\n");
        return;
    }

    struct Node *curr = list->head->prev;
    list->head->prev = curr->prev;
    curr->prev->next = list->head;
    list->size--;
    free(curr);
    return;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca parametru.
 * Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din lista se afla pe pozitia n=0).
 * Functia intoarce un pointer spre acest nod proaspat eliminat din lista. Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare.
 * Este responsabilitatea apelantului sa elibereze memoria acestui nod.
 */
struct Node* remove_nth_node(struct CircularDoublyLinkedList *list, int n) {
    if (list == NULL) {
        printf("Nu exista lista.\n");
        return NULL;
    }

    if (n < 0) {
        printf("Nu se poate sterge un nod cu nr de ordin negativ.\n");
        return NULL;
    }

    if (list->head == NULL) {
        printf("Nu e niciun nod.\n");
        return NULL;
    }

    struct Node *curr;

    if (n == 0) {
        curr = list->head;
        list->head = curr->next;
        curr->next->prev = curr->prev;
        curr->prev->next = curr->next;
        list->size--;
        return curr;
    }

    if (n >= list->size - 1) {
        curr = list->head->prev;
        list->head->prev = curr->prev;
        curr->prev->next = list->head;
        list->size--;
        return curr;
    } else {
        curr = get_nth_node(list, n);
        curr->next->prev = curr->prev;
        curr->prev->next = curr->next;
        list->size--;
        return curr;
    }
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca parametru.
 */
int get_size(struct CircularDoublyLinkedList *list) {
    if (list == NULL) {
        printf("Nu exista lista\n");
        return -1;
    }

    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la sfarsit, elibereaza memoria folosita
 * de structura lista.
 */
void free_list(struct CircularDoublyLinkedList **pp_list) {
    if ((*pp_list) == NULL) {
        printf("Problema sefule nu e lista\n");
        return;
    }
    
    struct Node *curr;
    while ((*pp_list)->size) {
        curr = (*pp_list)->head;
        (*pp_list)->head = (*pp_list)->head->next;
        free(curr);
        (*pp_list)->size--;
    }

    free(*pp_list);
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM ca stocheaza int-uri.
 * Functia afiseaza toate valorile int stocate in nodurile din lista separate printr-un spatiu, incepand de la primul
 * nod din lista.
 */
void print_int_list(struct CircularDoublyLinkedList *list) {
    if (list == NULL) {
        printf("Nu exista lista\n");
        return;
    }

    struct Node *curr = list->head;

    for (int i = 0; i < list->size; i++) {
        printf("%d ", *((int *)curr->data));
        curr = curr->next;
    }

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM ca stocheaza string-uri.
 * Functia afiseaza toate string-urile stocate in nodurile din lista separate printr-un spatiu, incepand de la primul
 * nod din lista.
 */
void print_string_list(struct CircularDoublyLinkedList *list) {
    if (list == NULL) {
        printf("Nu exista lista\n");
        return;
    }

    struct Node *curr = list->head;

    for (int i = 0; i < list->size; i++) {
        printf("%s ", (char *)curr->data);
        curr = curr->next;
    }

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM ca stocheaza int-uri.
 * Functia afiseaza o singura data toate valorile int stocate in nodurile din lista, separate printr-un spatiu, incepand
 * de la nodul dat ca parametru si continuand la stanga in lista dublu inlantuita circulara, pana cand sunt afisate
 * valorile tuturor nodurilor.
 */
void print_ints_left_circular(struct Node *start) {
    if (start == NULL) {
        printf("Nod invalid.\n");
        return;
    }

    struct Node *aux = start;
    printf("%d ", *((int *)start->data));
    
    while (start->prev != aux) {
        start = start->prev;
        printf("%d ", *((int *)start->data));
    }

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM ca stocheaza int-uri.
 * Functia afiseaza o singura data toate valorile int stocate in nodurile din lista, separate printr-un spatiu, incepand
 * de la nodul dat ca parametru si continuand la dreapta in lista dublu inlantuita circulara, pana cand sunt afisate
 * valorile tuturor nodurilor.
 */
void print_ints_right_circular(struct Node *start) {
    if (start == NULL) {
        printf("Nod invalid.\n");
        return;
    }
    
    struct Node *aux = start;
    printf("%d ", *((int *)start->data));

    while (start->next != aux) {
        start = start->next;
        printf("%d ", *((int *)start->data));
    }

    printf("\n");
}

/*int main() {
    struct CircularDoublyLinkedList *list;
    int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    list = malloc(sizeof(struct CircularDoublyLinkedList));
    init_list(list);

    add_sorted(list, &numbers[5]);
    add_sorted(list, &numbers[8]);
    add_sorted(list, &numbers[8]);
    add_sorted(list, &numbers[1]);
    add_sorted(list, &numbers[2]);
    remove_last(list);
    remove_last(list);
    remove_last(list);
    remove_last(list);
    remove_last(list);
    print_int_list(list);

    if (list->size > 0) {
        free_list(&list);
    } else {
        free_list(&list);
    }
    

    return 0;
}*/