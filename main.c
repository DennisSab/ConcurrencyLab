#include "hashtable.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <stdatomic.h>
#include "fastrand.h"
#include <time.h> 

#define THREADS_PER_NODE 16
#define NUMA_NODES 4 // Define the number of NUMA nodes
#define TOTAL_OPERATIONS 1000000 // Total number of operations to perform

_Atomic int top = -1;
struct timespec start,end;

typedef struct {
    int numa_node;
    int operations_per_thread;
    int thread_id;
} thread_arg_t;

void* thread_func(void* arg) {
    thread_arg_t* thread_arg = (thread_arg_t*)arg;
    int numa_node = thread_arg->numa_node;
    int thread_id=thread_arg->thread_id;
    int operations = thread_arg->operations_per_thread;

    if (thread_id==0){
        clock_gettime(CLOCK_MONOTONIC, &start);
    }

    // Set thread affinity to the NUMA node
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    // Define the core range based on NUMA node
    int start_core, end_core, start_core2, end_core2;
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

    for (int i = 0; i < operations; ++i) {
        int key = synchFastRandomRange(0, 100);  // Generate a random key between 0 and 99
        int value = synchFastRandomRange(0, 1000);
        
        atomic_fetch_add(&top, 1);
        insert(numa_node, key, value); // Insert a key-value pair
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_threads = atoi(argv[1]);

    if (num_threads != 4 && num_threads != 8 && num_threads != 16 && num_threads != 32 && num_threads != 64) {
        fprintf(stderr, "Please specify thread count as 4, 8, 16, 32, or 64.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[num_threads];
    thread_arg_t thread_args[num_threads];
    
    synchFastRandomSetSeed(thread_args->thread_id+1);

    init_hashtables();

    // Calculate operations per thread
    int operations_per_thread = TOTAL_OPERATIONS / num_threads;
    int remaining_operations = TOTAL_OPERATIONS % num_threads;

    for (int i = 0; i < num_threads; ++i) {
        thread_args[i].numa_node = i % NUMA_NODES;
        thread_args[i].operations_per_thread = operations_per_thread;

        // Distribute remaining operations evenly among threads
        if (i < remaining_operations) {
            thread_args[i].operations_per_thread++;
        }

        if (pthread_create(&threads[i], NULL, thread_func, &thread_args[i]) != 0) {
            fprintf(stderr, "Error setting thread affinity\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_threads; ++i) {
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
