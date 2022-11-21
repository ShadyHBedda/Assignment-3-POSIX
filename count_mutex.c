#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int length = 100000;
int count;
int count2;
int *array;
int numberOfThreads = 32;
int increment = 0;
pthread_mutex_t mutex;


int count1s()
{
    int i;
    count = 0;
    for (i = 0; i < length; i++)
    {
        if (array[i] == 1)
        {
            count++;
        }
    }
    return count;
}

void* count1s_parallel(void* arg) {
    int index = *(int*)arg;
    //printf("\nIndex: %d", index);
    //printf("\nIncrement: %d", increment);
    for (int i = index; i < index + increment; i++)
    {
        pthread_mutex_lock(&mutex);
        if (array[i] == 1)
        {
            count2++;
            //printf("\nCount2: %d", count2);
        }
        pthread_mutex_unlock(&mutex);
    }
    return arg;
}

int main(void)
{
    srand(time(0));
    int randArray[length];
    for(int i=0;i<length;i++)
        randArray[i]=rand()%6;   //Generate number between 0 and 5
    array = &randArray;
    /*
    printf("\nElements of the array::");
    for(int i=0;i<length;i++)
    {
        printf("\nElement number %d::%d",i+1,randArray[i]);
    }
    */
    increment = length/numberOfThreads;
    pthread_t th[numberOfThreads];
    count = 0;
    count2 = 0;
    clock_t start_sequential, end_sequential, start_multithreading, end_multithreading;
    start_multithreading = clock();
    printf("\nStart: %d", start_multithreading);
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < numberOfThreads; i++) {
        int* a = malloc(sizeof(int));
        *a = i * increment;
        if (pthread_create(&th[i], NULL, &count1s_parallel, a) != 0) {
            perror("Failed to create thread");
        }
    }
    for (int i = 0; i < numberOfThreads; i++) {
        int* r;
        if (pthread_join(th[i], (void**) &r) != 0) {
            perror("Failed to join thread");
        }
        free(r);
    }
    pthread_mutex_destroy(&mutex);
    end_multithreading = clock();
    start_sequential = clock();
    printf("\nNumber of 1's counted by sequential approach: %d", count1s());
    end_sequential = clock();
    printf("\nNumber of 1's counted by multithreading: %d", count2);
    printf("\n\nstart_sequential: %d", start_sequential);
    printf("\nend_sequential: %d", end_sequential);
    printf("\nTime taken for sequential approach: %.3f ms\n",1000.0*(end_sequential-start_sequential)/CLOCKS_PER_SEC);
    printf("\nstart_multithreading: %d", start_multithreading);
    printf("\nend_multithreading: %d", end_multithreading);
    printf("\nTime taken for multithreading approach: %.3f ms\n",1000.0*(end_multithreading-start_multithreading)/CLOCKS_PER_SEC);
    return 0;
}