typedef int _Cmpfun(const void*, const void*);
void qsort_orig(void* base, size_t left, size_t right, _Cmpfun* cmp);
void qsort_median_insert(void* base, size_t left, size_t right, _Cmpfun* cmp);
void qsort_median_insert_iter(void* base, size_t left, size_t right, _Cmpfun* cmp);