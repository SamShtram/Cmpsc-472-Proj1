#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>

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

    // Shared memory for global max and semaphore
    int *global_max = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *global_max = array[0];
    sem_init(sem, 1, 1); // shared between processes

    clock_t start = clock();

    for (int i = 0; i < num_workers; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            int start_idx = i * chunk_size;
            int end_idx = (i == num_workers - 1) ? array_size : start_idx + chunk_size;
            int local_max = array[start_idx];
            for (int j = start_idx + 1; j < end_idx; j++)
                if (array[j] > local_max)
                    local_max = array[j];

            sem_wait(sem);
            if (local_max > *global_max)
                *global_max = local_max;
            sem_post(sem);

            free(array);
            exit(0);
        }
    }

    for (int i = 0; i < num_workers; i++) wait(NULL);

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Multiprocessing Max Completed\n");
    printf("Workers: %d | Array Size: %d | Global Max: %d | Time: %.4f sec\n",
           num_workers, array_size, *global_max, time_spent);

    sem_destroy(sem);
    munmap(global_max, sizeof(int));
    munmap(sem, sizeof(sem_t));
    free(array);
    return 0;
}
