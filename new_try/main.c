// main.c

#include "hashtable.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <time.h>

#define THREADS_PER_NODE 16
#define NUMA_NODES 4 // Define the number of NUMA nodes

void* thread_func(void* arg) {
    int numa_node = *((int*)arg);
    
    // Set thread affinity to the NUMA node
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    // Define the core range based on NUMA node
    int start_core, end_core;
    switch (numa_node) {
        case 0:
            start_core = 0;
            end_core = 7;
            break;
        case 1:
            start_core = 8;
            end_core = 15;
            break;
        case 2:
            start_core = 16;
            end_core = 23;
            break;
        case 3:
            start_core = 24;
            end_core = 31;
            break;
        default:
            fprintf(stderr, "Invalid NUMA node\n");
            return NULL;
    }

    // Set affinity to cores of the given NUMA node
    for (int i = start_core; i <= end_core; ++i) {
        CPU_SET(i, &cpuset);
    }

    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np");
        return NULL;
    }

    int key = rand() % 100;
    int action = rand() % 2; // Randomly choose between 0 (insert) and 1 (delete)
    //printf("NUMA NODE:%d key:%d - value:%d\n",numa_node,key,action);

    if (action == 0) {
        int value = rand() % 1000; // Generate a random value
        insert(numa_node, key, value); // Insert a key-value pair
        //printf("Thread on NUMA node %d inserted key %d with value %d\n", numa_node, key, value);
    } else {
        int value = lookup(numa_node, key); // Lookup a value by key
        if (value != -1) {
            //printf("Thread on NUMA node %d found value %d for key %d\n", numa_node, value, key);
            delete(numa_node, key); // Delete the key-value pair
            //printf("Thread on NUMA node %d deleted key %d\n", numa_node, key);
        } else {
            // Key not found, attempt an insert to ensure action is taken
            int new_value = rand() % 1000;
            insert(numa_node, key, new_value);
            ///printf("Thread on NUMA node %d could not find key %d to delete, inserted with value %d\n", numa_node, key, new_value);
        }
    }
    
    return NULL;
}

int main() {
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

    // Cleanup
    for (int i = 0; i < NUMA_NODES; ++i) {
        pthread_mutex_destroy(&hashtables[i].lock);
    }

    return 0;
}
