// delete 수정하기
// rotate 안될 때가 있다! 수정하기

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



bool is_empty(TREE* tree);
bool is_full(TREE* tree);
void quit();

void rotate_when(TREE* tree, NODE* node);
NODE* rotate_what(TREE* tree, NODE* n1, NODE* n2);

int balance_factor(TREE* tree, NODE* node);
int height_node(TREE* tree, NODE* node);
int height_tree(TREE* tree);
int count_node(TREE* tree);

void insert_node(TREE* tree, NODE* node, int newdata);
void print(TREE* tree, NODE* node);
void delete_node(TREE* tree, NODE* node);

NODE* get_parent(TREE* tree, NODE* node);

void RL_rotate(TREE* tree, NODE* node);
void LR_rotate(TREE* tree, NODE* node);
void left_rotate(TREE* tree, NODE* node);
void right_rotate(TREE* tree, NODE* node);

void inorder_traversal(TREE* tree, NODE* node);
void right_root_left_traversal(TREE* tree, NODE* node);

void find_node(TREE* tree, NODE* node, int isdata);
void clear(TREE* tree, NODE* node);

NODE* get_min(TREE* tree, NODE* node);
NODE* get_max(TREE* tree, NODE* node);
NODE* get_right_child(TREE* tree, NODE* node);
NODE* get_left_child(TREE* tree, NODE* node);



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

	printf("(");
	print(tree, tree->root);
	printf(")");
	printf("\n");

	if (tree->cnt >= 3) rotate_when(tree, tree->root);
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

int balance_factor(TREE* tree, NODE* node) {
	return height_node(tree, node->left) - height_node(tree, node->right);
}

NODE* get_parent(TREE* tree, NODE* node) {
	if (node == NULL) return;

	NODE* cur = tree->root;
	NODE* next = cur;

	while (next != NULL) {
		cur = next;

		if (cur->data > node->data) {
			if (cur->left == node) return cur;
			else next = cur->left;
		}

		else if (cur->data < node->data) {
			if (cur->right == node) return cur;
			else next = cur->right;
		}
	}

	printf("Error: No such node\n");
	return NULL;
}

void left_rotate(NODE** node) {
	if (*node == NULL || (*node)->left == NULL) return;

	NODE* parent = *node;
	NODE* child = parent->left;

	parent->left = child->right;
	child->right = parent;

	*node = child;
}

void right_rotate(NODE** node) {
	if (*node == NULL || (*node)->right == NULL) return;

	NODE* parent = *node;
	NODE* child = parent->right;

	parent->right = child->left;
	child->left = parent;

	*node = child;
}

void RL_rotate(NODE** node) {
	if (*node == NULL || (*node)->left == NULL) return;

	NODE* parent = *node;
	NODE* child = parent->left;

	left_rotate(&child);
	*node = child;

	right_rotate(node);
}

void LR_rotate(NODE** node) {
	if (*node == NULL || (*node)->right == NULL) return;

	NODE* parent = *node;
	NODE* child = parent->right;

	right_rotate(&child);
	*node = child;

	left_rotate(node);
}

// rotate할 노드를 반환하는 함수
NODE* rotate_what(TREE* tree, NODE* n1, NODE* n2) {
	if (n1 == NULL) return NULL;

	else if (n2 == NULL) n2 = n1->right;  // left가 NULL > right를 n2로 놓기

	NODE* n;

	// BF 절댓값 비교
	if (abs(balance_factor(tree, n1)) < abs(balance_factor(tree, n2))) n = n2;

	else if (abs(balance_factor(tree, n1)) > abs(balance_factor(tree, n2))) n = n1;

	// BF 같을 경우 더 아래 있는 걸 n으로
	else {
		if (height_node(tree, n1) < height_node(tree, n2)) n = n1;
		else n = n2;
	}

	// 회전할 노드 발견 > 그 값을 n으로
	n = rotate_what(tree, n1->left, n) ? rotate_what(tree, n1->left, n) : n;
	n = rotate_what(tree, n1->right, n) ? rotate_what(tree, n1->right, n) : n;

	return n;
}


// rotate 함수 호출하는 함수
void rotate_when(TREE* tree, NODE* parent) {
	if (parent == NULL) return;

	NODE* node = rotate_what(tree, parent, parent->left);

	int n = balance_factor(tree, node);

	if (n >= 2) {
		int leftd = balance_factor(tree, node->left);

		if (leftd >= 1) {
			left_rotate(tree, node);
		}

		else {
			LR_rotate(tree, node);
		}
	}

	else if (n <= -2) {
		int rightd = balance_factor(tree, node->right);

		if (rightd <= -1) {
			right_rotate(tree, node);
		}

		else {
			RL_rotate(tree, node);
		}
	}
}

int height_tree(TREE* tree) {
	return height_node(tree, tree->root);
}

int height_node(TREE* tree, NODE* node) {
	if (node == NULL) return -1;

	if (node->left == NULL && node->right == NULL) return 0;

	else {
		int left_height = height_node(tree, node->left);
		int right_height = height_node(tree, node->right);

		return 1 + (left_height > right_height ? left_height : right_height);
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

NODE* get_right_child(TREE* tree, NODE* node) {
	if (node == NULL) return NULL;

	if (node->right != NULL) {
		return node->right;
	}

	return NULL;
}

NODE* get_left_child(TREE* tree, NODE* node) {
	if (node == NULL) return NULL;

	if (node->left != NULL) {
		return node->left;
	}

	return NULL;
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

	if (tree->cnt >= 3) rotate_when(tree, tree->root);
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
			if (find != NULL) printf("Root");

			find_node(mytree, mytree->root, input);
			printf("\n");
		}

				break;

		case 'G': {
			NODE* find = search(mytree->root, input);

			if (get_right_child(mytree, find) == NULL) printf("NULL\n");

			else printf("%d\n", get_right_child(mytree, find)->data);
		}

				break;

		case 'L': {
			NODE* find = search(mytree->root, input);

			if (get_left_child(mytree, find) == NULL) printf("NULL\n");

			else printf("%d\n", get_left_child(mytree, find)->data);
		}

				break;

		case 'C':
			clear(mytree, mytree->root);
			printf("BST cleared\n");
			break;

		case 'T':
			printf("%d\n", height_tree(mytree));
			break;

		case 'H': {
			NODE* find = search(mytree->root, input);

			printf("%d\n", height_node(mytree, find));
		}

				break;

				// 자신만의 기능 3가지 + a
		case 'E':
			printf("%s\n", is_empty(mytree) ? "empty" : "not empty");
			break;

		case '?':
			printf("%s\n", is_full(mytree) ? "True" : "False");
			break;

		case 'Q':
			quit();
			break;

		case '@': {
			NODE* find = search(mytree->root, input);

			printf("%d\n", get_parent(mytree, find)->data);
		}

				break;

		case 'B': {
			NODE* find = search(mytree->root, input);

			printf("%d\n", balance_factor(mytree, find));
		}

				break;
		}
	}

	return 0;
}