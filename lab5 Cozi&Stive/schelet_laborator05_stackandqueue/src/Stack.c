#include <stdio.h>
#include <stdlib.h>

#include "Stack.h"

void init_stack(struct Stack *stack) {
    stack->list = malloc(sizeof(struct LinkedList));
    if (stack->list == NULL) {
        perror("Not enough memory to initialize the stack!");
        return;
    }

    init_list(stack->list);
}

int get_size_stack(struct Stack *stack) {
    return stack->list->size;
}

int is_empty_stack(struct Stack *stack) {
    if (stack->list->head == NULL) {
        return 1;
    }

    return 0;
}

void* peek_stack(struct Stack *stack) {
    return stack->list->head->data;
}

void pop_stack(struct Stack *stack) {
    if (stack->list->head == NULL) {
        printf("Stiva este deja goala.\n");
        return;
    }

    struct Node *aux = stack->list->head;
    stack->list->head = stack->list->head->next;
    free(aux);
    stack->list->size--;
}

void push_stack(struct Stack *stack, void *new_data) {
    struct Node *new = (struct Node *)malloc(sizeof(struct Node));
    new->data = new_data;
    new->next = stack->list->head;
    stack->list->head = new;
    stack->list->size++;
}

void clear_stack(struct Stack *stack) {
    struct Node *curr = stack->list->head;

    while (curr != NULL) {
        struct Node *aux = curr;
        curr = curr->next;
        free(aux);
        stack->list->size--;
    }

    free(curr);
    stack->list->size--;
}

void purge_stack(struct Stack *stack) {
    free_list(&stack->list);
}
