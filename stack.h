#ifndef STACK_H
#define STACK_H

#include <pthread.h>

// Define the stack structure
typedef struct {
    int *data;        // Array to hold stack elements
    int top;          // Index of the top element
    int capacity;     // Capacity of the stack
    pthread_mutex_t lock;  // Mutex for thread-safe operations
} Stack;

// Function to create a stack
Stack* create_stack(int capacity);

// Function to push an element onto the stack
void push(Stack *stack, int item);

// Function to pop an element from the stack
int pop(Stack *stack);

// Function to check if the stack is empty
int is_empty(Stack *stack);

// Function to get the size of the stack
int size(Stack *stack);

// Function to destroy the stack
void destroy_stack(Stack *stack);

#endif // STACK_H
