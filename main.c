#include "stack.h"
int ok;

atomic_int top_value;

int main(){

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    atomic_init(&top_value,-1);
    srand(time(NULL));

    for(int i=0;i<NUM_THREADS;++i){
        thread_data[i].thread_id=i;
        if(rand()%2==0){
            if(pthread_create(&threads[i],NULL,push,&thread_data[i])!=0){
                perror("Failed to create thread!\n");
            }
        }else{
            if (pthread_create(&threads[i],NULL,pop,&thread_data[i])!=0){
                perror("Failed to create thread!\n");  
            }
            
        }
    }
    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    // Print the final value of top_value
    printf("Final value of top_value: %d\n", atomic_load(&top_value));

    return 0;


}