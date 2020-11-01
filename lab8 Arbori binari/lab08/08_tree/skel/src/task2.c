/**
 * SD, 2020
 * 
 * Lab #8, Binary Tree
 * 
 * Task #2 - <resume_or_paste_the_task_here>
 */

#include <stdio.h>
#include <stdlib.h>

#include "binary_tree.h"

#define MAXLENGTH 10

void path(b_node_t *root, int nod, int *count, int *drum, int *ok) {
    if (root == NULL) {
        return;
    }

    if (*(root->data) == nod) {
        *ok = 1;
        drum[*count] = *(root->data);
        return;
    }
    
    if (*ok == 0) {
        drum[*count] = *(root->data);
        (*count)++;
        path(root->left, nod, count, drum, ok);
        path(root->right, nod, count, drum, ok);

        if (*ok == 0) {
            (*count)--;
        }

    } else {
        return;
    }
}

int main() {
    b_tree_t *binary_tree;
    binary_tree = b_tree_create();
    
    printf("Pune un arbore\n");
    int N = 0;
    int data;
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        scanf("%d", &data);
        b_tree_insert(binary_tree, data);
    }

    int nodeA = 0, nodeB = 0, count = 0, ok = 0;
    int *pathA = calloc(MAXLENGTH, sizeof(int));
    int *pathB = calloc(MAXLENGTH, sizeof(int));
    printf("Primul nod: ");
    scanf("%d", &nodeA);
    printf(" \nSi acum nodul 2: ");
    scanf("%d", &nodeB);
    path(binary_tree->root, nodeA, &count, pathA, &ok);
    count = 0;
    ok = 0;
    path(binary_tree->root, nodeB, &count, pathB, &ok);

    int i = 0;

    while (1) {
        if (pathA[i] == pathB[i]) {
            i++;
        } else {
            printf(" \nStramosul comun cel mai de jos este: %d\n", pathA[i - 1]);
            break;
        }
    }

    free(pathA);
    free(pathB);
    b_tree_free(binary_tree);
    return 0;
}
