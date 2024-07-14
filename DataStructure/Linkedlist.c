#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct NODE {
    char data;
    struct NODE* next;
} NODE;

typedef struct LIST {
    NODE* head;
    NODE* pos;
    int size;
    int now;
} LIST;

LIST* create() {
    LIST* list = malloc(sizeof(LIST));

    if (list == NULL) {
        printf("Not enough memory");
        exit(EXIT_FAILURE);
    }

    list->size = 0;
    list->now = 0;
    list->head = NULL;
    list->pos = NULL;

    return list;
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

void addTail(LIST* list, char newdata);
void add(LIST* list, char A, char newdata);
void print(LIST* list);
void delete(LIST* list);

bool is_empty(LIST* list);
bool is_full(LIST* list);
void is_member(LIST* list, char isdata);

void gonext(LIST* list);
void goprevious(LIST* list);
void traverse_front(LIST* list);
void traverse_rear(LIST* list);

void get_data(LIST* list, int index); // 수정된 부분
void data_count(LIST* list);
void replace(LIST* list, int index, char newdata); // 수정된 부분
void clear(LIST* list);
void leftnright(LIST* list);
void reverse(LIST* list);

void addTail(LIST* list, char newdata) {
    NODE* n = newnode(newdata);

    if (list->size == 0) {
        n->next = n;
        list->head = n;
        list->pos = n;
        n->next = NULL;
    }
    else {
        NODE* last = list->head;
        while (last->next != NULL) {
            last = last->next;
        }

        last->next = n;
        list->pos = n;
    }

    list->size++;
    list->now = list->size - 1;
}

bool is_empty(LIST* list) {
    return list == NULL || list->pos == NULL;
}

void print(LIST* list) {
    NODE* n = list->head;

    if (is_empty(list)) {
        printf("List is empty\n");
        return;
    }

    while (n != NULL) {
        printf("%c", n->data);
        n = n->next;
    }

    printf("\n");
}

void delete(LIST* list) {
    if (is_empty(list)) {
        printf("List is empty\n");
        return;
    }

    NODE* del = list->pos;
    NODE* prev = NULL;
    NODE* n = list->head;

    if (del == list->head) {
        list->head = del->next;
        list->pos = list->head;
    }
    else {
        while (n->next != del) {
            n = n->next;
        }
        prev = n;
        prev->next = del->next;
        if (del->next == NULL) {
            list->pos = prev;
        }
        else {
            list->pos = del->next;
        }
    }

    free(del);
    list->size--;

    if (list->size == 0) {
        list->pos = NULL;
        list->now = 0;
    }

    print(list);
}

void gonext(LIST* list) {
    if (list->pos != NULL && list->pos->next != NULL) {
        list->pos = list->pos->next;
        list->now++;
    }
}

// 인덱스를 받아서 해당 위치의 데이터를 출력
void get_data(LIST* list, int index) {
    if (index < 0 || index >= list->size) {
        printf("Index out of bounds\n");
        return;
    }
    NODE* n = list->head;
    for (int i = 0; i < index; i++) {
        n = n->next;
    }
    printf("%c\n", n->data);
}

// 인덱스를 받아서 해당 위치의 데이터를 새로운 데이터로 교환
void replace(LIST* list, int index, char newdata) {
    if (index < 0 || index >= list->size) {
        printf("Index out of bounds\n");
        return;
    }
    NODE* n = list->head;
    for (int i = 0; i < index; i++) {
        n = n->next;
    }
    n->data = newdata;
}

void clear(LIST* list) {
    NODE* n = list->head;
    NODE* next;

    while (n != NULL) {
        next = n->next;
        free(n);
        n = next;
    }

    list->head = NULL;
    list->pos = NULL;
    list->size = 0;
    list->now = 0;
}

void traverse_front(LIST* list) {
    list->pos = list->head;
    list->now = 0;
}

void traverse_rear(LIST* list) {
    NODE* n = list->head->next;

    int num = 1;
    while (n->next != NULL) {
        n = n->next;
        num++;
    }

    n->next = NULL;
    list->pos = n;
    list->now = num;
}

void data_count(LIST* list) {
    printf("%d\n", list->size);
}

void is_member(LIST* list, char isdata) {
    NODE* n = list->head;
    int num = 1;
    bool found = false;

    for (int i = 0; i < list->size; i++) {
        if (isdata == n->data) {
            found = true;
            break;
        }

        n = n->next;
        num++;
    }

    if (found) printf("%d\n", num);
    else printf("No such data\n");
}

void add(LIST* list, char A, char newdata) {
    NODE* n = newnode(newdata);

    if (A == 'A') {
        if (list->pos == list->head && list->now == 0) {
            n->next = list->head;
            list->head = n;
            list->pos = n;
        }
        else {
            NODE* cur = list->pos;
            n->next = cur->next;
            cur->next = n;
            list->pos = n;
        }
        list->size++;
        list->now++;
    }
}

void goprevious(LIST* list) {
    NODE* n = list->head;

    for (int i = 0; i < list->now; i++) {
        n = n->next;
    }

    n->next = list->pos;
    list->pos = n;
    list->now--;
}

bool is_full(LIST* list) {
    return false;
}

void leftnright(LIST* list) {
    if (list->now == 0) {
        printf("No left data\n");
    }
    else {
        NODE* cur = list->head;
        for (int i = 0; i < list->now - 1; i++) {
            cur = cur->next;
        }
        printf("Left:%c\n", cur->data);
    }

    if (list->now == list->size) {
        printf("No right data\n");
    }
    else {
        printf("Right:%c\n", list->pos->next->data);
    }
}

void reverse(LIST* list) {
    NODE* prev = NULL;
    NODE* next = NULL;
    NODE* n = list->head;

    while (n != NULL) {
        next = n->next;
        n->next = prev;
        prev = n;
        n = next;
    }

    list->head = prev;
    list->pos = prev;
}

int main() {
    LIST* mylist = create();
    char input[30];
    int num;
    char data;

    while (1) {
        scanf("%s", input);

        for (int i = 0; i < strlen(input); i++) {
            if (isdigit(input[i]) && (input[i + 1] == 'G' || input[i + 1] == '=')) {
                int index = input[i] - '0' - 1;
                if (input[i + 1] == 'G') {
                    get_data(mylist, index); // 인덱스를 get_data에 전달
                    i++; 
                }

                else if (input[i + 1] == '=') {
                    replace(mylist, index, input[i + 2]); 
                    i += 2; 
                }
            }

            else {
                switch (input[i]) {
                case '+':
                    addTail(mylist, input[i + 1]);
                    i++;
                    break;

                case 'E':
                    printf("%s\n", is_empty(mylist) ? "empty" : "not empty");
                    break;

                case '-':
                    delete(mylist);
                    break;

                case 'C':
                    clear(mylist);
                    printf("List cleared\n");
                    break;

                case 'L':
                    print(mylist);
                    break;

                case '<':
                    traverse_front(mylist);
                    break;

                case '>':
                    traverse_rear(mylist);
                    break;

                case 'N':
                    gonext(mylist);
                    break;

                case 'P':
                    goprevious(mylist);
                    break;

                case 'G':
                    get_data(mylist, mylist->now);
                    break;

                case 'H':
                    leftnright(mylist);
                    break;

                case '?':
                    is_member(mylist, input[i + 1]);
                    i++;
                    break;

                case 'R':
                    reverse(mylist);
                    break;

                case '#':
                    data_count(mylist);
                    break;

                case 'A':
                    add(mylist, 'A', input[i + 1]);
                    i++;
                    break;

                case 'F':
                    printf("%s\n", is_full(mylist));
                    break;
                }
            }
        }
    }

    return 0;
}
