#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// Compare Function

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}
// Merge ttwo sorted Subarrays
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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_workers> <array_size>\n", argv[0]);
        return 1;
    }

    int num_workers = atoi(argv[1]);
    int array_size = atoi(argv[2]);
    int chunk_size = array_size / num_workers;

    int *array = malloc(array_size * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < array_size; i++) array[i] = rand() % 100000;

    int pipes[num_workers][2];
    clock_t start = clock();
// Create Child process for chunk sorting
    for (int i = 0; i < num_workers; i++) {
        pipe(pipes[i]);
        pid_t pid = fork();

        if (pid == 0) {
            close(pipes[i][0]);
            int start_idx = i * chunk_size;
            int end_idx = (i == num_workers - 1) ? array_size : start_idx + chunk_size;

            qsort(array + start_idx, end_idx - start_idx, sizeof(int), compare);
            write(pipes[i][1], array + start_idx, (end_idx - start_idx) * sizeof(int));
            close(pipes[i][1]);
            free(array);
            exit(0);
        }
    }

    for (int i = 0; i < num_workers; i++) {
        close(pipes[i][1]);
        int start_idx = i * chunk_size;
        int end_idx = (i == num_workers - 1) ? array_size : start_idx + chunk_size;
        read(pipes[i][0], array + start_idx, (end_idx - start_idx) * sizeof(int));
        close(pipes[i][0]);
        
    }
    for (int i = 0; i < num_workers; i++) wait(NULL);

    // Merge chunks 
    int merged_size = chunk_size;
    for (int i = 1; i < num_workers; i++) {
        int right_end = (i == num_workers - 1) ? array_size - 1 : (i + 1) * chunk_size - 1;
        merge(array, 0, merged_size - 1, right_end);
        merged_size = right_end + 1;
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Multiprocessing Sort Completed\n");
    printf("Workers: %d | Array Size: %d | Time: %.4f sec\n", num_workers, array_size, time_spent);

    if (array_size <= 32) {
        for (int i = 0; i < array_size; i++) printf("%d ", array[i]);
        printf("\n");
    }

    free(array);
    return 0;
}


