#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

// Function to create a stack
Stack* create_stack(int capacity) {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->data = (int*)malloc(capacity * sizeof(int));
    pthread_mutex_init(&stack->lock, NULL);  // Initialize the mutex
    return stack;
}

// Function to push an element onto the stack
void push(Stack *stack, int item) {
    pthread_mutex_lock(&stack->lock);  // Acquire the lock
    if (stack->top == stack->capacity - 1) {
        printf("Stack overflow\n");
    } else {
        stack->data[++stack->top] = item;
        printf("Pushed: %d | Stack size: %d\n", item, stack->top + 1);
    }
    pthread_mutex_unlock(&stack->lock);  // Release the lock
}

// Function to pop an element from the stack
int pop(Stack *stack) {
    pthread_mutex_lock(&stack->lock);  // Acquire the lock
    if (stack->top == -1) {
        printf("Stack underflow\n");
        pthread_mutex_unlock(&stack->lock);  // Release the lock
        return -1;  // Return -1 to indicate stack underflow
    } else {
        int item = stack->data[stack->top--];
        printf("Popped: %d | Stack size: %d\n", item, stack->top + 1);
        pthread_mutex_unlock(&stack->lock);  // Release the lock
        return item;
    }
}

// Function to check if the stack is empty
int is_empty(Stack *stack) {
    pthread_mutex_lock(&stack->lock);  // Acquire the lock
    int empty = (stack->top == -1);
    pthread_mutex_unlock(&stack->lock);  // Release the lock
    return empty;
}

// Function to get the size of the stack
int size(Stack *stack) {
    pthread_mutex_lock(&stack->lock);  // Acquire the lock
    int size = stack->top + 1;
    pthread_mutex_unlock(&stack->lock);  // Release the lock
    return size;
}

// Function to destroy the stack
void destroy_stack(Stack *stack) {
    pthread_mutex_destroy(&stack->lock);  // Destroy the mutex
    free(stack->data);  // Free the stack array
    free(stack);  // Free the stack structure
}
