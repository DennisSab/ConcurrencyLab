#include <stdio.h>
#include "stack.h"

int main() {
    Stack *stack = create_stack(5);

    // Push and pop elements to test the stack
    push(stack, 10);
    push(stack, 20);
    push(stack, 30);

    pop(stack);
    pop(stack);
    pop(stack);

    // Clean up
    destroy_stack(stack);

    return 0;
}
