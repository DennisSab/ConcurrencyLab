// hashtable.h

#ifndef HASHTABLE_H
#define HASHTABLE_H

#define _POSIX_C_SOURCE 199309L  // For clock_gettime and CLOCK_MONOTONIC
#define _GNU_SOURCE  // For CPU_ZERO and CPU_SET

#include <pthread.h>

#define NUMA_NODES 4
#define BUCKETS_PER_NODE 16  // Number of buckets per NUMA node

typedef struct Entry {
    int key;
    int value;
    struct Entry* next;
} Entry;

typedef struct Timer{
    struct timespec start;
    struct timespec finish;
}Timer;

typedef struct Hashtable {
    Entry* buckets[BUCKETS_PER_NODE];
    pthread_mutex_t lock; // Mutex for hashtable operations
    Timer timer;
} Hashtable;

// Declare hashtables array as extern
extern Hashtable hashtables[NUMA_NODES];

// Initialize hashtables
void init_hashtables(void);

// Insert a key-value pair into the hashtable for a specific NUMA node
void insert(int numa_node, int key, int value);

// Lookup a value by key in the hashtable for a specific NUMA node
int lookup(int numa_node, int key);

// Delete a key-value pair from the hashtable for a specific NUMA node
void delete(int numa_node, int key);

void start_timer(Timer* timer);

void stop_timer(Timer* timer);

double get_elapsed_time(Timer* timer);

#endif // HASHTABLE_H
