#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int length = 100;
int count;
int count2;
int *array;
int numberOfThreads = 4;
int increment = 0;

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
        if (array[i] == 1)
        {
            count2++;
            //printf("\nCount2: %d", count2);
        }
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
    printf("\nNumber of 1's is %d", count1s());
    printf("\nNumber of 1's counted by multithreading: %d", count2);
    return 0;
}