#include <stdlib.h>

#include "Queue.h"

void init_q(struct Queue *q) {
    q->list = malloc(sizeof(struct LinkedList));
    if (q->list == NULL) {
        perror("Not enough memory to initialize the queue!");
        return;
    }

    init_list(q->list);
}

int get_size_q(struct Queue *q) {
    return q->list->size;
}

int is_empty_q(struct Queue *q) {
    if (q->list->head == NULL) {
        return 1;
    }

    return 0;
}

void* front(struct Queue *q) {
    return q->list->tail->data;
}

void dequeue(struct Queue *q) {
    if (q->list->size == 1) {
        q->list->size = 0;
        free(q->list->head);
        q->list->tail = NULL;
        q->list->head = NULL;
        return;
    }

    struct Node *curr = q->list->head->next;
    struct Node *prev = q->list->head;

    while (curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = NULL;
    free(curr);
    q->list->size--;
    q->list->tail = prev;
}

void enqueue(struct Queue *q, void *new_data) {
    struct Node *new = (struct Node *)malloc(sizeof(struct Node));
    new->data = new_data;
    new->next = q->list->head;
    q->list->head = new;

    if (q->list->size == 0) {
        q->list->tail = new;
    }

    q->list->size++;
}

void clear_q(struct Queue *q) {
    struct Node *curr = q->list->head;

    while (curr != NULL) {
        struct Node *aux = curr;
        curr = curr->next;
        free(aux);
        q->list->size--;
    }

    free(curr);
    q->list->size--;
}

void purge_q(struct Queue *q) {
    free_list(&q->list);
}
