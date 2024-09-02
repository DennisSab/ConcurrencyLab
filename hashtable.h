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
    Timer timer;
} Hashtable;

// Declare hashtables array as extern
extern Hashtable hashtables[NUMA_NODES];

// Hash function 1: Determines NUMA node
unsigned int hash_to_numa_node(int key);

// Hash function 2: Determines bucket within NUMA node
unsigned int hash_to_bucket(int key,int numa_node);

// Initialize hashtables
void init_hashtables(void);

// Insert a key-value pair into the hashtable for a specific NUMA node
void insert(int numa_node, int key, int value);

// Lookup a value by key in the hashtable for a specific NUMA node
int lookup(int key);

// Delete a key-value pair from the hashtable for a specific NUMA node
void delete(int key);

#endif // HASHTABLE_H
