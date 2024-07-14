#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NODE {
	char data;
	struct NODE* left_child;
	struct NODE* right_sibling;
} NODE;

typedef struct TREE {
	NODE* root;
	int cnt;
} TREE;

NODE* search(NODE* parent, char isdata) {
	if (parent == NULL)return NULL;

	if (parent->data == isdata) return parent;

	NODE* found = search(parent->left_child, isdata);
	if (found != NULL) return found;

	found = search(parent->right_sibling, isdata);
	if (found != NULL) return found;
}

bool is_empty(TREE* tree) {
	return tree == NULL || tree->root == NULL;
}

bool is_full(TREE* tree) {
	return false;
}

void quit() {
	printf("Quit\n");
	exit(0);
}

void create(TREE* tree, char newdata) {
	NODE* n = malloc(sizeof(NODE));

	tree->root = n;
	tree->root->data = newdata;
	tree->root->left_child = NULL;
	tree->root->right_sibling = NULL;
	tree->cnt = 1;
}

void insert_node(TREE* tree, NODE* node, char newdata) {
	if (tree == NULL || node == NULL) {
		printf("Error\n");
		return;
	}

	NODE* n = malloc(sizeof(NODE));
	NODE* temp = node->left_child;
	n->data = newdata;

	if (node->left_child == NULL) {
		node->left_child = n;
	}

	else if (temp->right_sibling == NULL) {
		temp->right_sibling = n;
	}

	else {
		while (temp->right_sibling != NULL) {
			temp = temp->right_sibling;
		}

		temp->right_sibling = n;
	}

	n->right_sibling = NULL;
	n->left_child = NULL;
	tree->cnt++;
}

void print(TREE* tree, NODE* parent, NODE* child) {
	NODE* n = child->left_child;

	if (parent == child) printf("%c", parent->data);

	if (parent->left_child == NULL) return;

	printf("(");

	while (n != NULL) {
		printf("%c", n->data);

		if (n->left_child != NULL) {
			print(tree, parent, n);
		}

		if (n->right_sibling != NULL) {
			printf(",");
		}

		n = n->right_sibling;
	}

	printf(")");
}

NODE* get_parent(TREE* tree, NODE* node, char isdata) {
	NODE* parent = tree->root;
	NODE* n = parent->left_child;

	if (node == tree->root) return;

	if (n == node) return parent;

	while (n != NULL) {
		if (n->right_sibling == node) {
			return n;
		}

		n = n->right_sibling;
	}

	return NULL;
}

void get_sibling(TREE* tree, char isdata) {
	NODE* find = search(tree->root, isdata);
	NODE* parent = get_parent(tree, find, isdata);
	NODE* child = parent->left_child;

	int found = 0;

	while (child != NULL) {
		if (child != find) {
			if (found) {
				printf("%c", child->data);

				if (child->right_sibling != NULL) {
					printf(",");
				}
			}
		}

		else {
			found = 1;
		}

		child = child->right_sibling;
	}

	if (!found) {
		printf("No sibling");
	}
}

void get_child(TREE* tree, NODE* parent) {
	NODE* child = parent->left_child;

	if (child == NULL) {
		printf("No child\n");
		return;
	}

	while (child != NULL) {
		printf("%c", child->data);

		if (child->right_sibling != NULL) {
			printf(",");
		}

		child = child->right_sibling;
	}
}

void get_descendants(TREE* tree, NODE* parent, NODE* child) {
	NODE* n = child->left_child;

	/*
	if (parent->left_child == NULL) {
		printf("No child");
		return;
	}
	*/

	while (n != NULL) {
		printf("%c", n->data);

		if (n->left_child != NULL) {
			print(tree, parent, n);
		}

		n = n->right_sibling;
	}
}

void get_ancestors(TREE* tree, NODE* child) {
	NODE* n = child;
	NODE* parent;
	int first = 1;

	while (child != NULL) {
		parent = get_parent(tree, tree->root, n->data);

		if (parent != NULL) {
			printf("%c", parent->data);

			if (!first) {
				printf(",");
			}

			else first = 0;
		}

		else {
			break;
		}

		n = parent;
	}
}

int count_node(TREE* tree) {
	return tree->cnt;
}

int level_of_node(TREE* tree, NODE* child) {
	int level = 0;
	NODE* n = tree->root;

	while (n != NULL) {
		if (n == child) return level;

		n = n->left_child;
		level++;
	}

	return level;
}

int level_of_tree(TREE* tree, NODE* node) {
	if (node == NULL) return;

	while (node->left_child != NULL) {
		node = node->left_child;
	}

	return level_of_node(tree, node);
}

int degree_of_node(TREE* tree, NODE* parent) {
	if (parent == NULL) return 0;

	int num = 0;
	NODE* child = parent->left_child;

	if (parent->left_child == NULL) return num;

	while (child != NULL) {
		num++;
		child = child->right_sibling;
	}

	return num;
}

int degree_of_tree(TREE* tree, NODE* node) {
	if (node == NULL) return 0;

	int max = degree_of_node(tree, node);
	NODE* child = node->left_child;

	while (child != NULL) {
		int c = degree_of_tree(tree, child);

		if (c > max) {
			max = c;
		}

		child = child->right_sibling;
	}

	return max;
}

void insert_sibling(TREE* tree, NODE* sibling, char isdata) {
	NODE* new = malloc(sizeof(NODE));
	NODE* parent = get_parent(tree, tree->root, sibling->data);
	NODE* temp = parent->left_child;
	new->data = isdata;

	if (sibling->data == tree->root->data) {
		printf("Error\n");
		return;
	}

	if (temp->right_sibling != NULL) {
		temp = temp->right_sibling;
	}

	temp->right_sibling = new;

	new->right_sibling = NULL;
	new->left_child = NULL;
	tree->cnt++;
}

TREE* join_trees(TREE* tree, TREE* new, char newdata) {
	TREE* newtree = malloc(sizeof(TREE));
	create(newtree, newdata);

	newtree->root->left_child = tree->root;
	tree->root->right_sibling = new->root;
	new->root->right_sibling = NULL;

	return newtree;
}

void clear(TREE* tree, NODE* parent) {  // parent 는 루트
	if (tree == NULL || tree->root == NULL) return;

	NODE* n = parent->left_child;
	NODE* temp;

	while (n != NULL) {
		temp = n;
		clear(tree, n);
		n = n->right_sibling;
		free(temp);
	}

	parent->left_child = NULL;
	tree->root = NULL;
	tree->cnt = 0;
}

void delete_node(TREE* tree, NODE* node) {
	if (node == NULL) {
		printf("Error\n");
		return;
	}

	NODE* parent = get_parent(tree, tree->root, node->data);
	NODE* n = parent->left_child;

	if (parent == NULL) {
		clear(tree, tree->root);
		return;
	}

	if (n == node) {
		if (node->right_sibling != NULL) {
			n = node->right_sibling;
		}
	}

	else {
		while (n->right_sibling != node) {
			n = n->right_sibling;
		}

		n->right_sibling = node->right_sibling;
	}

	free(node);

	tree->cnt--;
}

int main() {
	char choice[100];
	int b = 0;
	printf("What tree? [G: general tree / B: binary tree]\n");
	scanf("%s", choice);

	if (strcmp(choice, "G") == 0) {
		printf("General tree start\n");
	}

	else if (strcmp(choice, "B") == 0) {
		printf("Binary tree start\n");
		b = 1;
	}

	printf("\n");

	char input[100];
	TREE* mytree = malloc(sizeof(TREE));
	TREE* new = malloc(sizeof(TREE));

	while (1) {
		scanf("%s", input);

		switch (input[0]) {
		case '+':
			if (input[1] == '^') {
				create(mytree, input[2]);
			}

			if (input[1] == '=') {
				NODE* find1 = search(mytree->root, input[2]);

				if (b && degree_of_node(mytree, find1) >= 2) {
					printf("Cannot add more\n");
				}

				else {
					insert_sibling(mytree, find1, input[4]);
				}
			}

			if (input[2] == '(' && input[4] == ')') {
				NODE* find2 = search(mytree->root, input[1]);

				if (b && degree_of_node(mytree, find2) >= 2) {
					printf("Cannot add more\n");
				}

				else {
					insert_node(mytree, find2, input[3]);
				}
			}

			break;

		case '-': {
			NODE* find = search(mytree->root, input[1]);
			delete_node(mytree, find);
			break;
		}

		case 'S':
			if (input[1] == '(' && input[3] == ')') {
				printf("(");
				get_sibling(mytree, input[2]);
				printf(")");
			}

			printf("\n");

			break;

		case 'P':
			if (input[1] == '(' && input[3] == ')') {
				NODE* find = search(mytree->root, input[2]);

				if (find == NULL) {
					printf("Error\n");
					break;
				}

				NODE* parent = get_parent(mytree, find, input[2]);
				if (parent != NULL) {
					printf("%c\n", parent->data);
				}

				else {
					printf("No parent\n");
				}
			}
			break;

		case 'C':
			if (input[1] == '(' && input[3] == ')') {
				printf("(");
				NODE* find = search(mytree->root, input[2]);
				get_child(mytree, find);
				printf(")");
			}

			printf("\n");

			break;

		case 'T':
			print(mytree, mytree->root, mytree->root);
			printf("\n");
			break;

		case 'A':
			if (input[1] == '(' && input[3] == ')') {
				printf("(");
				NODE* find = search(mytree->root, input[2]);
				get_ancestors(mytree, find);
				printf(")");
			}

			printf("\n");

			break;

		case 'D':
			if (input[1] == '(' && input[3] == ')') {
				printf("(");
				NODE* find = search(mytree->root, input[2]);
				get_descendants(mytree, find, find);
				printf(")");
			}

			printf("\n");

			break;

		case '#':
			printf("%d\n", count_node(mytree));
			break;

		case 'K':
			clear(mytree, mytree->root);
			printf("Tree cleared\n");
			break;

		case 'L':
			if (input[1] == '(' && input[3] == ')') {
				NODE* find = search(mytree->root, input[2]);
				printf("%d\n", level_of_node(mytree, find));
			}

			else {
				printf("%d\n", level_of_tree(mytree, mytree->root));
			}

			break;

		case 'G':
			if (input[1] == '(' && input[3] == ')') {
				NODE* find1 = search(mytree->root, input[2]);
				printf("%d\n", degree_of_node(mytree, find1));
			}

			else {
				NODE* find2 = search(mytree->root, input[2]);
				printf("%d\n", degree_of_tree(mytree, find2));
			}

			break;

		case 'J':
			create(new, '-');
			TREE* newtree = join_trees(mytree, new, input[2]);
			print(newtree, newtree->root, newtree->root);
			printf("\n");
			break;

			// 자신만의 기능 3가지
		case 'E':
			printf("%s\n", is_empty(mytree) ? "empty" : "not empty");
			break;

		case 'F':
			printf("%s\n", is_full(mytree) ? "True" : "False");
			break;

		case 'Q':
			quit();
			break;
		}
	}

	return 0;
}