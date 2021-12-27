//
// 2021.11.16
// Problem_2 : 섞인 카드 묶음
// 20192138 조명재
//

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* command_fp, * output_fp, * fp;
int left_n, right_n, card_size;
int* left_card, * right_card, * card;
bool ***check_card, global_result;

bool solution(int left, int right, int index)
{
	bool result;

	// 1. Base case 처리
	// 어디선가 정답 처리된 경우 => true
	if (global_result == true)
		return true;
	// 카드의 총 갯수 내에서 카드를 적절히 뽑아낸 경우 => true
	if (left == left_n && right == right_n && index <= card_size) {
		global_result = true;
		return true;
	}
	// 선택해야 할 총 카드의 갯수 > 남아있는 카드의 갯수 => false
	if ((left_n - left) + (right_n - right) > (card_size - index))
		return false;
	// 카드를 더 뽑은 경우 => false
	if (left > left_n || right > right_n)
		return false;
	// 이미 해당 경우를 거친 경우 => false
	if (check_card[left][right][index])
		return global_result;

	check_card[left][right][index] = true; // 방문 처리

	// 2. Dynamic Programming
	result = false;
	if (card[index] == left_card[left] && card[index] == right_card[right]) {
		result = solution(left, right, index + 1) || solution(left + 1, right, index + 1)
			|| solution(left, right + 1, index + 1);
	}
	else if (card[index] == left_card[left]) {
		result = solution(left, right, index + 1) || solution(left + 1, right, index + 1);
	}
	else if (card[index] == right_card[right]) {
		result = solution(left, right, index + 1) || solution(left, right + 1, index + 1);
	}
	
	return result;
}

void set_card() 
{
	int i;

	fscanf(fp, "%d ", &left_n);
	left_card = (int*)malloc(sizeof(int) * left_n);
	for (i = 0; i < left_n; i++) {
		if (i == left_n - 1) fscanf(fp, "%d\n", &left_card[i]);
		else fscanf(fp, "%d ", &left_card[i]);
	}

	fscanf(fp, "%d ", &right_n);
	right_card = (int*)malloc(sizeof(int) * right_n);
	for (i = 0; i < right_n; i++) {
		if (i == right_n - 1) fscanf(fp, "%d\n", &right_card[i]);
		else fscanf(fp, "%d ", &right_card[i]);
	}
}

void get_card()
{
	int i;
	
	fscanf(fp, "%d ", &card_size);
	card = (int*)malloc(sizeof(int) * card_size);
	for (i = 0; i < card_size; i++) {
		if (i == card_size - 1) fscanf(fp, "%d\n", &card[i]);
		else fscanf(fp, "%d ", &card[i]);
	}
}

void init_card()
{
 	register int i, j, k;

	global_result = false;
	check_card = (bool***)malloc((left_n + 1) * (right_n + 1) * (card_size + 1));
	for (i = 0; i <= left_n; i++) {
		check_card[i] = (bool**)malloc((right_n + 1) * (card_size + 1));
		for (j = 0; j <= right_n; j++) {
			check_card[i][j] = (bool*)malloc(card_size + 1);
			for (k = 0; k <= card_size; k++) {
				check_card[i][j][k] = false;
			}
		}
	}
}

void free_card(int free_op)
{
	register int i, j;

	free(card);

	if (free_op) {
		for (i = 0; i < left_n; i++)
			for (j = 0; j < right_n; j++)
				free(check_card[i][j]);
		for (i = 0; i < left_n; i++)
			free(check_card[i]);
		free(check_card);
	}
}

void start_card()
{
	get_card();
	if (left_n + right_n <= card_size) {
		init_card();
		fprintf(output_fp, "%d", solution(0,0,0));
		free_card(1);
	}
	else {
		fprintf(output_fp, "0");
		free_card(0);
	}

	get_card();
	if (left_n + right_n <= card_size) {
		init_card();
		fprintf(output_fp, "%d\n", solution(0,0,0));
		free_card(1);
	}
	else {
		fprintf(output_fp, "0\n");
		free_card(0);
	}
}

int main(void)
{
	int i;
	char fileName[30];

	command_fp = fopen("commands_3_2.txt", "r");
	output_fp = fopen("outputs_3_2.txt", "w");
	
	while (!feof(command_fp)) {
		fscanf(command_fp, "%s", fileName);
		fp = fopen(fileName, "r");
		set_card();
		start_card();
		fclose(fp);
	}

	free(left_card), free(right_card);

	fclose(output_fp);
	fclose(command_fp);

	return 0;
}