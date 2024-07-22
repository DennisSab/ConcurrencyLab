#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>


#define NUM_THREADS 8
#define HASH_TABLE_SIZE 10

typedef struct Node{
    int key;
    int data;
    struct Node* next;
}Node;

int hashFunction(int key);

int DirInsert(int key,int data);

int DirDelete(int key);

void* push() ;

void* pop() ;
