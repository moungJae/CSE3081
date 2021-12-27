//
// Example codes for Sogang CSE0381(2)
// 2021.10.11
// 20192138 Α¶Ένΐη
//

#define _CRT_SECURE_NO_WARNINGS 
#define _CRT_NONSTDC_NO_DEPRECATE
#include <Windows.h>
__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <stdlib.h>
#include <string.h>
#include "my_quick_sorts.h"

#define MAX_BUF	256

typedef struct {
	unsigned int score;
	float data[3];
	char comments[16];
} ELEMENT;

void qsort(void* base, size_t n, size_t size, _Cmpfun* cmp)
{
	while (1 < n)
	{
		size_t	i = 0;
		size_t	j = n - 1;
		char* qi = (char*)base;
		char* qj = qi + size * j;
		char* qp = qj;

		while (i < j)
		{
			while (i < j && (*cmp)(qi, qp) <= 0)
				++i, qi += size;
			while (i < j && (*cmp)(qp, qj) <= 0)
				--j, qj -= size;

			if (i < j) {
				char	buf[MAX_BUF];
				char* q1 = qi;
				char* q2 = qj;
				size_t	m, ms;

				for (ms = size; 0 < ms; ms -= m, q1 += m, q2 += m) {
					m = (ms < sizeof(buf)) ? ms : sizeof(buf);
					memcpy(buf, q1, m);
					memcpy(q1, q2, m);
					memcpy(q2, buf, m);
				}
				++i, qi += size;
			}
		}
		if (qi != qp) {
			char	buf[MAX_BUF];
			char* q1 = qi;
			char* q2 = qp;
			size_t	m, ms;

			for (ms = size; 0 < ms; ms -= m, q1 += m, q2 += m) {
				m = (ms < sizeof(buf)) ? ms : sizeof(buf);
				memcpy(buf, q1, m);
				memcpy(q1, q2, m);
				memcpy(q2, buf, m);
			}
		}
		j = n - i - 1, qi += size;
		if (j < i) {
			if (1 < j)
				qsort(qi, j, size, cmp);
			n = i;
		}
		else {
			if (1 < i)
				qsort(base, i, size, cmp);
			base = qi;
			n = j;
		}
	}
}

int my_element_keys_compare(const void* a, const void* b) {
	ELEMENT* rec_a, * rec_b;

	rec_a = (ELEMENT*)a;
	rec_b = (ELEMENT*)b;

	if (rec_a->score == rec_b->score)
		return 0;
	else if (rec_a->score < rec_b->score)
		return -1;
	else return 1;

	// could be "return rec_a->score - rec_b->score;"
}

int my_unsigned_int_keys_compare(const void* a, const void* b) {
	unsigned int* ui_a, * ui_b;;

	ui_a = (unsigned*)a;
	ui_b = (unsigned*)b;

	if (*ui_a == *ui_b)
		return 0;
	else if (*ui_a < *ui_b)
		return -1;
	else return 1;
}

void init_ELEMENT_array(ELEMENT* data, int n) {
	// Generate an array with random key sequences.
	int i, j;

	for (i = 0; i < n; i++) {
		data[i].score = i;
		data[i].data[0] = data[i].data[1] = data[i].data[2] = i;
		itoa(i, data[i].comments, 16);
	}

	// Shuffle the key values using the Fisher-Yates shuffle algorithm.
	srand((unsigned int)time(NULL));

	for (i = n - 1; i >= 1; i--) {
		j = rand() % (i + 1);

		{
			// codes from qsort.c
			char buf[256];
			char* r_i = (char*)(data + i);
			char* r_j = (char*)(data + j);
			size_t m, ms;

			for (ms = sizeof(ELEMENT); ms > 0; ms -= m, r_i += m, r_j += m) {
				m = ms < sizeof(buf) ? ms : sizeof(buf);

				memcpy(buf, r_i, m);
				memcpy(r_i, r_j, m);
				memcpy(r_j, buf, m);
			}
		}
	}
}

int main(void) 
{
	/*
	int x;
	FILE* f;
	f = fopen("unsorted_array.bin", "wb");

	// ================== [ Determine the size of array ] ====================== 
	int _n[11] = { 1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14,
				1 << 15, 1 << 16, 1 << 17, 1 << 18, 1 << 19, 1 << 20 };
	const int N = _n[0]; // n[x] <= If you want to change the size, change the 'x'.
	ELEMENT* E = (ELEMENT*)malloc(sizeof(ELEMENT) * N);
	// =========================================================================

	init_ELEMENT_array(E, N);

	for (x = 0; x < N; x++) {
		fwrite(&E[x], sizeof(ELEMENT), 1, f);
	}

	fclose(f);
	*/

	int n, qsort_method, sort_check;
	char rFile[20], wFile[20];
	unsigned int i;
	ELEMENT* Array;
	FILE* fp, * rfp, * wfp;

	// Read HW2_commands.txt 
	fp = fopen("HW2_commands.txt", "r");
	fscanf(fp, "%d\n%d\n%s\n%s", &qsort_method, &n, &rFile, &wFile);
	fclose(fp);

	printf("*** Array size: %d\n", n);

	// Read a unsorted_array.bin and set an array.
	Array = (ELEMENT*)malloc(sizeof(ELEMENT) * n);
	rfp = fopen(rFile, "rb");
	for (i = 0; i < n; i++) {
		fread(&Array[i], sizeof(ELEMENT), 1, rfp);
	}
	fclose(rfp);

	// Check Time
	CHECK_TIME_START;
	switch (qsort_method) {
		case 1:
			qsort(Array, n, sizeof(ELEMENT), my_element_keys_compare);
			break;
		case 21:
			qsort_orig(Array, 0, n - 1, my_element_keys_compare);
			break;
		case 22:
			qsort_median_insert(Array, 0, n - 1, my_element_keys_compare);
			break;
		case 23:
			qsort_median_insert_iter(Array, 0, n - 1, my_element_keys_compare);
			break;
	}
	CHECK_TIME_END(compute_time);
	
	sort_check = 0;
	for (i = 0; i < n; i++) {
		if (Array[i].score == i)
			sort_check++;
	}
	// If Array is nondecreasing order, print the compute_time.
	if(sort_check == n)
		printf("*** RECORD type of size %d: Time taken by qsort() = %.3fms\n", sizeof(ELEMENT), compute_time);

	// Write an array into sorted_array.bin
	wfp = fopen(wFile, "wb");
	for (i = 0; i < n; i++) {
		fwrite(&Array[i], sizeof(ELEMENT), 1, wfp);
	}
	fclose(wfp);
	free(Array);

	return 0;
}