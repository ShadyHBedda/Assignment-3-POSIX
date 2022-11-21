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
    printf("\nNumber of 1's is %d", count1s());
    printf("\nNumber of 1's counted by multithreading: %d", globalSum);
    return 0;
}