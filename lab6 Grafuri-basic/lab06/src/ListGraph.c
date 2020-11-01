#include <stdlib.h>
#include <stdio.h>

#include "ListGraph.h"

void init_list_graph(ListGraph *graph, int nodes) {
    graph->nodes = nodes;
    graph->neighbors = (LinkedList **)malloc(nodes * sizeof(LinkedList *));

    for (int i = 0; i < nodes; i++) {
        graph->neighbors[i] = (LinkedList *)malloc(sizeof(LinkedList));
    }

    for (int i = 0; i < nodes; i++) {
        init_list(graph->neighbors[i]);
    }
}

void add_edge_list_graph(ListGraph *graph, int src, int *dest) {
    add_nth_node(graph->neighbors[src], graph->neighbors[src]->size, dest);
}

int has_edge_list_graph(ListGraph *graph, int src, int dest) {
    Node *node = graph->neighbors[src]->head;
    
    while (node != NULL) {
        if (*((int *)node->data) == dest) {
            return 1;
        }

        node = node->next;
    }

    return 0;
}

LinkedList* get_neighbours_list_graph(ListGraph *graph, int node) {
    return graph->neighbors[node];
}

void remove_edge_list_graph(ListGraph *graph, int src, int dest) {
    Node *aux = graph->neighbors[src]->head;
    int number = 0;

    while (aux != NULL) {
        if (*(int *)aux->data == dest) {
            remove_nth_node(graph->neighbors[src], number);
            return;
        }

        aux = aux->next;
        number++;
    }
}

void clear_list_graph(ListGraph *graph) {
    for (int i = 0; i < graph->nodes; i++) {
        free_list(&(graph->neighbors)[i]);
    }
    free(graph->neighbors);
}
