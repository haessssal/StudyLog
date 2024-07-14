#include <stdio.h>
#include <string.h>

int idx = -1;
int length = 0;
char arr[30];

void state() {
    printf("Array: %s / Index: %d\n", arr, idx);
}

void insert(char arr[], char data) {
    idx++;
    length = strlen(arr);

    if (idx < 0) idx = 0;

    for (int i = length; i >= idx; i--) {
        arr[i] = arr[i - 1];
    }

    arr[idx] = data;
    length++;
    state();
}

void gofront(char arr[]) {
    idx = 0;
}

void gorear(char arr[]) {
    idx = length - 1;
}

void gonext(char arr[]) {
    if (idx < length - 1) idx++;
    else {
        printf("can't go next\n");
        return;
    }
    state();
}

void goprevious(char arr[]) {
    if (idx > 0) idx--;
    else {
        printf("can't go previous");
        return;
    }
    state();
}

void print(char arr[]) {
    printf("%s\n", arr);
}

void delete(char arr[]) {
    if (idx >= 0 && idx < length) {
        for (int i = idx; i < length - 1; i++) {
            arr[i] = arr[i + 1];
        }
        arr[length - 1] = '\0';
        length--;
        if (idx >= length) idx = 0;
    }
    state();
}

void get_data(char arr[]) {
    if (idx >= 0 && idx < length) {
        printf("%c\n", arr[idx]);
    }
}

void replace(char arr[], char data) {
    if (idx >= 0 && idx < length) {
        arr[idx] = data;
    }
    printf("%s\n", arr);
}

void move(char arr[], int n) {
    char temp;

    if (idx > n) {
        for (int i = idx; i > n; i--) {
            temp = arr[i - 1];
            arr[i - 1] = arr[i];
            arr[i] = temp;
        }
    }

    else if (idx < n) {
        for (int i = idx; i < n; i++) {
            temp = arr[i + 1];
            arr[i + 1] = arr[i];
            arr[i] = temp;
        }
    }

    idx = n;
    printf("%s\n", arr);
}

// 데이터 개수(인덱스 + 1)
int data_count(char arr[]) {
    return length;
}

void empty(char arr[]) {
    for (int i = 0; i < length; i++) {
        arr[i] = NULL;
    }
    idx = -1;
}

// 자신만의 기능 추가
void howlong(char arr[]) {
    printf("%d\n", length + 1);
}

void isempty(char arr[]) {
    if (length == 0) printf("empty\n");
    else printf("not empty\n");
}

void reverse(char arr[]) {
    char temp;

    for (int i = 0; i < (length + 1) / 2; i++) {
        temp = arr[i];
        arr[i] = arr[(length)-i];
        arr[(length)-i] = temp;
    }

    printf("%s\n", arr);
}

int main() {
    char input[30];

    while (1) {
        scanf("%s", input);

        for (int i = 0; i < strlen(input); i++) {
            switch (input[i]) {
            case '+':
                insert(arr, input[i + 1]);
                break;

            case '<':
                gofront(arr);
                break;

            case '>':
                gorear(arr);
                break;

            case 'N':
                gonext(arr);
                break;

            case 'P':
                goprevious(arr);
                break;

            case 'L':
                print(arr);
                break;

            case '-':
                delete(arr);
                break;

            case'@':
                get_data(arr);
                break;

            case '=':
                replace(arr, input[i + 1]);
                break;

            case 'n':
                printf("%d\n", data_count(arr));
                break;

            case 'M':
                if (input[i + 1] == 'n') {
                    move(arr, length - 1);
                }
                else if (input[i + 1] == 'P') {
                    move(arr, idx - 1);
                }
                else if (input[i + 1] == 'N') {
                    move(arr, idx + 1);
                }
                else {
                    move(arr, atoi(&input[i + 1]));
                }
                i++;
                state();
                break;

            case 'E':
                empty(arr);
                printf("Array cleared\n");
                break;

            case 'H':
                howlong(arr);
                break;

            case '!':
                isempty(arr);
                break;

            case 'R':
                reverse(arr);
                break;
            }
        }
    }
    return 0;
}