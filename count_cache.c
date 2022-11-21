#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int length = 100000;
int count;
int *array;
int numberOfThreads = 32;
int increment = 0;

typedef struct filler{
    int *privateCount;
    char *fillerArray;

} filler;

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
    filler *filler1 = (filler*)arg;
    int *index = (filler1->privateCount);
    int sum = 0;
    //printf("\nIndex: %d", index);
    //printf("\nIncrement: %d", increment);
    for (int i = *index; i < *index + increment; i++)
    {
        if (array[i] == 1)
        {
            sum++;
            //printf("\nCount2: %d", count2);
        }
    }
    *(filler1->privateCount) = sum;
    return filler1;
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
    clock_t start_sequential, end_sequential, start_multithreading, end_multithreading;
    start_multithreading = clock();
    for (int i = 0; i < numberOfThreads; i++) {
        struct filler *fillerInstancePointer = malloc(262144);
        if ( fillerInstancePointer != NULL )
        {
            fillerInstancePointer->privateCount = malloc(sizeof(int));
            fillerInstancePointer->fillerArray = malloc(262140);

        }

        *(fillerInstancePointer->privateCount) = i * increment;
        if (pthread_create(&th[i], NULL, &count1s_parallel, &fillerInstancePointer) != 0) {
            perror("Failed to create thread");
        }
    }
    int globalSum = 0;
    for (int i = 0; i < numberOfThreads; i++) {
        filler* r;
        if (pthread_join(th[i], (void**) &r) != 0) {
            perror("Failed to join thread");
        }
        globalSum += *(r->privateCount);
        free(r);
    }

    end_multithreading = clock();
    start_sequential = clock();
    printf("\nNumber of 1's counted by sequential approach: %d", count1s());
    end_sequential = clock();
    printf("\nNumber of 1's counted by multithreading: %d", globalSum);
    printf("\n\nstart_sequential: %d", start_sequential);
    printf("\nend_sequential: %d", end_sequential);
    printf("\nTime taken for sequential approach: %.3f ms\n",1000.0*(end_sequential-start_sequential)/CLOCKS_PER_SEC);
    printf("\nstart_multithreading: %d", start_multithreading);
    printf("\nend_multithreading: %d", end_multithreading);
    printf("\nTime taken for multithreading approach: %.3f ms\n",1000.0*(end_multithreading-start_multithreading)/CLOCKS_PER_SEC);
    return 0;
}