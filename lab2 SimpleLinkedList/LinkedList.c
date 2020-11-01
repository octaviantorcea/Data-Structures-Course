#include <stdio.h>
#include <stdlib.h>

#include "LinkedList.h"

/*
 * Functie care trebuie apelata dupa alocarea unei liste simplu inlantuite, pentru a o initializa.
 * (Setare valori initiale pentru campurile specifice structurii LinkedList).
 */
void init_list(struct LinkedList *list) {
    list->head = NULL;
    list->size = 0;
}

void add_first(struct LinkedList *list, void *new_data) {
    if (list == NULL) {
        printf("Nu exista lista boss\n");
        return;
    }

    list->size++;
    struct Node *new;
    new = (struct Node*)malloc(sizeof(struct Node));
    new->data = new_data;
    new->next = list->head;
    list->head = new;
}

void add_last(struct LinkedList *list, void *new_data) {
    if (list == NULL) {
        printf("Nu exista lista boss\n");
        return;
    }
    list->size++;
    struct Node *new, *curr;
    new = (struct Node*)malloc(sizeof(struct Node));
    new->data = new_data;
    new->next = NULL;
    curr = list->head;

    if (curr == NULL) {
        list->head = new;
        return;
    }

    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new;
    return;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e adaugat pe pozitia n a listei
 * reprezentata de pointerul list. Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla
 * pe pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca n < 0, eroare.
 */
void add_nth_node(struct LinkedList *list, int n, void *new_data) {
    if (list == NULL) {
        printf("Problema sefule nu e lista\n");
        return;
    }

    if (n < 0) {
        printf("Nu prea merge sa adaug un nod de ordin negativ, boss\n");        
        return;
    }

    struct Node *new, *curr;
    new = (struct Node *)malloc(sizeof(struct Node));
    new->data = new_data;
    list->size++;
    if (list->head == NULL) {
        list->head = new;
        new->next = NULL;
        return;
    }

    if (n == 0) {
        new->next = list->head;
        list->head = new;
        return;
    }

    curr = list->head;
    n--;
    while (n > 0 && curr->next != NULL) {
        curr = curr->next;
        n--;
    }

    new->next = curr->next;
    curr->next = new;
}

void remove_first(struct LinkedList *list) {
    if (list == NULL) {
        printf("Problema sefule nu e lista\n");
        return;
    }

    if (list->head == NULL) {
        printf("Lista oricum n-are niciun element.\n");
        return;
    }

    list->size--;
    struct Node *first;
    first = list->head;
    list->head = first->next;
    free(first);
}

void remove_last(struct LinkedList *list) {
    if (list == NULL) {
        printf("Problema sefule nu e lista\n");
        return;
    }

    if (list->head == NULL) {
        printf("Lista oricum n-are niciun element.\n");
        return;
    }

    list->size--;
    if (list->head->next == NULL) {
        free(list->head);
        list->head = NULL;
        return;
    }

    struct Node *last, *curr;
    last = list->head->next;
    curr = list->head;
    while(last->next != NULL) {
        curr = last;
        last = last->next;
    }

    curr->next = NULL;
    free(last);
    return;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca parametru.
 * Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1,
 * se elimina nodul de la finalul listei. Daca n < 0, eroare.
 * Functia intoarce un pointer spre acest nod proaspat eliminat din lista.
 * Este responsabilitatea apelantului sa elibereze memoria acestui nod.
 */
struct Node* remove_nth_node(struct LinkedList *list, int n) {
    if (list == NULL) {
        printf("Problema sefule nu e lista\n");
        return NULL;
    }

    if (n < 0) {
        printf("Nu prea merge sa sterg un nod de ordin negativ, boss\n");        
        return NULL;
    }

    if (list->head == NULL) {
        printf("Problem boss nu e niciun nod\n");
        return NULL;
    }

    list->size--;
    struct Node *curr, *prev;
    if (n == 0) {
        curr = list->head;
        list->head = curr->next;
        return curr;
    }

    curr = list->head->next;
    prev = list->head;
    n--;
    while (n > 0 && curr->next != NULL) {
        prev = curr;
        curr = curr->next;
        n--;
    }

    prev->next = curr->next;
    return curr;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca parametru.
 */
int get_size(struct LinkedList *list) {
    if (list == NULL) {
        printf("Nu exista lista\n");
        return -1;
    }
    
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la sfarsit, elibereaza memoria folosita
 * de structura lista si actualizeaza la NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void free_list(struct LinkedList **pp_list) {
    if ((*pp_list) == NULL) {
        printf("Problema sefule nu e lista\n");
        return;
    }

    struct Node *prev;
    while((*pp_list)->size) {
        prev = (*pp_list)->head;
        (*pp_list)->head = (*pp_list)->head->next;
        free(prev);
        (*pp_list)->size--;
    }

    free(*pp_list);
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM ca stocheaza int-uri.
 * Functia afiseaza toate valorile int stocate in nodurile din lista inlantuita separate printr-un spatiu.
 */
void print_int_linkedlist(struct LinkedList *list) {
    if (list == NULL) {
        printf("Nu exista lista\n");
        return;
    }

    struct Node *curr;
    curr = list->head;
    while (curr != NULL) {
        printf("%d ", *((int *)curr->data));
        curr = curr->next;
    }

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM ca stocheaza string-uri.
 * Functia afiseaza toate string-urile stocate in nodurile din lista inlantuita, separate printr-un spatiu.
 */
void print_string_linkedlist(struct LinkedList *list) {
    if (list == NULL) {
        printf("Nu exista lista\n");
        return;
    }

    struct Node *curr;
    curr = list->head;
    while (curr != NULL) {
        printf("%s ", (char *)curr->data);
        curr = curr->next;
    }

    printf("\n");
}

/*
 * Intrebare interviu
 */
struct Node* findKthNodeFromTail(struct LinkedList *list, int k) {
    if (list == NULL) {
        printf("Lista nu exista.\n");
        return NULL;
    }
    
    struct Node *aux = list->head, *kNode = list->head;
    for (int i = 0; i < k; i++) {
        aux = aux->next;
    }
    
    while (aux != NULL) {
        aux = aux->next;
        kNode = kNode->next;
    }

    return kNode;
}

/*
 * Bonus 1
 */
struct LinkedList* mergeLists(struct LinkedList *list1, struct LinkedList *list2) {
    if (list1 == NULL || list2 == NULL) {
        printf("Una din liste nu exista\n");
        return NULL;
    }

    struct LinkedList *merged = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    init_list(merged);
    struct Node *node1 = list1->head, *node2 = list2->head;
    while (node1 != NULL && node2 != NULL) {
        add_last(merged, node1->data);
        add_last(merged, node2->data);
        node1 = node1->next;
        node2 = node2->next;
    }

    if (node2 == NULL) {
        while (node1 != NULL) {
            add_last(merged, node1->data);
            node1 = node1->next;
        }
    } else if (node1 == NULL) {
        while (node2 != NULL) {
            add_last(merged, node2->data);
            node2 = node2->next;
        }
    }

    return merged;
}

/*
 * Bonus 2
 */
void addMiddle(struct LinkedList *list, void *new_data) {
    if (list == NULL) {
        printf("Lista nu exista.\n");
        return;
    }

    int n = get_size(list);
    n = n / 2;
    add_nth_node(list, n, new_data);
}

int main() {
    struct LinkedList *list;
    struct Node *currNode;
    int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    list = malloc(sizeof(struct LinkedList));
    init_list(list);

    add_nth_node(list, 0, &numbers[10]);
    currNode = remove_nth_node(list, 0);
    printf("#1:\n");
    printf("Output: %d\n", *((int*)(currNode->data)));
    printf("Output asteptat: 10\n\n");
    free(currNode);

    add_nth_node(list, 0, &numbers[8]);
    currNode = remove_nth_node(list, 0);
    printf("#2:\n");
    printf("Output: %d\n", *((int*)(currNode->data)));
    printf("Output asteptat: 8\n\n");
    free(currNode);

    add_nth_node(list, 0, &numbers[0]);
    add_nth_node(list, 10, &numbers[1]);
    add_nth_node(list, 20, &numbers[2]);
    add_nth_node(list, 30, &numbers[3]);
    add_nth_node(list, 40, &numbers[4]);
    printf("#3:\n");
    printf("Output: ");
    print_int_linkedlist(list);
    printf("Output asteptat: 0 1 2 3 4\n\n");

    add_nth_node(list, 0, &numbers[0]);
    add_nth_node(list, 0, &numbers[1]);
    add_nth_node(list, 0, &numbers[2]);
    add_nth_node(list, 0, &numbers[3]);
    add_nth_node(list, 0, &numbers[4]);
    printf("#4:\n");
    printf("Output: ");
    print_int_linkedlist(list);
    printf("Output asteptat: 4 3 2 1 0 0 1 2 3 4\n\n");

    currNode = remove_nth_node(list, 0);
    free(currNode);
    printf("#5:\n");
    printf("Output: ");
    print_int_linkedlist(list);
    printf("Output asteptat: 3 2 1 0 0 1 2 3 4\n\n");

    currNode = remove_nth_node(list, 8);
    free(currNode);
    printf("#6:\n");
    printf("Output: ");
    print_int_linkedlist(list);
    printf("Output asteptat: 3 2 1 0 0 1 2 3\n\n");

    currNode = remove_nth_node(list, 0);
    free(currNode);
    currNode = remove_nth_node(list, 0);
    free(currNode);
    currNode = remove_nth_node(list, 0);
    printf("#7:\n");
    printf("Output: ");
    printf("%d\n", *((int*)currNode->data));
    free(currNode);
    printf("Output asteptat: 1\n\n");

    currNode = remove_nth_node(list, 40);
    free(currNode);
    currNode = remove_nth_node(list, 30);
    free(currNode);
    currNode = remove_nth_node(list, 20);
    free(currNode);
    currNode = remove_nth_node(list, 10);
    free(currNode);
    currNode = remove_nth_node(list, 0);
    free(currNode);
    add_nth_node(list, 0, &numbers[3]);
    add_nth_node(list, 1, &numbers[3]);
    add_nth_node(list, 2, &numbers[7]);
    add_nth_node(list, 0, &numbers[1]);
    printf("#8:\n");
    printf("Output: ");
    print_int_linkedlist(list);
    printf("Output asteptat: 1 3 3 7\n\n");

    currNode = remove_nth_node(list, 2);
    free(currNode);
    currNode = remove_nth_node(list, 1);
    free(currNode);
    printf("#9:\n");
    printf("Output: ");
    print_int_linkedlist(list);
    printf("Output asteptat: 1 7\n\n");

    free_list(&list);

    list = malloc(sizeof(struct LinkedList));
    init_list(list);

    add_nth_node(list, 0, "mere");
    add_nth_node(list, 0, "are");
    add_nth_node(list, 0, "Ana");
    printf("#10:\n");
    printf("Output: ");
    print_string_linkedlist(list);
    printf("Output asteptat: Ana are mere\n");

    free_list(&list);

    return 0;
}
