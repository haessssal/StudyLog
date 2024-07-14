#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct VERTEX {
	char data;
	int visited;
	int cnt_edge;
	struct EDGE* connect;  // 정점의 인접 리스트
	struct VERTEX* next;
} VERTEX;

typedef struct GRAPH {
	int cnt_vertex;
	VERTEX* head;
} GRAPH;

typedef struct EDGE {
	struct VERTEX* vertex;
	struct EDGE* next;
} EDGE;

void intro() {
	printf("***\n\n");
	printf("This is a code defining a GRAPH data structure.\n\n");

	printf("GRAPH  : Represents a data structure containing vertices and edges, used to model relationships between entities.\n");
	printf("VERTEX : A node within a graph that represents an entity or an object.\n");
	printf("EDGE   : A connection between two vertices in a graph that represents a relationship or interaction between them.\n\n");
	printf("***\n\n");
}

void create(GRAPH* graph) {
	graph->head = NULL;
	graph->cnt_vertex = 0;
}

VERTEX* search(GRAPH* graph, char data) {
	VERTEX* cur = graph->head;

	while (cur != NULL) {
		if (cur->data == data) return cur;
		cur = cur->next;
	}

	return NULL;
}



void insert_vertex(GRAPH* graph, char newdata);
void delete_vertex(GRAPH* graph, char isdata);
void rename_vertex(GRAPH* graph, char data1, char data2);

void add_edge(GRAPH* graph, char data1, char data2);
void delete_edge(GRAPH* graph, char data1, char data2);

void print(GRAPH* graph);
void adjacent(GRAPH* graph, char isdata);
void clear(GRAPH* graph);
void quit();

bool is_empty(GRAPH* graph);
bool is_full(GRAPH* graph);
bool is_connected(GRAPH* graph);
bool path_exist(GRAPH* graph, char data1, char data2);

int degree_of_vertex(GRAPH* graph, char isdata);
int num_of_vertex(GRAPH* graph);
int num_of_edge(GRAPH* graph);



bool is_empty(GRAPH* graph) {
	return graph == NULL || graph->head == NULL;
}

bool is_full(GRAPH* graph) {
	return false;
}

void dfs(VERTEX* v, int* count) {
	if (v->visited) return;
	v->visited = 1;
	(*count)++;
	EDGE* edge = v->connect;
	while (edge != NULL) {
		dfs(edge->vertex, count);
		edge = edge->next;
	}
}

bool is_connected(GRAPH* graph) {
	if (is_empty(graph)) return false;

	// 모든 정점의 방문 여부를 초기화
	VERTEX* cur = graph->head;
	while (cur != NULL) {
		cur->visited = 0;
		cur = cur->next;
	}

	int count = 0;
	dfs(graph->head, &count);
	return count == graph->cnt_vertex;
}

bool path_exist(GRAPH* graph, char data1, char data2) {
	VERTEX* start = search(graph, data1);
	VERTEX* end = search(graph, data2);

	if (start == NULL || end == NULL) return false;

	start->visited = 1;

	if (start == end) return true;

	EDGE* edge = start->connect;
	while (edge) {
		if (!edge->vertex->visited) {
			if (path_exist(graph, edge->vertex->data, data2)) return true;
		}
		edge = edge->next;
	}

	return false;
}


void quit() {
	printf("Quit\n");
	exit(0);
}

void insert_vertex(GRAPH* graph, char newdata) {
	if (search(graph, newdata) != NULL) {
		printf("Same vertex\n");
		return;
	}

	VERTEX* new = malloc(sizeof(VERTEX));

	new->data = newdata;
	new->visited = 0;
	new->cnt_edge = 0;
	new->next = NULL;
	new->connect = NULL;

	if (graph->head == NULL) {
		graph->head = new;
	}
	else {
		VERTEX* cur = graph->head;
		while (cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = new;
	}

	graph->cnt_vertex++;

	print(graph);
}

void add_edge(GRAPH* graph, char data1, char data2) {
	VERTEX* start = search(graph, data1);
	VERTEX* end = search(graph, data2);

	if (start == NULL || end == NULL) {
		printf("Error: add edge\n");
		return;
	}

	// 시작 정점의 인접 리스트에 도착 정점 추가
	EDGE* new1 = malloc(sizeof(EDGE));

	new1->vertex = end;
	new1->next = start->connect;
	start->connect = new1;
	start->cnt_edge++;

	// 도착 정점의 인접 리스트에 시작 정접 추가
	EDGE* new2 = malloc(sizeof(EDGE));

	new2->vertex = start;
	new2->next = end->connect;
	end->connect = new2;
	end->cnt_edge++;

	print(graph);
}

void print(GRAPH* graph) {
	VERTEX* cur1 = graph->head;

	printf("(");
	while (cur1 != NULL) {
		printf("%c", cur1->data);

		EDGE* cur2 = cur1->connect;
		if (cur2 != NULL) {
			printf("(");
			while (cur2 != NULL) {
				printf("%c", cur2->vertex->data);
				if (cur2->next != NULL) {
					printf(",");
				}
				cur2 = cur2->next;
			}
			printf(")");
		}

		if (cur1->next != NULL) {
			printf(",");
		}

		cur1 = cur1->next;
	}
	printf(")\n");
}


int degree_of_vertex(GRAPH* graph, char isdata) {
	VERTEX* cur = search(graph, isdata);

	if (cur == NULL) {
		printf("No such data\n");
		return -1;
	}

	return cur->cnt_edge;
}

int num_of_vertex(GRAPH* graph) {
	return graph->cnt_vertex;
}

int num_of_edge(GRAPH* graph) {
	int num = 0;

	VERTEX* cur = graph->head;

	while (cur != NULL) {
		num += cur->cnt_edge;
		cur = cur->next;
	}

	// 중복 제거
	return num / 2;
}

void adjacent(GRAPH* graph, char isdata) {
	VERTEX* cur = search(graph, isdata);

	if (cur == NULL) {
		printf("No such data\n");
		return;
	}

	EDGE* edge = cur->connect;
	bool first = true;

	while (edge != NULL) {
		if (!first) printf(",");

		printf("%c", edge->vertex->data);
		first = false;
		edge = edge->next;
	}
}

void rename_vertex(GRAPH* graph, char data1, char data2) {
	VERTEX* old = search(graph, data1);
	VERTEX* new = search(graph, data2);

	if (old == NULL) {
		printf("No such data\n");
		return;
	}

	if (new != NULL) {
		printf("Already exist\n");
		return;
	}

	old->data = data2;

	print(graph);
}

void delete_vertex(GRAPH* graph, char isdata) {
	VERTEX* del = graph->head;
	VERTEX* pre = NULL;

	while (del != NULL && del->data != isdata) {
		pre = del;
		del = del->next;
	}

	if (del == NULL) return;

	EDGE* edge = del->connect;

	while (edge != NULL) {
		// 해당 정점 삭제 + 해당 정점이 다른 정점의 인접 리스트에 나와있는 것도 삭제
		// 즉, (A(B),B(A),C)에서 -A를 한 경우 (B,C)만 남도록
		VERTEX* connected_vertex = edge->vertex;
		EDGE* cur = connected_vertex->connect;
		EDGE* pre = NULL;
		while (cur != NULL) {
			if (cur->vertex->data == isdata) {
				if (pre == NULL) {
					connected_vertex->connect = cur->next;
				}

				else {
					pre->next = cur->next;
				}

				free(cur);
				connected_vertex->cnt_edge--;
				break;
			}

			pre = cur;
			cur = cur->next;
		}

		EDGE* temp = edge;
		edge = edge->next;
		free(temp);
	}

	if (pre == NULL) {
		graph->head = del->next;
	}

	else {
		pre->next = del->next;
	}

	free(del);
	graph->cnt_vertex--;
}

void delete_edge(GRAPH* graph, char data1, char data2) {
	VERTEX* start = search(graph, data1);
	VERTEX* end = search(graph, data2);

	if (start == NULL || end == NULL) {
		printf("Error\n");
		return;
	}

	/*
	if (path_exist(graph, data1, data2) == false) {
		printf("Error\n");
		return;
	}
	*/

	// 첫번째(start) 정점의 인접 리스트의 첫번째 간선을 가리킴
	EDGE** cur = &(start->connect);

	// 두번째(end) 정점과의 간선을 찾음
	while (*cur != NULL && (*cur)->vertex != end) {
		cur = &((*cur)->next);
	}

	if (*cur != NULL) {
		EDGE* del = *cur;
		*cur = (*cur)->next;

		free(del);
		start->cnt_edge--;
	}

	// 두번째 정점의 인접 리스트의 첫번째 간선을 가리짐
	cur = &(end->connect);

	// 첫번째 정점과의 간선을 찾음
	while (*cur != NULL && (*cur)->vertex != start) {
		cur = &((*cur)->next);
	}

	if (*cur != NULL) {
		EDGE* del = *cur;
		*cur = (*cur)->next;

		free(del);
		end->cnt_edge--;
	}

	print(graph);
}

void clear(GRAPH* graph) {
	while (graph->head != NULL) {
		delete_vertex(graph, graph->head->data);
	}

	free(graph);
}

int main() {
	GRAPH* mygraph = malloc(sizeof(GRAPH));
	char input[100];

	while (1) {
		scanf("%s", input);

		switch (input[0]) {
		case 'G':
			printf("(");
			create(mygraph);
			printf(")\n");
			break;

		case '+':
			insert_vertex(mygraph, input[1]);
			break;

		case 'E':
			add_edge(mygraph, input[2], input[4]);
			break;

		case 'L':
			print(mygraph);
			break;

		case 'V':
			printf("%d\n", degree_of_vertex(mygraph, input[2]));
			break;

		case 'C':
			printf("%s\n", is_connected(mygraph) ? "True" : "False");
			break;

		case 'N':
			printf("%s\n", is_empty(mygraph) ? "True" : "False");
			break;

		case 'A':
			printf("(");
			adjacent(mygraph, input[2]);
			printf(")\n");
			break;

		case '-':
			delete_vertex(mygraph, input[1]);
			print(mygraph);
			break;

		case 'P':
			printf("%s\n", path_exist(mygraph, input[2], input[4]) ? "True" : "False");
			break;

		case 'X':
			printf("%d\n", num_of_vertex(mygraph));
			break;

		case 'H':
			printf("%d\n", num_of_edge(mygraph));
			break;

		case 'D':
			delete_edge(mygraph, input[2], input[4]);
			break;

		case 'R':
			rename_vertex(mygraph, input[2], input[4]);
			break;

		case 'K':
			printf("Graph cleared\n");
			clear(mygraph);
			break;

			// 자신만의 기능 3가지
		case '?':
			printf("%s\n", is_full(mygraph) ? "True" : "False");
			break;

		case 'Q':
			quit();
			break;

		case '!':
			intro();
			break;
		}
	}
}