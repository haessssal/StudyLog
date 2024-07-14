#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NODE {
	char data;
	struct NODE* next;
} NODE;

typedef struct QUEUE {
	NODE* front;
	NODE* rear;
	int size;
	int head;
	int tail;
} QUEUE;

QUEUE* create() {
	QUEUE* que = malloc(sizeof(QUEUE));

	if (que == NULL) {
		printf("Not enough memory");
		exit(EXIT_FAILURE);
	}

	que->size = 0;
	que->head = -1;
	que->tail = -1;
	que->front = NULL;
	que->rear = NULL;

	return que;
}

NODE* newnode(char data) {
	NODE* n = malloc(sizeof(NODE));

	if (n == NULL) {
		printf("Not enough memory\n");
		exit(EXIT_FAILURE);
	}

	n->data = data;
	n->next = NULL;

	return n;
}

bool is_empty(QUEUE* que) {
	return que == NULL || que->front == NULL;
}

bool is_full(QUEUE* que) {
	return false;
}

bool is_member(QUEUE* que, char isdata) {
	NODE* n = que->front;

	while (n) {
		if (n->data == isdata) {
			return true;
		}

		n = n->next;
	}

	return false;
}

void enqueue(QUEUE* que, char newdata) {
	NODE* n = newnode(newdata);

	if (que->size == 0) {
		que->front = n;
		que->rear = n;
	}
	else {
		que->rear->next = n;
		que->rear = n;
	}

	que->tail++;
	que->size++;
}

void dequeue(QUEUE* que) {
	if (que->size == 0) {
		printf("Error (nothing to dequeue)\n");
		que->front = NULL;
		que->rear = NULL;
	}

	else {
		NODE* del = que->front;
		int data = del->data;

		que->front = que->front->next;

		free(del);
		que->head++;
		que->size--;
	}
}

int peek(QUEUE* que) {
	if (is_empty(que)) return 0;
	return que->front->data;
}

int data_count(QUEUE* que) {
	if (is_empty(que)) return 0;
	return que->size;
}

void print(QUEUE* que) {
	NODE* n = que->front;

	if (is_empty(que)) {
		printf("Queue is empty\n");
		return;
	}

	for (int i = 0; i < que->size; i++) {
		printf("%c", n->data);
		n = n->next;
	}

	printf("\n");
}

void clear(QUEUE* que) {
	NODE* n = NULL;

	while (que->front != NULL) {
		n = que->front;
		que->front = n->next;
		free(n);
	}

	free(que->front);
	que->size = 0;
	que->front = NULL;
	que->rear = NULL;
}

void replace(QUEUE* que, char newdata) {
	que->rear->data = newdata;
}

int head(QUEUE* que) {
	if (is_empty(que)) return 0;
	return que->head;
}

int tail(QUEUE* que) {
	if (is_empty(que)) return 0;
	return que->tail;
}

// 자신만의 기능 3가지
void reverse(QUEUE* que) {
	NODE* n = que->front;
	NODE* prev = NULL;
	NODE* next = NULL;

	while (n != NULL) {
		next = n->next;
		n->next = prev;
		prev = n;
		n = next;
	}

	que->front = prev;
}

// 해당 위치의 데이터가 무엇인지
// W2, W6과 같이 입력
void whatdata(QUEUE* que, int num) {
	if (num <= 0 || num > que->size) {
		printf("Write correct number\n");
		return;
	}

	NODE* n = que->front;
	int cnt = 1;

	while (cnt < num) {
		n = n->next;
		cnt++;
	}

	printf("%c\n", n->data);
}

int quit(QUEUE* que) {
	return 0;
}

int main() {
	QUEUE* myque = create();
	char input[100];

	while (1) {
		scanf("%s", input);

		for (int i = 0; i < strlen(input); i++) {
			switch (input[i]) {
			case '+':
				enqueue(myque, input[i + 1]);
				break;

			case '-':
				dequeue(myque);
				break;

			case 'L':
				print(myque);
				break;

			case 'P':
				printf("%c\n", peek(myque));
				break;

			case 'F':
				printf("%s\n", is_full(myque) ? "True" : "False");
				break;

			case '#':
				printf("%d\n", data_count(myque));
				break;

			case 'H':
				printf("%d\n", head(myque) + 2);
				break;

			case 'T':
				printf("%d\n", tail(myque) + 1);
				break;

			case 'E':
				printf("%s\n", is_empty(myque) ? "True" : "False");
				break;

			case '?':
				printf("%s\n", is_member(myque, input[i + 1]) ? "True" : "False");
				break;

			case '=':
				replace(myque, input[i + 1]);
				break;

			case 'C':
				clear(myque);
				break;

			case 'R':
				reverse(myque);
				break;

			case 'W':
				whatdata(myque, input[i + 1] - '0');
				break;

			case 'Q':
				quit(myque);
				return 0;
				break;
				
			default:
				if (input[i] >= '0' && input[i] <= '9') {
					int n = input[i] - '0';
					if (input[i + 1] == '-') {
						i++; // '-'를 스킵
						for (int j = 0; j < n; j++) {
							char top = peek(myque);
							if (top != '\0') {
								printf("Dequeue: %c\n", top);
							}
							dequeue(myque);
						}
					}
				}

				break;
			}
		}
	}

	return 0;
}