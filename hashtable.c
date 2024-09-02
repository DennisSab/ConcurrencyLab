// hashtable.c

#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fastrand.h>

Hashtable hashtables[NUMA_NODES];
unsigned int a[NUMA_NODES];
unsigned int b[NUMA_NODES];
const unsigned int p = 101;  // Prime number slightly larger than 100


// Hash function 1: Determines NUMA node
unsigned int hash_to_numa_node(int key) {
    return ((a[0] * key + b[0]) % p) % NUMA_NODES;
}

// Hash function 2: Determines bucket within NUMA node
unsigned int hash_to_bucket(int key, int numa_node) {
    return ((a[numa_node] * key + b[numa_node]) % p) % BUCKETS_PER_NODE;
}


void init_hashtables() {
    for (int i = 0; i < NUMA_NODES; ++i) {
        // Randomly select a and b for universal hashing
        a[i] = synchFastRandomRange(1,p -1) ;// a is non-zero
        b[i] = synchFastRandomRange(1,p);

        for (int j = 0; j < BUCKETS_PER_NODE; ++j) {
            hashtables[i].buckets[j] = NULL;
        }
    }
}


void insert(int numa_node,int key, int value) {
    unsigned int bucket = hash_to_bucket(key, numa_node);

    Hashtable* table = &hashtables[numa_node];

    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = NULL;


    Entry* current = table->buckets[bucket];
    Entry* prev=NULL;
    
    while (current != NULL) {
        if (current->key == key) {
            current->value = value; // Update existing key
            free(new_entry); // Free the new entry as it's not needed anymore
            return;
        }
        prev = current;
        current = current->next;
    }

    // If we reach here, the key does not exist, so add the new entry
    if (prev == NULL) {
        table->buckets[bucket] = new_entry;
    } else {
        prev->next = new_entry;
    }
}
