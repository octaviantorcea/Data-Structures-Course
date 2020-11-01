#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl.h"


// Creaza structura avl-ului
Avl_tree* avl_create(int (*cmp)(void*, void*)) {
	Avl_tree *tree = malloc(sizeof(Avl_tree));
	tree->compar = cmp;
	if (tree == NULL) {
		return NULL;
	}
	tree->root = NULL;
	return tree;
}

void node_free(Node **node) {
	if ((*node)->left != NULL) {
		node_free(&(*node)->left);
	}
	if ((*node)->right != NULL) {
		node_free(&(*node)->right);
	}
	Node *temp = (*node);
	if ((*node)->left == NULL && (*node)->right == NULL) {
		free(temp->data);
		(*node) = NULL;
		free(temp);
		return;
	}
}

void avl_free(Avl_tree *tree) {
	if (tree->root != NULL) {
		node_free(&tree->root);
	}
	free(tree);
}

int max(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}

/* Creaza un nod 
 * @param1: Valoarea ce trebuie pusa in nod.
 * @param2: Numarul de octeti pe care scrie valoarea.
 */
Node* node_create(void *value, int data_size) {
	Node *node = malloc(sizeof(Node));
	if (node == NULL) {
		return NULL;
	}
	node->data = malloc(data_size);

	// Copiere octet cu octet din value, in nodul curent.
    // OBS: char are 1 octet.  
    for (int i = 0; i < data_size; ++i) {
        *(char *)(node->data + i) = *(char *)(value + i);
    }

    // Nodurile frunze au inaltimea 0.
	node->height = 0;
	node->left = NULL;
	node->right = NULL;

	return node;
}

// Nodurile NULL au inaltimea -1 pentru a respecta regula:
// node->height = 1 + max(node->left->height, nod->right->height)
int height(Node *node) {
	if (node == NULL) {
		return -1;
	}
	return node->height;
}

/* Rotire dreapta
 * Pentru a nu fi nevoie sa mentinem pointer catre nodul parinte,
 * se vor folosi pointeri la noduri
 *
 *      node  			    lson
 *     /    \			   /    \ 
 *   lson    y    --->    x    node
 *   /  \     		           /   \ 
 *  x   lrson                lrson  y
 */
void rotate_right(Node **node) {
	Node *lson = (*node)->left;
	Node *lrson = lson->right;
	Node *rson = (*node);

	(*node) = lson;
	(*node)->right = rson;
	(*node)->right->left = lrson;

	// Restabilire intaltimi
	(*node)->right->height = 1 + max(height((*node)->right->left), height((*node)->right->right));
	(*node)->height = 1 + max(height((*node)->left), height((*node)->right));

}

/* Rotire stanga
 * Pentru a nu fi nevoie sa mentinem pointer catre nodul parinte,
 * se vor folosi pointeri la noduri
 *
 *     node  			    rson
 *    /    \			   /    \ 
 *   x     rson    --->  node    y
 *         /   \		 /   \
 *       rlson  y       x   rlson
 */	
void rotate_left(Node **node) {
	Node *rson = (*node)->right;
	Node *rlson = rson->left;
	Node *lson = (*node);
	
	(*node) = rson;
	(*node)->left = lson;
	(*node)->left->right = rlson;

	// Restabilire inaltimi
	(*node)->left->height = 1 + max(height((*node)->left->left), height((*node)->left->right));
	(*node)->height = 1 + max(height((*node)->left), height((*node)->right));

}

// Echilibrarea AVL-ului
void avl_fix(Node **node) {
	// TODO: Completati rotatiile corespunzatoare
	if (height((*node)->left) > height((*node)->right) + 1 
		&& height((*node)->left->left) >= height((*node)->left->right)) {
		/* Rotatie RR
		 *
		 *     node                  a
		 *     /                    / \
		 *    a          --->      b   node
		 *   /
		 *  b
		 *
		 */
		rotate_right(node);

	} else if (height((*node)->right) > height((*node)->left) + 1 
				&& height((*node)->right->right) >= height((*node)->right->left)) {
		/* Rotatie LL
		 *
		 *     node                a
		 *        \               / \
		 *         a     --->  node  b
		 *          \ 
		 *           b
		 *
		 */
		rotate_left(node);

	} else if (height((*node)->left) > height((*node)->right) + 1 
				&& height((*node)->left->left) < height((*node)->left->right)) {
		/* Rotatie LR
		 *
		 *     node                node               b
		 *     /        left       /      right      / \
		 *    a         --->      b        --->     a   node
		 *     \                 /
		 *      b               a
		 *
		 */
		rotate_left(&((*node)->left));
		rotate_right(node);


	} else if (height((*node)->right) > height((*node)->left) + 1 
				&& height((*node)->right->right) < height((*node)->right->left)) {

		/* Rotatie RL
		 *
		 *     node                node                  b
		 *        \      right        \       left      / \
		 *         a      --->         b       --->   node a
		 *        /                     \
		 *       b                       a
		 *
		 */
		rotate_right(&((*node)->right));
		rotate_left(node);
	}
}

/* Inserare in AVL
 * 
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in AVL.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din AVL.
 */
void avl_insert(Node **node, void *value, int data_size, int (*compar)(void*, void*)) {
	if ((*node) == NULL) {
		(*node) = node_create(value, data_size);
		return;
	} else if(compar((*node)->data, value) > 0) {
		avl_insert(&(*node)->left, value, data_size, compar);
	} else {
		avl_insert(&(*node)->right, value, data_size, compar);
	}

	(*node)->height = 1 + max(height((*node)->left), height((*node)->right));
	avl_fix(node);
}

void* get_key(Node *node, void *value, int data_size, int (*compar)(void*, void*)) {
	if (node == NULL) {
		return NULL;
	}

	if (compar(value, node->data) == 1) {
		return get_key(node->right, value, data_size, compar);
	} else if (compar(value, node->data) == -1) {
		return get_key(node->left, value, data_size, compar);
	} else {
		return value;
	}
}

// Elementul maxim din subarbore
Node *max_element(Node *node) {
	if (node->right == NULL) {
		return node;
	}
	return max_element(node->right);
}

/* Stergere din AVL
 * 
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in AVL.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din AVL.
 */
void avl_delete(Node **node, void *value, int data_size, int (*compar)(void*, void*)) {
	if ((*node) == NULL) {
		return;
	} else if (compar((*node)->data, value) > 0) {
		avl_delete(&(*node)->left, value, data_size, compar);
	} else if (compar((*node)->data, value) < 0) {
		avl_delete(&(*node)->right, value, data_size, compar);
	} else {
		if ((*node)->left == NULL && (*node)->right == NULL) {  // nu are niciun fiu
			Node *aux = (*node);
			(*node) = NULL;
			free(aux->data);
			free(aux);
			return;
		} else if ((*node)->left == NULL && (*node)->right != NULL) {  // are fiu dreapta
			Node *aux = (*node)->right;
			free((*node)->data);
			free(*node);
			*node = aux;
		} else if ((*node)->left != NULL && (*node)->right == NULL) {  // are fiu stanga
			Node *aux = (*node)->left;
			free((*node)->data);
			free(*node);
			*node = aux;
		} else {
			Node *max_node = max_element((*node)->left);
			
			for (int i = 0; i < data_size; ++i) {
        		*(char *)((*node)->data + i) = *(char *)(max_node->data + i);
    		}

			avl_delete(&(*node)->left, max_node->data, data_size, compar);
		}
	}

	(*node)->height = 1 + max(height((*node)->left), height((*node)->right));
	avl_fix(node);
}

void preorder_traversal(Node *node) {
	if (node == NULL) {
		return;
	}
	printf("%d ", *(int*)node->data);
	preorder_traversal(node->left);
	preorder_traversal(node->right);
}
