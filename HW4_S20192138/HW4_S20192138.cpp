#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

__int64 start, freq, end;

#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))

float compute_time;

struct Result {
	int connected_cnt;
	long long total_weight;
};

struct Graph {
	int from;
	int to;
	long long weight;
};

struct Edge {
	int next_node;
	long long weight;
	struct Edge* next;
};

struct Node {
	struct Edge* head;
};

Result* result;
Graph* graph;
Node* node;
int* parent;
bool *visited;
int graph_size, result_size;

Edge* dummy_edge() {
	Edge* e;
	e = (Edge*)malloc(sizeof(Edge));
	e->next = NULL;
	return e;
}

Edge* make_edge(int next_node, long long weight) {
	Edge* e;
	e = (Edge*)malloc(sizeof(Edge));
	e->next_node = next_node;
	e->weight = weight;
	e->next = NULL;
	return e;
}

void connect_edge(Edge* head, Edge* edge) {
	if (head->next == NULL) {
		head->next = edge;
	}
	else {
		edge->next = head->next;
		head->next = edge;
	}
}

void initialize(int n_vertices, int n_edges) {
	int i;

	result = (Result*)malloc(sizeof(Result) * (n_vertices + 1));
	graph = (Graph*)malloc(sizeof(Graph) * (2 * n_edges + 1));
	node = (Node*)malloc(sizeof(Node) * (n_vertices + 1));
	parent = (int*)malloc(sizeof(int) * (n_vertices + 1));
	visited = (bool*)malloc(n_vertices + 1);

	for (i = 0; i < n_vertices; i++) {
		node[i].head = dummy_edge();
		parent[i] = i;
		visited[i] = false;
	}
}

void all_free() {
	free(result);
	free(node);
	free(parent);
	free(visited);
	free(graph);
}

void PQ_insert(int from, int to, long long weight) {
	int x;
	x = ++graph_size;
	while ((x != 1) && weight < graph[x / 2].weight) {
		graph[x] = graph[x / 2];
		x /= 2;
	}
	graph[x].from = from, graph[x].to = to;
	graph[x].weight = weight;
}

struct Graph PQ_delete() {
	int parent, child;
	Graph temp, ret;

	parent = 1, child = 2;
	ret = graph[1], temp = graph[graph_size--];
	while (child <= graph_size) {
		if ((child < graph_size) && graph[child].weight > graph[child + 1].weight) 
			child++;
		if (temp.weight <= graph[child].weight) break;
		graph[parent] = graph[child];
		parent = child;
		child *= 2;
	}

	graph[parent] = temp;
	return ret;
}

int Find(int* parent, int x) {
	if (parent[x] == x)
		return x;
	return (parent[x] = Find(parent, parent[x]));
}

void Union(int* parent, int x, int y) {
	x = Find(parent, x);
	y = Find(parent, y);
	if (x < y) parent[y] = x;
	else parent[x] = y;
}

void dfs(int before_node, int cur_node, int* connected_cnt) {
	if (visited[cur_node]) 
		return;

	Edge* cur, *before;
	visited[cur_node] = true;
	(*connected_cnt)++;
	cur = node[cur_node].head;

	while (cur->next) {
		before = cur;
		cur = cur->next;
		free(before);
		if (cur->next_node != before_node) {
			PQ_insert(cur_node, cur->next_node, cur->weight);
			dfs(cur_node, cur->next_node, connected_cnt);
		}
	}

	free(cur);
}

long long Kruskal_MST(int connected_cnt, int edge_cnt) {
	int i, from, to, cnt, k_scanned;
	long long weight, total_weight;
	struct Graph edge;

	cnt = k_scanned = total_weight = 0;
	for (i = 0; i < edge_cnt; i++) {
		edge = PQ_delete();
		from = edge.from, to = edge.to;
		weight = edge.weight;
		if (Find(parent, from) != Find(parent, to)) {
			Union(parent, from, to);
			total_weight += weight;
			cnt++;
		}
		if (cnt == connected_cnt - 1) {
			k_scanned = i;
			break;
		}
	}

	//printf("kscanned : %d\n", k_scanned);
	return total_weight;
}

int main() {
	char input_file_name[128];
	char output_file_name[128];

	FILE* fp_in, * fp_out, *fp_command, *fp;
	int n_vertices, n_edges, v_from, v_to, i, connected_cnt;
	long long weight, max_weight, total_weight;

	// ===== commands.txt 처리 작업 시작 ===========
	fp_command = fopen("commands.txt", "r");
	
	if (!fp_command) { 
		fprintf(stderr, "^^... Error: cannot open the file %s.\n", "commands.txt");
		exit(-1);
	}

	fscanf(fp_command, "%s", input_file_name);
	fscanf(fp_command, "%s", output_file_name);

	fclose(fp_command);
	// ===== commands.txt 처리 작업 종료 ===========

	// ===== MST 를 구하기 위한 세팅 과정 시작 =====
	fp = fopen(input_file_name, "r");
	
	CHECK_TIME_START;
	fscanf(fp, "%d %d %lld", &n_vertices, &n_edges, &max_weight);

	initialize(n_vertices, n_edges);
	for (i = 0; i < n_edges; i++) {
		fscanf(fp, "%d %d %lld", &v_from, &v_to, &weight);
		connect_edge(node[v_from].head, make_edge(v_to, weight));
		connect_edge(node[v_to].head, make_edge(v_from, weight));
	}

	fclose(fp);
	// ===== MST 를 구하기 위한 세팅 과정 종료 =====

	// ===== MST 구하는 과정 시작 ==================
	for (i = 0; i < n_vertices; i++) {
		if (!visited[i]) {
			connected_cnt = graph_size = 0;
			dfs(-1, i, &connected_cnt);
			if (connected_cnt == 1) {
				result[result_size].connected_cnt = 1;
				result[result_size].total_weight = 0;
			}
			else {
				result[result_size].connected_cnt = connected_cnt;
				result[result_size].total_weight = Kruskal_MST(connected_cnt, graph_size);
			}
			result_size++;
		}
	}
	CHECK_TIME_END(compute_time);
	//printf("%s's Time = %.3fms\n", input_file_name, compute_time);
	// ===== MST 구하는 과정 종료 ==================

	// ===== 결과를 출력하는 과정 시작 =============
	fp = fopen(output_file_name, "w");
	fprintf(fp, "%d\n", result_size);
	for (i = 0; i < result_size; i++) {
		fprintf(fp, "%d %lld\n", result[i].connected_cnt, result[i].total_weight);
	}
	fclose(fp);
	// ===== 결과를 출력하는 과정 종료 =============

	all_free();

	return 0;
}