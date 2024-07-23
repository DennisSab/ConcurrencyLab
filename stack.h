#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>


#define NUM_THREADS 10
#define HASH_TABLE_SIZE 10

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


int hashFunction(int key);

int DirInsert(int key,int data);

int DirDelete(int key);

void start_timer(Timer* timer);

void stop_timer(Timer* timer);

double get_elapsed_time(Timer* timer);

void* push(void* arg);

void* pop(void* arg);
