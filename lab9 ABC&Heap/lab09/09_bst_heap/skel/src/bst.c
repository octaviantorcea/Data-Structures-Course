/**
 * SD, 2020
 * 
 * Lab #9, BST & Heap
 * 
 * Task #1 - Binary Search Tree implementation
 */

#include "bst.h"
#include "utils.h"

static bst_node_t *__bst_node_create(char *data)
{
    char *rc;
    bst_node_t *bst_node;

    bst_node = malloc(sizeof(bst_node_t));
    DIE(bst_node == NULL, "bst_node malloc");

    bst_node->left = bst_node->right = NULL;

    bst_node->data = calloc(BST_DATA_LEN, sizeof(char));
    DIE(bst_node->data == NULL, "bst_node->data malloc");

    rc = strncpy(bst_node->data, data, BST_DATA_LEN - 1);
    DIE(rc != bst_node->data, "bst_node->data strncpy");

    return bst_node;
}

bst_tree_t *bst_tree_create(int (*cmp_f) (const char *, const char *))
{
    bst_tree_t *bst_tree;

    bst_tree = malloc(sizeof(bst_tree_t));
    DIE(bst_tree == NULL, "bst_tree malloc");

    bst_tree->root  = NULL;
    bst_tree->cmp   = cmp_f;

    return bst_tree;
}

void bst_tree_insert(bst_tree_t *bst_tree, char *data)
{
    bst_node_t *root    = bst_tree->root;
    bst_node_t *parent  = NULL;
    bst_node_t *node    = __bst_node_create(data);

    if (root == NULL) {
        bst_tree->root = node;
        return;
    }

    parent = root;

    while (1) {
        if (bst_tree->cmp(node->data, parent->data) >= 0) {
            if (parent->right == NULL) {
                parent->right = node;
                return;
            } else {
                parent = parent->right;
            }
        } else {
            if (parent->left == NULL) {
                parent->left = node;
                return;
            } else {
                parent = parent->left;
            }
        }
    }
}

static bst_node_t *__bst_tree_remove(bst_node_t *bst_node,
                                    char *data,
                                    int (*f) (const char *, const char *))
{
    int rc;
    bst_node_t *tmp;

    if (!bst_node)
        return NULL;

    rc = f(data, bst_node->data);

    if (rc < 0) {
        if (f(bst_node->left->data, data) == 0 && (bst_node->left->left == NULL && bst_node->left->right == NULL)) {
            tmp = bst_node->left;
            bst_node->left = NULL;
            free(tmp->data);
            free(tmp);
        } else {
            __bst_tree_remove(bst_node->left, data, f);
        }
    } else if (rc > 0) {
        if (f(bst_node->right->data, data) == 0 && (bst_node->right->left == NULL && bst_node->right->right == NULL)) {
            tmp = bst_node->right;
            bst_node->right = NULL;
            free(tmp->data);
            free(tmp);
        } else {
            __bst_tree_remove(bst_node->right, data, f);
        }
    } else {
        if (bst_node->left == NULL && bst_node->right == NULL) {  // daca trebuie sa elimin singurul nod din arbore
            free(bst_node->data);
            free(bst_node);
            return NULL;
        } else if (bst_node->right->left == NULL) {  // daca fiul drept al nodului ce trebuie sters nu are subarbore stang
            strcpy(bst_node->data, bst_node->right->data);
            tmp = bst_node->right;
            bst_node->right = bst_node->right->right;
            free(tmp->data);
            free(tmp);
        } else if (bst_node->right->left != NULL) {  // daca fiul drept al nodului ce trebui sters are subarbore stang
            bst_node_t *parent = bst_node;
            bst_node_t *aux = bst_node->right;

            while (1) {
                parent = aux;
                aux = aux->left;
                
                if (aux->left == NULL) {  // am gasit succesorul nodului ce trebuie sters
                    strcpy(bst_node->data, aux->data);
                    parent->left = aux->right;
                    free(aux->data);
                    free(aux);
                    break;
                }
            }
        }
    }

    return bst_node;
}

void bst_tree_remove(bst_tree_t *bst_tree, char *data)
{
    bst_tree->root = __bst_tree_remove(bst_tree->root, data, bst_tree->cmp);
}

static void __bst_tree_free(bst_node_t *bst_node)
{
    if (!bst_node)
        return;

    __bst_tree_free(bst_node->left);
    __bst_tree_free(bst_node->right);
    free(bst_node->data);
    free(bst_node);
}

void bst_tree_free(bst_tree_t *bst_tree)
{
    __bst_tree_free(bst_tree->root);
    free(bst_tree);
}
