#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h> 
#include <fastrand.h>

#define PRIME 101  // A prime number larger than the range of keys

// Structure to store coefficients a and b for universal hashing
typedef struct {
    int a;
    int b;
} HashCoefficients;

Hashtable hashtables[NUMA_NODES];
HashCoefficients hash_coefficients[NUMA_NODES];

// Initialize universal hashing coefficients for each NUMA node
void init_hash_coefficients() {
    for (int i = 0; i < NUMA_NODES; ++i) {
        hash_coefficients[i].a = synchFastRandomRange(1,PRIME -1) ;// a is non-zero
        hash_coefficients[i].b = synchFastRandomRange(1,PRIME);
    }
}

// Universal hash function for NUMA node
unsigned int hash_to_numa_node(int key) {
    int numa_node = key % NUMA_NODES;  // Calculate initial NUMA node
    int a = hash_coefficients[numa_node].a;
    int b = hash_coefficients[numa_node].b;
    return ((a * key + b) % PRIME) % NUMA_NODES;
}

// Universal hash function for bucket within NUMA node
unsigned int hash_to_bucket(int key) {
    unsigned int numa_node = hash_to_numa_node(key);
    int a = hash_coefficients[numa_node].a;
    int b = hash_coefficients[numa_node].b;
    return ((a * key + b) % PRIME) % BUCKETS_PER_NODE;
}

void init_hashtables() {
    init_hash_coefficients();  // Initialize hashing coefficients

    for (int i = 0; i < NUMA_NODES; ++i) {
        for (int j = 0; j < BUCKETS_PER_NODE; ++j) {
            hashtables[i].buckets[j] = NULL;
        }
    }
}

void insert(int key, int value) {
    unsigned int numa_node = hash_to_numa_node(key);
    unsigned int bucket = hash_to_bucket(key);

    Hashtable* table = &hashtables[numa_node];
    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = NULL;

    Entry* current = table->buckets[bucket];
    if (current == NULL) {
        table->buckets[bucket] = new_entry;
    } else {
        while (current->next != NULL && current->key != key) {
            current = current->next;
        }
        if (current->key == key) {
            current->value = value;  // Update existing key
            free(new_entry);
        } else {
            current->next = new_entry;  // Insert new entry
        }
    }
}

int lookup(int key) {
    unsigned int numa_node = hash_to_numa_node(key);
    unsigned int bucket = hash_to_bucket(key);

    Hashtable* table = &hashtables[numa_node];
    Entry* current = table->buckets[bucket];

    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    return -1;  // Key not found
}

void delete(int key) {
    unsigned int numa_node = hash_to_numa_node(key);
    unsigned int bucket = hash_to_bucket(key);

    Hashtable* table = &hashtables[numa_node];

    Entry* current = table->buckets[bucket];
    Entry* prev = NULL;

    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                table->buckets[bucket] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
