// stack.h

#ifndef STACK_H
#define STACK_H

#define _POSIX_C_SOURCE 199309L  // For clock_gettime and CLOCK_MONOTONIC
#define _GNU_SOURCE  // For CPU_ZERO and CPU_SET

#include <pthread.h>

#define NUMA_NODES 4
#define STACK_SIZE 1024  // Size of stack for each NUMA node

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
    pthread_mutex_t lock; // Mutex for stack operations
} Stack;

// Declare stacks array as extern
extern Stack stacks[NUMA_NODES];

// Initialize stacks
void init_stacks(void);

// Push a value onto the stack for a specific NUMA node
void push(int numa_node, int value);

// Pop a value from the stack for a specific NUMA node
int pop(int numa_node);

#endif // STACK_H
