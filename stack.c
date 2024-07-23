#include "stack.h"

extern atomic_int top_value;

Node * hashTable[HASH_TABLE_SIZE];

int hashFunction(int key){
    return key %HASH_TABLE_SIZE;
}

int DirInsert(int key,int data){
    unsigned int hashIndex = hashFunction(key);
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return false;

    newNode->key = key;
    newNode->data = data;
    newNode->next = NULL;

    Node* current = hashTable[hashIndex];
    if (!current) {
        hashTable[hashIndex] = newNode;
    } else {
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }

    //printf("Inserted KEY: %d\n", key);
    return true;
}

int DirDelete(int key){
    unsigned int hashIndex = hashFunction(key);

    Node* current = hashTable[hashIndex];
    Node* prev = NULL;

    while (current && current->key != key) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        //printf("No key deleted!\n");
        return false;
    }

    if (prev) {
        prev->next = current->next;
    } else {
        hashTable[hashIndex] = current->next;
    }

    free(current);

    //printf("Deleted KEY: %d\n", key);
    return true;
};

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


void* push(void* arg) {

    ThreadData*data=(ThreadData*)arg;
    start_timer(&data->timer);

    struct timespec ts;
    ts.tv_sec=0;
    ts.tv_nsec=(rand()%100)*1000000;
    nanosleep(&ts,NULL);

    atomic_fetch_add(&top_value, 1);
    DirInsert(top_value,0);

    stop_timer(&data->timer);
    printf("Thread %d PUSH %.5f\n", data->thread_id, get_elapsed_time(&data->timer));
    return NULL;
}

void* pop(void* arg){

    ThreadData* data = (ThreadData*) arg;
    start_timer(&data->timer);

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = (rand() % 100) * 1000000; // Random sleep between 0 and 99 milliseconds
    nanosleep(&ts, NULL);  

    atomic_fetch_sub(&top_value, 1);
    int key=top_value;
    if(key==-2){
        atomic_fetch_add(&top_value,1);
        //printf("POP SKIPPED!\n");
        stop_timer(&data->timer);
        return NULL;
    }else{
        DirDelete(top_value+1);
    }
    stop_timer(&data->timer);
    printf("Thread %d POP %.5f\n", data->thread_id, get_elapsed_time(&data->timer));
    return NULL;
}
