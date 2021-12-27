#include "my_quick_sorts.h"

typedef struct {
	unsigned int score;
	float data[3];
	char comments[16];
} ELEMENT;

void qsort_orig(void* base, size_t left, size_t right, _Cmpfun* cmp) {
	int pivot, i, j, int_left, int_right;
	int_left = (int)left, int_right = (int)right;
	ELEMENT tmp;

	if (int_left < int_right) {
		i = int_left, j = int_right + 1;
		pivot = ((ELEMENT*)base + int_left)->score;
		do {
			do {
				i++;
			} while (((ELEMENT*)base + i)->score < pivot);
			do {
				j--;
			} while (((ELEMENT*)base + j)->score > pivot);
			if (i < j) {
				tmp = ((ELEMENT*)base)[i];
				((ELEMENT*)base)[i] = ((ELEMENT*)base)[j];
				((ELEMENT*)base)[j] = tmp;
			}
		} while (i < j);

		tmp = ((ELEMENT*)base)[int_left];
		((ELEMENT*)base)[int_left] = ((ELEMENT*)base)[j];
		((ELEMENT*)base)[j] = tmp;
		qsort_orig(base, int_left, j - 1, cmp);
		qsort_orig(base, j + 1, int_right, cmp);
	}
}

void qsort_median_insert(void* base, size_t left, size_t right, _Cmpfun* cmp)
{
	if ((int)right - (int)left + 1 <= 20) return;
	size_t i, j, pivot;
	ELEMENT temp;

	temp = ((ELEMENT*)base)[(left + right) / 2];
	((ELEMENT*)base)[(left + right) / 2] = ((ELEMENT*)base)[right - 1];
	((ELEMENT*)base)[right - 1] = temp;

	if (((ELEMENT*)base)[left].score > ((ELEMENT*)base)[right - 1].score) {
		temp = ((ELEMENT*)base)[left];
		((ELEMENT*)base)[left] = ((ELEMENT*)base)[right - 1];
		((ELEMENT*)base)[right - 1] = temp;
	}
	if (((ELEMENT*)base)[left].score > ((ELEMENT*)base)[right].score) {
		temp = ((ELEMENT*)base)[left];
		((ELEMENT*)base)[left] = ((ELEMENT*)base)[right];
		((ELEMENT*)base)[right] = temp;
	}
	if (((ELEMENT*)base)[right - 1].score > ((ELEMENT*)base)[right].score) {
		temp = ((ELEMENT*)base)[right - 1];
		((ELEMENT*)base)[right - 1] = ((ELEMENT*)base)[right];
		((ELEMENT*)base)[right] = temp;
	}

	pivot = left;
	for (i = left; i < right; i++) {
		if (((ELEMENT*)base)[i].score < ((ELEMENT*)base)[right].score) {
			temp = ((ELEMENT*)base)[i];
			((ELEMENT*)base)[i] = ((ELEMENT*)base)[pivot];
			((ELEMENT*)base)[pivot] = temp;
			pivot++;
		}
	}
	temp = ((ELEMENT*)base)[pivot];
	((ELEMENT*)base)[pivot] = ((ELEMENT*)base)[right];
	((ELEMENT*)base)[right] = temp;

	qsort_median_insert(base, left + 1, pivot - 1, cmp);
	qsort_median_insert(base, pivot + 1, right - 1, cmp);

	if (left == 0) {
		for (i = left + 1; i <= right; i++) {
			temp = ((ELEMENT*)base)[i];
			j = i;
			while ((j > left) && (temp.score < ((ELEMENT*)base)[j - 1].score)) {
				((ELEMENT*)base)[j] = ((ELEMENT*)base)[j - 1];
				j--;
			}
			((ELEMENT*)base)[j] = temp;
		}
	}
}

void qsort_median_insert_iter(void* base, size_t left, size_t right, _Cmpfun* cmp)
{
	ELEMENT temp;
	size_t i, pivot;

	if ((int)right - (int)left == 1) {
		if (((ELEMENT*)base)[left].score > ((ELEMENT*)base)[right].score) {
			temp = ((ELEMENT*)base)[left];
			((ELEMENT*)base)[left] = ((ELEMENT*)base)[right];
			((ELEMENT*)base)[right] = temp;
		}
		return;
	}

	while ((int)right - (int)left > 1) {
		pivot = left;
		for (i = left; i < right; i++) {
			if (((ELEMENT*)base)[i].score < ((ELEMENT*)base)[right].score) {
				temp = ((ELEMENT*)base)[i];
				((ELEMENT*)base)[i] = ((ELEMENT*)base)[pivot];
				((ELEMENT*)base)[pivot] = temp;
				pivot++;
			}
		}
		temp = ((ELEMENT*)base)[pivot];
		((ELEMENT*)base)[pivot] = ((ELEMENT*)base)[right];
		((ELEMENT*)base)[right] = temp;

		if (pivot < (left + right) / 2) {
			qsort_median_insert_iter(base, left, pivot - 1, cmp);
			left = pivot + 1;
		}
		else {
			qsort_median_insert_iter(base, pivot + 1, right, cmp);
			right = pivot - 1;
		}
	}

	if ((int)right - (int)left == 1) {
		if (((ELEMENT*)base)[left].score > ((ELEMENT*)base)[right].score) {
			temp = ((ELEMENT*)base)[left];
			((ELEMENT*)base)[left] = ((ELEMENT*)base)[right];
			((ELEMENT*)base)[right] = temp;
		}
	}
}