#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ListGraph.h"
#include "Queue.h"
#include "Stack.h"

#define MAX_NODES 100

void visitNode(int node, int *contor, int *t_desc, int *visited) {
    (*contor)++;
    t_desc[node] = *contor;
    visited[node] = 1;
}

void dfs_connected_comps(ListGraph *lg, int node, int *visited, int *contor, int *t_desc, int *t_fin) {
    Stack *stack = malloc(sizeof(Stack));
    init_stack(stack);
    visitNode(node, contor, t_desc, visited);
    printf("%d ", node);
    push_stack(stack, &node);

    while (!is_empty_stack(stack)) {
        int nodTop = (*(int *)peek_stack(stack));
        LinkedList *neighbours = get_neighbours_list_graph(lg, nodTop);
        Node *curr = neighbours->head;

        while (curr != NULL) {
            if (visited[(*(int *)curr->data)] == 0) {
                break;
            }

            curr = curr->next;
        }

        if (curr != NULL) {
            visitNode((*(int *)curr->data), contor, t_desc, visited);
            printf("%d ", (*(int *)curr->data));
            push_stack(stack, curr->data);
        } else {
            (*contor)++;
            t_fin[nodTop] = *contor;
            visited[nodTop] = 2;
            pop_stack(stack);
        }
    }

    printf("  |  ");
    purge_stack(stack);
    free(stack);
}

void connected_components(ListGraph *lg, int *color) {
    int *t_desc = calloc(lg->nodes, sizeof(int));
    int *t_fin = calloc(lg->nodes, sizeof(int));

    for (int i = 0; i < lg->nodes; i++) {
        color[i] = 0;
    }

    int contor = -1;
    int componenteConexe = 0;

    for (int i = 0; i < lg->nodes; i++) {
        if (color[i] == 0) {
            componenteConexe++;
            dfs_connected_comps(lg, i, color, &contor, t_desc, t_fin);
        }
    }

    printf("Sunt %d componente conexe", componenteConexe);
    printf(" \n \n");
    free(t_desc);
    free(t_fin);
}

int min_path(ListGraph *lg, int src, int dest) {
    int *visited = calloc(lg->nodes, sizeof(int));
    int *distance = calloc(lg->nodes, sizeof(int));
    int *parents = calloc(lg->nodes, sizeof(int));

    for (int i = 0; i < lg->nodes; i++) {
        visited[i] = 0;  /* 0 = alb, 1 = gri, 2 = negru */
        distance[i] = 0;
        parents[i] = 0;
    }

    if (src == dest) {
        free(visited);
        free(distance);
        free(parents);
        return 0;
    }

    Queue *queue = malloc(sizeof(Queue));
    init_q(queue);
    enqueue(queue, &src);
    visited[src] = 1;
    parents[src] = -1;

    while (!is_empty_q(queue)) {
        int nodeToGo = (*(int *)front(queue));
        LinkedList *neighbours = get_neighbours_list_graph(lg, nodeToGo);
        Node *curr = neighbours->head;

         while (curr != NULL) {
            if (visited[(*(int *)curr->data)] == 0) {
                visited[(*(int *)curr->data)] = 1;
                distance[(*(int *)curr->data)] = distance[nodeToGo] + 1;
                parents[(*(int *)curr->data)] = nodeToGo;
                enqueue(queue, curr->data);
            }

            curr = curr->next;
        }

        visited[nodeToGo] = 2;
        dequeue(queue);
    }

    int sol = distance[dest];

    if (sol == 0) {
        purge_q(queue);
        free(queue);
        free(visited);
        free(distance);
        free(parents);
        return -1;  //-1 daca nu exista drum intre cele doua noduri
    }

    // ca sa le afisez in ordinea corecta
    int *road = calloc(sol + 1, sizeof(int));
    int count = 0;
    int ok = 0;
    road[count] = dest;
    ok = parents[dest];
    count++;
    
    while (ok != -1) {
        road[count] = ok;
        count++;
        ok = parents[ok];
    }

    for (int i = sol; i >= 0; i--) {
        printf("%d ", road[i]);
    }

    printf("\n");

    free(road);
    purge_q(queue);
    free(queue);
    free(visited);
    free(distance);
    free(parents);

    return sol;
}

int check_bipartite(ListGraph *lg, int *color) {
    int *visited = calloc(lg->nodes, sizeof(int));
    
    for (int i = 0; i < lg->nodes; i++) {
        color[i] = -1;
    }

    int start = 0;
    color[start] = 0;
    visited[start] = 0;
    Queue *queue = malloc(sizeof(Queue));
    init_q(queue);
    enqueue(queue, &start);

    while (!is_empty_q(queue)) {
        int nodeToGo = (*(int *)front(queue));
        LinkedList *neighbours = get_neighbours_list_graph(lg, nodeToGo);
        Node *curr = neighbours->head;

        while (curr!= NULL) {
            if (visited[(*(int *)curr->data)] == 0) {
                if (color[(*(int *)curr->data)] == -1) {
                    if (color[nodeToGo] == 0) {
                        color[(*(int *)curr->data)] = 1;
                        enqueue(queue, curr->data);
                    } else {
                        color[(*(int *)curr->data)] = 0;
                        enqueue(queue, curr->data);
                    }
                } else if (color[nodeToGo] == color[(*(int *)curr->data)]) {
                    free(visited);
                    purge_q(queue);
                    free(queue);
                    return 0;
                }
            }

            curr = curr->next;
        }

        visited[nodeToGo] = 2;
        dequeue(queue);
    }

    for (int i = 0; i < lg->nodes; i++) {
        if (visited[i] == 0) {
            free(visited);
            purge_q(queue);
            free(queue);
            return 0;
        }
    }

    free(visited);
    purge_q(queue);
    free(queue);
    return 1;
}

int main() {
    int nodes, edges;
    int color[MAX_NODES];
    int x[MAX_NODES], y[MAX_NODES];
    ListGraph *lg = malloc(sizeof(ListGraph));

    /* Ex 1 */
    printf(" \n \n-----Verificare componente conexe-----\n \n");
    printf("Un nou graf:\n");
    scanf("%d %d", &nodes, &edges);
    init_list_graph(lg, nodes);

    // pun doua muchii ca sa fac un graf neorientat
    for (int i = 0; i < edges; ++i) {
        scanf("%d %d", &x[i], &y[i]);
        add_edge_list_graph(lg, x[i], &y[i]);
        add_edge_list_graph(lg, y[i], &x[i]);
    }

    printf(" \n");

    connected_components(lg, color);
    clear_list_graph(lg);

    /* Ex 2 */
    printf("-----Calcularea distantei minime-----\n \n");
    printf("Un nou graf:\n");
    scanf("%d %d", &nodes, &edges);
    init_list_graph(lg, nodes);

    // pun doua muchii ca sa fac un graf neorientat
    for (int i = 0; i < edges; ++i) {
        scanf("%d %d", &x[i], &y[i]);
        add_edge_list_graph(lg, x[i], &y[i]);
        add_edge_list_graph(lg, y[i], &x[i]);
    }

    printf(" \n");

    int src = 0, dest = 0;
    printf("Nodul sursa: ");
    scanf("%d", &src);
    printf("Nodul destinatie: ");
    scanf("%d", &dest);
    printf("Distanta dintre cele doua noduri este: %d\n \n", min_path(lg, src, dest));
    clear_list_graph(lg);

    /* Ex 4 */
    printf(" \n-----Verificare graf bipartit-----\n \n");
    printf("Un nou graf:\n");
    scanf("%d %d", &nodes, &edges);
    init_list_graph(lg, nodes);

    // pun doua muchii ca sa fac un graf neorientat
    for (int i = 0; i < edges; ++i) {
        scanf("%d %d", &x[i], &y[i]);
        add_edge_list_graph(lg, x[i], &y[i]);
        add_edge_list_graph(lg, y[i], &x[i]);
    }

    printf(" \n");

    if (check_bipartite(lg, color)) {
        for (int i = 0; i < lg->nodes; ++i) {
            if (color[i] == 1) {
                printf("%d ", i);
            }
        } 
        printf("\n");
        for (int i = 0; i < lg->nodes; ++i) {
            if (color[i] == 0) {
                printf("%d ", i);
            }
        } 
        printf("\n");
    } else {
        printf("The graph is not bipartite\n");
    }

    clear_list_graph(lg);
    free(lg);
    return 0;
}
