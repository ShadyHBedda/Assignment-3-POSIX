#include "count_race.h"

int length = 100;
int count;

typedef struct countInput {
    int *arrayPointerStruct;
    int arrayLengthStruct;
    int numberOfThreadsStruct;
    int *arrayOfResults;
} countInput;

int count1s (struct countInput countInput_count1s)
{
    int i;
    count = 0;
    int length = countInput_count1s.arrayLengthStruct;
    for (i=0; i<length; i++)
    {
        if (countInput_count1s.arrayPointerStruct[i] == 1)
        {
            count++;
        }
    }

    return count;
}

int psix(struct countInput countInput_psix, void *functionParameter)
{

    int numberOfThreads = countInput_psix.numberOfThreadsStruct;
    int results[numberOfThreads];
    for (int i = 0; i < numberOfThreads; i++) {
        results[i] = 0;
    }
    countInput_psix.arrayOfResults = &results;
    int i;
    pthread_t *threads;
    threads = (pthread_t *)malloc(numberOfThreads * sizeof(pthread_t));
    for (i = 0; i < numberOfThreads; i++)
    {
        pthread_create(&threads[i], NULL, (void *)functionParameter, (void *) countInput_psix);

    }
    int j;
    for (j = 0; j < numberOfThreads; j++)
    {
        pthread_join(threads[j], NULL);
    }

    for (int resultsIterator = 0; resultsIterator < numberOfThreads; resultsIterator++) {
        printf("\nNumber of 1's found by Thread %d is %d", resultsIterator, results[resultsIterator]);
    }
    return count;
}

int main(void)
{
    srand(time(0));
    int randArray[length],i;
    for(i=0;i<length;i++)
        randArray[i]=rand()%6;   //Generate number between 0 and 5
    /*
    printf("\nElements of the array::");
    for(i=0;i<length;i++)
    {
        printf("\nElement number %d::%d",i+1,randArray[i]);
    }
    */

    int *array = &randArray;
    printf("\nNumber of 1's is %d", count1s());
    printf("\nNumber of 1's found by race_count is %d", race_count(array, length, 10));

}