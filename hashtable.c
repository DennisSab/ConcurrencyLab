// hashtable.c

#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

Hashtable hashtables[NUMA_NODES];

// Hash function 1: Determines NUMA node
unsigned int hash_to_numa_node(int key) {
    return key % 3;
}

// Hash function 2: Determines bucket within NUMA node
unsigned int hash_to_bucket(int key) {
    return key % 15;
}


void init_hashtables() {
    for (int i = 0; i < NUMA_NODES; ++i) {
        for (int j = 0; j < BUCKETS_PER_NODE; ++j) {
            hashtables[i].buckets[j] = NULL;
        }
    }
}

void insert(int numa_node,int key, int value) {
    unsigned int bucket = hash_to_bucket(key);

    Hashtable* table = &hashtables[numa_node];

    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = NULL;

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = (rand() % 100) * 1000000;
    nanosleep(&ts, NULL);  // Simulate delay

    Entry* current = table->buckets[bucket];
    if (current == NULL) {
        table->buckets[bucket] = new_entry;
    } else {
        while (current->next != NULL && current->key != key) {
            current = current->next;
        }
        if (current->key == key) {
            current->value = value; // Update existing key
            free(new_entry);
        } else {
            current->next = new_entry; // Insert new entry
        }
    }
}

int lookup(int key) {
    for (int numa_node = 0; numa_node < NUMA_NODES; ++numa_node) {
        unsigned int bucket = hash_to_bucket(key);

        Hashtable* table = &hashtables[numa_node];
        Entry* current = table->buckets[bucket];

        while (current != NULL) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
    }
    return -1; // Key not found
}

void delete(int key) {
    for (int numa_node = 0; numa_node < NUMA_NODES; ++numa_node) {
        unsigned int bucket = hash_to_bucket(key);

        Hashtable* table = &hashtables[numa_node];

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = (rand() % 100) * 1000000;
        nanosleep(&ts, NULL);  // Simulate delay

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
}
