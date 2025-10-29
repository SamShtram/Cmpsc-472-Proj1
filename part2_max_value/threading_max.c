#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int *array;
int num_workers;
int array_size;
int chunk_size;
int global_max;
pthread_mutex_t lock;
// Each thread computes its local maximum and updates global max
void *find_local_max(void *arg) {
    int id = *(int *)arg;
    int start = id * chunk_size;
    int end = (id == num_workers - 1) ? array_size : start + chunk_size;
    int local_max = array[start];
    for (int i = start + 1; i < end; i++)
        if (array[i] > local_max)
            local_max = array[i];

    pthread_mutex_lock(&lock);
    if (local_max > global_max)
        global_max = local_max;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
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

    global_max = array[0];
    pthread_mutex_init(&lock, NULL);
    pthread_t threads[num_workers];
    int ids[num_workers];

    clock_t start = clock();
// Create Threads
    for (int i = 0; i < num_workers; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, find_local_max, &ids[i]);
    }
// Wait for all threads
    for (int i = 0; i < num_workers; i++)
        pthread_join(threads[i], NULL);

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Threading Max Completed\n");
    printf("Workers: %d | Array Size: %d | Global Max: %d | Time: %.4f sec\n",
           num_workers, array_size, global_max, time_spent);

    pthread_mutex_destroy(&lock);
    free(array);
    return 0;
}

