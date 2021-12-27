//
// 2021.11.16
// Problem_1 : Longest Palindromic Subsequence 
// 20192138 조명재
//

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int SELF = 0, LEFT = 100, LEFT_AND_RIGHT = 200, RIGHT = 300;

void LPS(const int m, const char* X, int* LPS_length, char** LPS_string)
{
	int** Len_Table;
	int **Pal_Table;
	int i, j, k, gap, left,right, length;
	char* left_string, *right_string, *result_string;

	Len_Table = (int **)malloc(sizeof(int) * m);
	Pal_Table = (int **)malloc(sizeof(int) * m);
	left_string = (char*)malloc(m);
	right_string = (char*)malloc(m);
	for (i = 0; i < m; i++) {
		Len_Table[i] = (int *)malloc(sizeof(int) * m);
		Pal_Table[i] = (int*)malloc(sizeof(int) * m);
	}

	// 1. Set the Table
	for (i = 0; i < m; i++)
		Len_Table[i][i] = 1, Pal_Table[i][i] = SELF;
	for (i = 0; i < m - 1; i++) {
		if (X[i] == X[i + 1])
			Len_Table[i][i + 1] = 2, Pal_Table[i][i + 1] = LEFT_AND_RIGHT;
		else
			Len_Table[i][i + 1] = 1, Pal_Table[i][i + 1] = LEFT;
	}

	// 2. Dynamic Programming
	for (gap = 2; gap < m; gap++) {
		for (i = 0; i < m - gap; i++) {
			j = i + gap;
			if (X[i] == X[j])
				Len_Table[i][j] = Len_Table[i + 1][j - 1] + 2, Pal_Table[i][j] = LEFT_AND_RIGHT;
			else {
				if (Len_Table[i][j] < Len_Table[i + 1][j])
					Len_Table[i][j] = Len_Table[i + 1][j], Pal_Table[i][j] = LEFT;
				if (Len_Table[i][j] < Len_Table[i][j - 1])
					Len_Table[i][j] = Len_Table[i][j - 1], Pal_Table[i][j] = RIGHT;
				if (Len_Table[i][j] < Len_Table[i + 1][j - 1])
					Len_Table[i][j] = Len_Table[i + 1][j - 1], Pal_Table[i][j] = LEFT_AND_RIGHT;
			}
		}
	}

	// 3. Find the Longest Panlidrome Subsequence
	left = 0, right = m - 1;
	i = j = length = 0;
	while (left <= right) {
		switch (Pal_Table[left][right]) {
			case SELF:
				left_string[i++] = X[left];
				left += 1, length += 1;
				break;
			case LEFT:
				left += 1;
				break;
			case RIGHT:
				right -= 1;
				break;
			case LEFT_AND_RIGHT:
				left_string[i++] = X[left];
				right_string[j++] = X[right];
				left += 1, right -= 1, length += 2;
		}
	}

	while (j) {
		left_string[i++] = right_string[--j];
	}

	result_string = (char*)malloc(length + 1);
	for (i = 0; i < length; i++) result_string[i] = left_string[i];
	result_string[length] = '\0';

	*LPS_length = length, * LPS_string = result_string;
	return;
}

void check(char *rFile)  // 결과 확인 함수
{
	int m;
	char in_X[1001];
	FILE* fp;
	fp = fopen(rFile, "rb");

	fread(&m, sizeof(int), 1, fp);
	fread(&in_X, sizeof(char), m, fp);
	in_X[m] = '\0';
	printf("Result : %d %s \n", m, in_X);
	
	fclose(fp);
}

int main(void)
{
	int test_case, m, LPS_length;
	char in_X[1001], out_Y[1001]; // 최대 문자열 길이 1000
	char rFile[30], wFile[30];
	char* LPS_string;
	FILE* fp, * rfp, * wfp;

	fp = fopen("config_LPS.txt", "r");
	fscanf(fp, "%d\n", &test_case);

	LPS_length = -1;
	while (test_case--) {
		fscanf(fp, "%s\n", rFile);
		rfp = fopen(rFile, "rb");
		fread(&m, sizeof(int), 1, rfp);
		fread(&in_X, sizeof(char), m, rfp);
		in_X[m] = '\0';
		fclose(rfp);
		
		LPS_string = (char*)malloc(m + 1);
		LPS(m, in_X, &LPS_length, &LPS_string);
		strcpy(out_Y, LPS_string);

		fscanf(fp, "%s\n", wFile);
		wfp = fopen(wFile, "wb");
		fwrite(&LPS_length, sizeof(int), 1, wfp);
		fwrite(&out_Y, sizeof(char), LPS_length, wfp);
		fclose(wfp);
		//check(wFile);     <=  결과 확인하기 위한 용도로 만들었습니다.

		free(LPS_string);
	}

	fclose(fp);
	
	return 0;
}