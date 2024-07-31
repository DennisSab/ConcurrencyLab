// stack.c

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

Stack stacks[NUMA_NODES]; // Array of stacks, one for each NUMA node

void init_stacks() {
    for (int i = 0; i < NUMA_NODES; ++i) {
        stacks[i].top = NULL;
        if (pthread_mutex_init(&stacks[i].lock, NULL) != 0) {
            perror("pthread_mutex_init");
            exit(EXIT_FAILURE);
        }
    }
}

void push(int numa_node, int value) {
    if (numa_node < 0 || numa_node >= NUMA_NODES) {
        fprintf(stderr, "Invalid NUMA node\n");
        return;
    }

    Stack* stack = &stacks[numa_node];
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_node->data = value;
    new_node->next = NULL;

    pthread_mutex_lock(&stack->lock);
    new_node->next = stack->top;
    stack->top = new_node;
    pthread_mutex_unlock(&stack->lock);
}

int pop(int numa_node) {
    if (numa_node < 0 || numa_node >= NUMA_NODES) {
        fprintf(stderr, "Invalid NUMA node\n");
        return -1; // Indicate error
    }

    Stack* stack = &stacks[numa_node];
    pthread_mutex_lock(&stack->lock);
    
    if (stack->top == NULL) {
        pthread_mutex_unlock(&stack->lock);
        return -1; // Stack is empty
    }

    Node* top_node = stack->top;
    int value = top_node->data;
    stack->top = top_node->next;
    pthread_mutex_unlock(&stack->lock);

    free(top_node);
    return value;
}
