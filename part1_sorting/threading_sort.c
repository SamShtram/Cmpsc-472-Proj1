#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int *array;
int num_workers;
int array_size;
int chunk_size;

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Worker thread: sort assigned chunk
void *worker_sort(void *arg) {
    int id = *(int *)arg;
    int start = id * chunk_size;
    int end = (id == num_workers - 1) ? array_size : start + chunk_size;
    qsort(array + start, end - start, sizeof(int), compare);
    pthread_exit(NULL);
}

// Merge two sorted subarrays
void merge(int *arr, int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

// Merge chunks sequentially (Reducer)
void merge_chunks() {
    int merged_size = chunk_size;
    for (int i = 1; i < num_workers; i++) {
        int right_end = (i == num_workers - 1) ? array_size - 1 : (i + 1) * chunk_size - 1;
        merge(array, 0, merged_size - 1, right_end);
        merged_size = right_end + 1;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_workers> <array_size>\n", argv[0]);
        return 1;
    }

    num_workers = atoi(argv[1]);
    array_size = atoi(argv[2]);
    chunk_size = array_size / num_workers;

    array = malloc(array_size * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < array_size; i++) array[i] = rand() % 100000;

    pthread_t threads[num_workers];
    int ids[num_workers];

    clock_t start = clock();

    for (int i = 0; i < num_workers; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker_sort, &ids[i]);
    }

    for (int i = 0; i < num_workers; i++) pthread_join(threads[i], NULL);

    merge_chunks();

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Threading Sort Completed\n");
    printf("Workers: %d | Array Size: %d | Time: %.4f sec\n", num_workers, array_size, time_spent);

    if (array_size <= 32) {
        for (int i = 0; i < array_size; i++) printf("%d ", array[i]);
        printf("\n");
    }

    free(array);
    return 0;
}
`
