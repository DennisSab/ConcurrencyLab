// main.c

#include "hashtable.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <stdatomic.h>
#include <time.h>

#define THREADS_PER_NODE 16
#define NUMA_NODES 4 // Define the number of NUMA nodes

_Atomic int top=-1;

void* thread_func(void* arg) {
    int numa_node = *((int*)arg);
    
    // Set thread affinity to the NUMA node
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    // Define the core range based on NUMA node
    int start_core, end_core,start_core2,end_core2;
    switch (numa_node) {
        case 0:
            start_core = 0;
            end_core = 7;
            start_core2 = 32;
            end_core2 = 39;
            break;
        case 1:
            start_core = 8;
            end_core = 15;
            start_core2 = 40;
            end_core2 = 47;
            break;
        case 2:
            start_core = 16;
            end_core = 23;
            start_core2 = 48;
            end_core2 = 55;
            break;
        case 3:
            start_core = 24;
            end_core = 31;
            start_core2 = 56;
            end_core2 = 63;
            break;
        default:
            fprintf(stderr, "Invalid NUMA node\n");
            return NULL;
    }

    // Set affinity to cores of the given NUMA node
    for (int i = start_core; i <= end_core; ++i) {
        CPU_SET(i, &cpuset);
    }
    for (int i = start_core2; i <= end_core2; ++i) {
        CPU_SET(i, &cpuset);
    }

    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np");
        return NULL;
    }

    int key = rand() % 100;
    int action = rand() % 2; // Randomly choose between 0 (insert) and 1 (delete)
    //printf("NUMA NODE:%d key:%d - value:%d\n",numa_node,key,action);

     if (action == 0) { // Insert operation
        int value = rand() % 1000; // Generate a random value
        atomic_fetch_add(&top, 1);
        insert(numa_node,key, value); // Insert a key-value pair
    } else { // Delete operation
        if (top == -1) {
            // Stack is empty, skip the delete operation
            return NULL;
        }
        
        int value = lookup(key); // Lookup a value by key
        if (value != -1) {
            delete(key); // Delete the key-value pair
            atomic_fetch_sub(&top, 1);
        } else {
            return NULL;
        }
    }
    
    return NULL;
}

int main() {

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    pthread_t threads[NUMA_NODES * THREADS_PER_NODE];
    int numa_nodes[NUMA_NODES * THREADS_PER_NODE];

    srand(time(NULL));

    init_hashtables();

    for (int i = 0; i < NUMA_NODES * THREADS_PER_NODE; ++i) {
        numa_nodes[i] = i % NUMA_NODES;
        if (pthread_create(&threads[i], NULL, thread_func, &numa_nodes[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUMA_NODES * THREADS_PER_NODE; ++i) {
        pthread_join(threads[i], NULL);
    }

     clock_gettime(CLOCK_MONOTONIC, &end);
    // Calculate elapsed time in milliseconds
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed_milliseconds = (seconds * 1e3) + (nanoseconds * 1e-6);
    printf("Execution time: %.3f milliseconds\n", elapsed_milliseconds);

    return 0;
}
