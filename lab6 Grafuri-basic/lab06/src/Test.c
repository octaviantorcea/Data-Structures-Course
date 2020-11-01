#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "Queue.h"
#include "Stack.h"

#define MAX_NODES 10

void visitNode(int node, int *contor, int *t_desc, int *visited) {
    (*contor)++;
    t_desc[node] = *contor;
    visited[node] = 1;
}

void print_matrix_graph(MatrixGraph *mg) {
    for (int i = 0; i < mg->nodes; i++) {
        printf("Nodul %d are drum catre: ", i);
        for (int j = 0; j < mg->nodes; j++) {
            if (mg->matrix[i][j]) {
                printf("%d ", j);
            }
        }
        printf("\n \n");
    }
}

void print_list_graph(ListGraph *lg) {
    for (int i = 0; i < lg->nodes; i++) {
        printf("Nodul %d are drum catre: ", i);
        LinkedList *toPrint = get_neighbours_list_graph(lg, i);
        print_int_linkedlist(toPrint);
        printf(" \n");
    }
}

void dfs_list_graph(ListGraph *lg, int node, int *visited, int *t_desc, int *t_fin) {
    for (int i = 0; i < lg->nodes; i++) {
        visited[i] = 0;  /* 0 = alb, 1 = gri, 2 = negru */
        t_desc[i] = 0;
        t_fin[i] = 0;
    }

    int contor = -1;
    Stack *stack = malloc(sizeof(Stack));
    init_stack(stack);
    visitNode(node, &contor, t_desc, visited);
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
            visitNode((*(int *)curr->data), &contor, t_desc, visited);
            push_stack(stack, curr->data);
        } else {
            contor++;
            t_fin[nodTop] = contor;
            visited[nodTop] = 2;
            pop_stack(stack);
        }
    }

    purge_stack(stack);
    free(stack);
}

void dfs_matrix_graph(MatrixGraph *mg, int node, int *visited, int *t_desc, int *t_fin) {
    for (int i = 0; i < mg->nodes; i++) {
        visited[i] = 0;  /* 0 = alb, 1 = gri, 2 = negru */
        t_desc[i] = 0;
        t_fin[i] = 0;
    }

    int contor = -1;
    Stack *stack = malloc(sizeof(Stack));
    init_stack(stack);
    visitNode(node, &contor, t_desc, visited);
    push_stack(stack, &node);

    /* imi iau un vector auxiliar in care sa tin minte toate valorile nodurilor
     * care trebuie puse in coada, deoarece cand se face enqueue ar trebui sa se
     * dea adresa lui i, insa i-ul se schimba mereu fiind interator in for
     */
    int count = 0;
    int *aux = calloc(mg->nodes * mg->nodes, sizeof(int));

    while (!is_empty_stack(stack)) {
        int nodTop = (*(int *)peek_stack(stack));
        int ok = 0;

        for (int i = 0; i < mg->nodes; i++) {
            if (visited[i] == 0 && mg->matrix[nodTop][i]) {
                aux[count] = i;
                ok = 1;
                break;
            }
        }

        if (ok) {
            visitNode(aux[count], &contor, t_desc, visited);
            push_stack(stack, &aux[count]);
            count++;
        } else {
            contor++;
            t_fin[nodTop] = contor;
            visited[nodTop] = 2;
            pop_stack(stack);
        }
    }

    free(aux);
    purge_stack(stack);
    free(stack);
}

void bfs_list_graph(ListGraph *lg, int node, int *visited, int *parents) {
    for (int i = 0; i < lg->nodes; i++) {
        visited[i] = 0;  /* 0 = alb, 1 = gri, 2 = negru */
        parents[i] = -1;  /* -1 -> acest nod nu are niciun parinte, -2 = de la acest cod a inceput parcurgerea */
    }

    Queue *queue = malloc(sizeof(Queue));
    init_q(queue);
    enqueue(queue, &node);

    parents[node] = -2;
    visited[node] = 1;

    while (!is_empty_q(queue)) {
        int nodeToGo = (*(int *)front(queue));
        LinkedList *neighbours = get_neighbours_list_graph(lg, nodeToGo);
        Node *curr = neighbours->head;
        
        while (curr != NULL) {
            if (visited[(*(int *)curr->data)] == 0) {
                visited[(*(int *)curr->data)] = 1;
                parents[(*(int *)curr->data)] = nodeToGo;
                enqueue(queue, curr->data);
            }

            curr = curr->next;
        }

        visited[nodeToGo] = 2;
        dequeue(queue);
    }

    purge_q(queue);
    free(queue);
}

void bfs_matrix_graph(MatrixGraph *mg, int node, int *visited, int *parents) {
    for (int i = 0; i < mg->nodes; i++) {
        visited[i] = 0;  /* 0 = alb, 1 = gri, 2 = negru */
        parents[i] = -1;  /* -1 -> acest nod nu are niciun parinte, -2 = de la acest cod a inceput parcurgerea */
    }

    /* imi iau un vector auxiliar in care sa tin minte toate valorile nodurilor
     * care trebuie puse in coada, deoarece cand se face enqueue ar trebui sa se
     * dea adresa lui i, insa i-ul se schimba mereu fiind interator in for
     */
    int count = 0;
    int *aux = calloc(mg->nodes * mg->nodes, sizeof(int));

    Queue *queue = malloc(sizeof(Queue));
    init_q(queue);
    enqueue(queue, &node);

    parents[node] = -2;
    visited[node] = 1;

    while (!is_empty_q(queue)) {
        int nodeToGo = (*(int *)front(queue));
        for (int i = 0; i < mg->nodes; i++) {
            if (mg->matrix[nodeToGo][i] == 1) {
                if (visited[i] == 0) {
                    visited[i] = 1;
                    parents[i] = nodeToGo;
                    aux[count] = i;
                    enqueue(queue, &aux[count]);
                    count++;
                }
            }
        }

        visited[nodeToGo] = 2;
        dequeue(queue);
    }

    purge_q(queue);
    free(queue);
    free(aux);
}

/* Afiseaza timpii de descoperire si de finalizare ai fiecarui nod din graf. */
void print_times(int* t_desc, int* t_fin, int nodes)
{
    int i;

    for (i = 0; i < nodes; ++i)
    {
        printf("%d: %d/%d\n", i, t_desc[i], t_fin[i]);
    }
    printf("\n");
}

/* Afiseaza parintii nodurilor din graf. */
void print_parents(int* parents, int nodes)
{
    int i;

    for (i = 0; i < nodes; ++i)
    {
        printf("%d: %d\n", i, parents[i]);
    }
    printf("\n");
}

int main(void) {
    int nodes, edges;
    int x[MAX_NODES], y[MAX_NODES];
    int visited[MAX_NODES], t_desc[MAX_NODES], t_fin[MAX_NODES];
    int parents[MAX_NODES];
    ListGraph *lg = malloc(sizeof(ListGraph));
    MatrixGraph *mg = malloc(sizeof(MatrixGraph));

    scanf("%d %d", &nodes, &edges);

    init_list_graph(lg, nodes);
    init_matrix_graph(mg, nodes);

    for (int i = 0; i < edges; ++i) {
        scanf("%d %d", &x[i], &y[i]);
        add_edge_matrix_graph(mg, x[i], y[i]);
        add_edge_list_graph(lg, x[i], &y[i]);
    }

    printf("====== Graf modelat cu matrice de adiacenta ======\n");
    print_matrix_graph(mg);

    printf("====== Graf modelat cu liste de adiacenta ======\n");
    print_list_graph(lg);

    memset(visited, 0, sizeof(visited));
    dfs_list_graph(lg, 0, visited, t_desc, t_fin);
    printf("====== Timpi de descoperire si finalizare - graf modelat cu liste ======\n");
    print_times(t_desc, t_fin, lg->nodes);

    memset(visited, 0, sizeof(visited));
    dfs_matrix_graph(mg, 0, visited, t_desc, t_fin);
    printf("====== Timpi de descoperire si finalizare - graf modelat cu matrice ======\n");
    print_times(t_desc, t_fin, mg->nodes);

    printf("====== Parintii nodurilor - graf modelat cu matrice ======\n");
    memset(visited, 0, sizeof(visited));
    memset(parents, 0xff, sizeof(parents));  /* parents[i] = -1; i = 0:(nodes - 1) */
    bfs_list_graph(lg, 0, visited ,parents);
    print_parents(parents, lg->nodes);

    printf("====== Parintii nodurilor - graf modelat cu liste ======\n");
    memset(visited, 0, sizeof(visited));
    memset(parents, 0xff, sizeof(parents));  /* parents[i] = -1; i = 0:(nodes - 1) */
    bfs_matrix_graph(mg, 0, visited ,parents);
    print_parents(parents, mg->nodes);

    clear_list_graph(lg);
    clear_matrix_graph(mg);
    free(lg);
    free(mg);
    return 0;
}
