#include "stack.h"

extern atomic_int top_value;

Node * hashTable[HASH_TABLE_SIZE];

int hashFunction(int key){
    return key %HASH_TABLE_SIZE;
}

int getNumaNode(int index){
    return index/PARTICION_SIZE;
}

int DirInsert(int key, int data) {
    int index = hashFunction(key);
    int numaNode = getNumaNode(index);

    Node* newNode = (Node*)numa_alloc_onnode(sizeof(Node), numaNode);
    if (!newNode) {
        perror("Allocation failed");
        return 0;
    }

    newNode->key = key;
    newNode->data = data;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
    return 1;
}

int DirDelete(int key) {
    int index = hashFunction(key);
    Node* current = hashTable[index];
    Node* prev = NULL;

    while (current && current->key != key) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        return 0;
    }

    if (prev) {
        prev->next = current->next;
    } else {
        hashTable[index] = current->next;
    }

    numa_free(current, sizeof(Node));
    return 1;
}

void setThreadAffinity(int thread_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    
    int core_ids[NUMA_NODES][16] = {
        {0, 1, 2, 3, 4, 5, 6, 7, 32, 33, 34, 35, 36, 37, 38, 39}, // NUMA node 0
        {8, 9, 10, 11, 12, 13, 14, 15, 40, 41, 42, 43, 44, 45, 46, 47}, // NUMA node 1
        {16, 17, 18, 19, 20, 21, 22, 23, 48, 49, 50, 51, 52, 53, 54, 55}, // NUMA node 2
        {24, 25, 26, 27, 28, 29, 30, 31, 56, 57, 58, 59, 60, 61, 62, 63}  // NUMA node 3
    };
    
    int numaNode = thread_id % NUMA_NODES;
    int core_id = core_ids[numaNode][thread_id / NUMA_NODES];

    CPU_SET(core_id, &cpuset);

    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np");
    }
}

void start_timer(Timer* timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->start);
}

void stop_timer(Timer* timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->finish);
}

double get_elapsed_time(Timer* timer) {
    double start_sec = (timer->start.tv_sec*1000.0) + timer->start.tv_nsec / 1e6;
    double finish_sec = (timer->finish.tv_sec*1000.0)+ timer->finish.tv_nsec / 1e6;
    return finish_sec - start_sec;
}


void* push(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    setThreadAffinity(data->thread_id);


    start_timer(&data->timer);

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = (rand() % 100) * 1000000;
    nanosleep(&ts, NULL);  // Simulate delay

    int key = atomic_fetch_add(&top_value, 1);
    DirInsert(key, 0);

    stop_timer(&data->timer);
    printf("Thread %d PUSH took %.4f milliseconds\n", data->thread_id, get_elapsed_time(&data->timer));

    return NULL;
}

void* pop(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    setThreadAffinity(data->thread_id);

    start_timer(&data->timer);

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = (rand() % 100) * 1000000;
    nanosleep(&ts, NULL);  // Simulate delay

    int key = atomic_fetch_sub(&top_value, 1);
    if (key >= 0) {
        DirDelete(key + 1);
    } else {
        atomic_fetch_add(&top_value, 1); // Undo decrement if key < 0
    }

    stop_timer(&data->timer);

    printf("Thread %d POP took %.4f milliseconds\n", data->thread_id, get_elapsed_time(&data->timer));

    return NULL;
}
