#define _POSIX_C_SOURCE 199309L  // For clock_gettime and CLOCK_MONOTONIC
#define _GNU_SOURCE  // For CPU_ZERO and CPU_SET

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <numa.h>
#include <numaif.h>
#include <sched.h>
#include <stdbool.h>
#include <unistd.h>


#define NUM_THREADS 64
#define HASH_TABLE_SIZE 1024
#define NUMA_NODES 4
#define PARTICION_SIZE (HASH_TABLE_SIZE/NUMA_NODES)

typedef struct Node{
    int key;
    int data;
    struct Node* next;
}Node;

typedef struct Timer{
    struct timespec start;
    struct timespec finish;
}Timer;

typedef struct ThreadData {
    int thread_id;
    Timer timer;
} ThreadData;


Node* hashTable[HASH_TABLE_SIZE];


int hashFunction(int key);

int getNumaNode(int index);

int DirInsert(int key,int data);

int DirDelete(int key);

void setThreadAffinity(int thread_id);

void start_timer(Timer* timer);

void stop_timer(Timer* timer);

double get_elapsed_time(Timer* timer);

void* push(void* arg);

void* pop(void* arg);
