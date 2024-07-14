#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct NODE {
	int data;
	struct NODE* left;
	struct NODE* right;
} NODE;

typedef struct TREE {
	NODE* root;
	int cnt;
} TREE;

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

NODE* search(NODE* node, int isdata) {
	if (node == NULL) return NULL;

	else if (node->data == isdata) return node;

	else {
		NODE* found = search(node->left, isdata);
		if (found != NULL) return found;

		found = search(node->right, isdata);
		if (found != NULL) return found;
	}

	return NULL;
}

void create(TREE* tree) {
	NODE* n = malloc(sizeof(NODE));
	tree->root = n;
	tree->root->left = NULL;
	tree->root->right = NULL;
	tree->root->data = '\0';
	tree->cnt = 0;
}

void insert_node(TREE* tree, NODE* node, int newdata) {
	NODE* n = malloc(sizeof(NODE));
	n->data = newdata;
	n->left = NULL;
	n->right = NULL;

	if (tree->cnt == 0) {
		tree->root = n;
	}

	while (node != NULL) {
		if (newdata < node->data) {
			if (node->left == NULL) {
				node->left = n;
				break;
			}

			else {
				node = node->left;
				continue;
			}
		}

		else if (newdata > node->data) {
			if (node->right == NULL) {
				node->right = n;
				break;
			}

			else {
				node = node->right;
				continue;
			}
		}

		else {
			printf("Error: Same data\n");
			free(n);
			return;
		}
	}

	tree->cnt++;
}

void print(TREE* tree, NODE* node) {
	if (node == NULL) {
		printf("No data");
		return;
	}

	printf("%d", node->data);

	if (node->left != NULL || node->right != NULL) {
		printf("(");

		if (node->left != NULL) {
			print(tree, node->left);
		}

		printf(",");

		if (node->right != NULL) {
			print(tree, node->right);
		}

		printf(")");
	}

}

void inorder_traversal(TREE* tree, NODE* node) {
	if (node == NULL) return;

	inorder_traversal(tree, node->left);
	printf("%d ", node->data);
	inorder_traversal(tree, node->right);
}

void right_root_left_traversal(TREE* tree, NODE* node) {
	if (node == NULL) return;

	right_root_left_traversal(tree, node->right);
	printf("%d ", node->data);
	right_root_left_traversal(tree, node->left);
}

NODE* get_min(TREE* tree, NODE* node) {
	if (node == NULL) return;

	while (node->left != NULL) {
		node = node->left;
	}

	return node;
}

NODE* get_max(TREE* tree, NODE* node) {
	if (node == NULL) return;

	while (node->right != NULL) {
		node = node->right;
	}

	return node;
}

void find_node(TREE* tree, NODE* node, int isdata) {
	if (node == NULL) return;

	NODE* find = search(node, isdata);

	if (find == NULL) {
		printf("Error: No such data");
	}

	else {
		if (node->data < isdata) {
			if (node->right == NULL) return;

			printf(" > Right");
			find_node(tree, node->right, isdata);
		}

		else if (node->data > isdata) {
			if (node->left == NULL) return;

			printf(" > Left");
			find_node(tree, node->left, isdata);
		}

		// node->data == isdata
		else {
			return;
		}
	}
}

void delete_node(TREE* tree, NODE* node, int isdata) {
	if (node == NULL) return;

	NODE* del = node;
	NODE* parent = NULL;

	while (del != NULL && del->data != isdata) {
		parent = del;

		if (isdata < del->data) del = del->left;
		else del = del->right;
	}

	if (del == NULL) return;

	// 루트 지우기
	if (del == tree->root) {
		// 루트에 자식이 없는 경우
		if (del->left == NULL && del->right == NULL) {
			tree->root = NULL;
			free(del);
		}

		// 루트가 한쪽 자식만 가진 경우
		else if (del->left == NULL || del->right == NULL) {
			tree->root = del->left != NULL ? del->left : del->right;
			free(del);
		}

		// 루트가 좌우 자식을 모두 가진 경우
		else {
			NODE* max = get_max(tree, del->left);

			del->data = max->data;

			delete_node(tree, del->left, max->data);
		}
	}

	// 루트 외 노드 지우기
	else {
		if (parent != NULL) {
			// 단말 노드인 경우
			if (del->left == NULL && del->right == NULL) {
				if (parent->left == del) parent->left = NULL;
				else parent->right = NULL;

				free(del);
			}

			// 한쪽 자식만 가진 경우
			else if (del->left == NULL) {
				if (parent->left == del) parent->left = del->right;
				else parent->right = del->right;

				free(del);
			}

			else if (del->right == NULL) {
				if (parent->left == del) parent->left = del->left;
				else parent->right = del->left;

				free(del);
			}

			// 좌우 자식을 모두 가진 경우
			else {
				NODE* max = get_max(tree, del->left); 

				del->data = max->data;

				delete_node(tree, del->left, max->data);
			}
		}
		
		else {
			printf("Error(delete)\n");
			return;
		}
	}

	tree->cnt--;
}

int height(TREE* tree, NODE* node) {
	if (node == NULL) return -1;

	int left_height = height(tree, node->left);
	int right_height = height(tree, node->right);

	return 1 + (left_height > right_height ? left_height : right_height);
}

int get_right_child(TREE* tree, NODE* node) {
	if (node == NULL) return;

	if (node->right != NULL) {
		return node->right->data;
	}

	return -1;
}

int get_left_child(TREE* tree, NODE* node) {
	if (node == NULL) return;

	if (node->left != NULL) {
		return node->left->data;
	}

	return -1;
}

int count_node(TREE* tree) {
	return tree->cnt;
}

void clear(TREE* tree, NODE* node) {
	if (node == NULL) return;

	clear(tree, node->left);
	clear(tree, node->right);

	free(node);
	tree->root = NULL;
	tree->cnt = 0;
}

int main() {
	printf("모든 실행에서 <<< 명령기호 + int(숫자) >>> 입력하기!\n\n");

	char op[1];
	int input;

	TREE* mytree = malloc(sizeof(TREE));
	create(mytree);

	while (1) {
		scanf("%c%d", op, &input);
		getchar();

		switch (op[0]) {
		case '+':
			insert_node(mytree, mytree->root, input);
			break;

		case 'P':
			printf("(");
			print(mytree, mytree->root);
			printf(")");
			printf("\n");
			break;

		case '#':
			printf("%d\n", count_node(mytree));
			break;

		case 'I':
			inorder_traversal(mytree, mytree->root);
			printf("\n");
			break;

		case 'R':
			right_root_left_traversal(mytree, mytree->root);
			printf("\n");
			break;

		case 'N':
			printf("%d\n", get_min(mytree, mytree->root)->data);
			break;

		case 'X':
			printf("%d\n", get_max(mytree, mytree->root)->data);
			break;

		case 'F': {
			NODE* find = search(mytree->root, input);
			if (find != NULL) {
				printf("Root");
			}

			find_node(mytree, mytree->root, input);
			printf("\n");
		}
				break;

		case '-':
			delete_node(mytree, mytree->root, input);
			break;

		case 'H':
			printf("%d\n", height(mytree, mytree->root));
			break;

		case 'G': {
			NODE* find = search(mytree->root, input);

			if (get_right_child(mytree, find) == -1) printf("NULL\n");

			printf("%d\n", get_right_child(mytree, find));
		}
				break;

		case 'L': {
			NODE* find = search(mytree->root, input);

			if (get_left_child(mytree, find) == -1) printf("NULL\n");

			printf("%d\n", get_left_child(mytree, find));
		}
				break;

		case 'C':
			clear(mytree, mytree->root);
			printf("BST cleared\n");
			break;

			// 자신만의 기능 3가지
		case 'E':
			printf("%s\n", is_empty(mytree) ? "empty" : "not empty");
			break;

		case '?':
			printf("%s\n", is_full(mytree) ? "True" : "False");
			break;

		case 'Q':
			quit();
			break;
		}
	}

	return 0;
}