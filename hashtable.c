// hashtable.c

#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

Hashtable hashtables[NUMA_NODES];

void init_hashtables() {
    for (int i = 0; i < NUMA_NODES; ++i) {
        for (int j = 0; j < BUCKETS_PER_NODE; ++j) {
            hashtables[i].buckets[j] = NULL;
        }
        if (pthread_mutex_init(&hashtables[i].lock, NULL) != 0) {
            perror("pthread_mutex_init");
            exit(EXIT_FAILURE);
        }
    }
}

unsigned int hash(int key) {
    return key % BUCKETS_PER_NODE;
}

void insert(int numa_node, int key, int value) {
    if (numa_node < 0 || numa_node >= NUMA_NODES) {
        fprintf(stderr, "Invalid NUMA node\n");
        return;
    }

    Hashtable* table = &hashtables[numa_node];
    unsigned int bucket = hash(key);

    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    if (new_entry == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = NULL;

    pthread_mutex_lock(&table->lock);
    start_timer(&table->timer);

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
    stop_timer(&table->timer);
    pthread_mutex_unlock(&table->lock);

    double elapsed_time = get_elapsed_time(&table->timer);
    printf("Insert operation in numa node %d took %f seconds\n",numa_node,elapsed_time);
}

int lookup(int numa_node, int key) {
    if (numa_node < 0 || numa_node >= NUMA_NODES) {
        fprintf(stderr, "Invalid NUMA node\n");
        return -1; // Indicate error
    }

    Hashtable* table = &hashtables[numa_node];
    unsigned int bucket = hash(key);

    pthread_mutex_lock(&table->lock);

    Entry* current = table->buckets[bucket];
    while (current != NULL) {
        if (current->key == key) {
            pthread_mutex_unlock(&table->lock);
            return current->value;
        }
        current = current->next;
    }

    pthread_mutex_unlock(&table->lock);
    return -1; // Key not found
}

void delete(int numa_node, int key) {
    if (numa_node < 0 || numa_node >= NUMA_NODES) {
        fprintf(stderr, "Invalid NUMA node\n");
        return;
    }

    Hashtable* table = &hashtables[numa_node];
    unsigned int bucket = hash(key);

    pthread_mutex_lock(&table->lock);
    start_timer(&table->timer);
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
            pthread_mutex_unlock(&table->lock);
            return;
        }
        prev = current;
        current = current->next;
    }

    stop_timer(&table->timer);
    pthread_mutex_unlock(&table->lock);

    double elapsed_time = get_elapsed_time(&table->timer);
    printf("Delete operation in numa node %d took %f seconds\n",numa_node, elapsed_time);
}


void start_timer(Timer* timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->start);
}

void stop_timer(Timer* timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->finish);
}

double get_elapsed_time(Timer* timer) {
    double start_sec = timer->start.tv_sec + timer->start.tv_nsec / 1e9;
    double finish_sec = timer->finish.tv_sec + timer->finish.tv_nsec / 1e9;
    return finish_sec - start_sec;
}
