#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct NODE {
	char data;
	struct NODE* next; // 앞부분을 가리키는 next
} NODE;

typedef struct STACK {
	NODE* top;
	int size;
} STACK;

STACK* create() {
	STACK* stk = malloc(sizeof(STACK));

	if (stk == NULL) {
		printf("Not enough memory");
		exit(EXIT_FAILURE);
	}

	stk->size = 0;
	stk->top = NULL;

	return stk;
}

NODE* newnode(char data) {
	NODE* new = malloc(sizeof(NODE));

	if (new == NULL) {
		printf("Not enough memory\n");
		exit(EXIT_FAILURE);
	}

	new->data = data;
	new->next = NULL;

	return new;
}

bool is_empty(STACK* stk) {
	return stk == NULL || stk->top == NULL;
}

bool is_member(STACK* stk, char isdata) {
	NODE* n = stk->top;

	while (n) {
		if (n->data == isdata) {
			return true;
		}

		n = n->next;
	}

	return false;
}

bool is_full(STACK* stk) {
	return false;
}

void push(STACK* stk, char newdata) {
	NODE* n = newnode(newdata);

	n->next = stk->top;
	stk->top = n;
	stk->size++;
}

void pop(STACK* stk) {
	if (stk->size == 0) {
		printf("Stack is empty\n");
		return;
	}

	NODE* del = stk->top;
	stk->top = stk->top->next;

	free(del);
	stk->size--;
}

void clear(STACK* stk) {
	NODE* n = NULL;

	while (stk->top != NULL) {
		n = stk->top;
		stk->top = n->next;
		free(n);
	}

	free(stk->top);
	stk->size = 0;
}

char peek(STACK* stk) {
	if (is_empty(stk)) return '\0';
	return stk->top->data;
}

void print(STACK* stk) {
	if (is_empty(stk)) {
		printf("Stack is empty\n");
		return;
	}

	STACK* rev = create();

	while (!is_empty(stk)) {
		push(rev, peek(stk));
		pop(stk);
	}

	while (!is_empty(rev)) {
		push(stk, peek(rev));
		printf("%c", peek(rev));
		pop(rev);
	}

	printf("\n");

	clear(rev);
}

int element_count(STACK* stk) {
	if (is_empty(stk)) return 0;
	return stk->size;
}

void top(STACK* stk) {
	if (is_empty(stk)) {
		printf("Stack is empty\n");
		return;
	}

	printf("( %d, %c )\n", stk->size, stk->top->data);
}

void replace(STACK* stk, char newdata) {
	stk->top->data = newdata;
}

void reverse(STACK* stk) {
	NODE* n = stk->top;

	while (n) {
		printf("%c", n->data);
		n = n->next;
	}

	printf("\n");
}

// 해당 위치의 데이터가 무엇인지
void whatdata(STACK* stk, int num) {
	if (num <= 0 || num > stk->size) {
		printf("Write correct number");
		return;
	}

	NODE* n = stk->top;
	int cnt = 0;

	while (cnt < stk->size - num) {
		n = n->next;
		cnt++;
	}

	printf("%c\n", n->data);
}

int main() {
	STACK* mystack = create();
	char input[100];

	while (1) {
		scanf("%s", input);

		for (int i = 0; i < strlen(input); i++) {
			switch (input[i]) {
			case '+':
				push(mystack, input[i + 1]);
				break;

			case '-': {
				char top = peek(mystack);
				if (top != '\0') {
					printf("Pop: %c\n", top);
				}
				pop(mystack);

				break;
			}

			case 'L':
				print(mystack);
				break;

			case 'P':
				printf("%c\n", peek(mystack));
				break;

			case 'F':
				printf("%s\n", is_full(mystack) ? "True" : "False");
				break;

			case '#':
				printf("%d\n", element_count(mystack));
				break;

			case 'T':
				top(mystack);
				break;

			case '?':
				printf("%s\n", is_member(mystack, input[i + 1]) ? "True" : "False");
				break;

			case '=':
				replace(mystack, input[i + 1]);
				break;

			case 'C':
				clear(mystack);
				printf("Stack cleared\n");
				break;

				// 자신만의 기능 3가지
			case 'E':
				printf("%s\n", is_empty(mystack) ? "empty" : "not empty");
				break;

			case 'R':
				reverse(mystack);
				break;

			case 'W':
				whatdata(mystack, input[i + 1] - '0');
				break;

			default:
				if (input[i] >= '0' && input[i] <= '9') {
					int n = input[i] - '0';
					if (input[i + 1] == '-') {
						i++; // '-'를 스킵
						for (int j = 0; j < n; j++) {
							char top = peek(mystack);
							if (top != '\0') {
								printf("Pop: %c\n", top);
							}
							pop(mystack);
						}
					}
				}

				break;
			}
		}
	}

	return 0;
}