#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>

int race_count(int *arrayInput, int arrayLength, int numberOfThreads) {
    int T[numberOfThreads];
    for (int i = 0; i < numberOfThreads; i++) {
        T[i] = 0;
    }

    omp_set_num_threads(numberOfThreads);
    int parallel_count = 0;
    #pragma omp parallel
    {
        int i;
        #pragma omp for
        for (i=0; i<arrayLength; i++)
        {
            if (arrayInput[i] == 1)
            {
                parallel_count++;
                T[omp_get_thread_num()]++;
            }
        }
    }

    for (int i = 0; i < numberOfThreads; i++) {
        printf("\nNumber of 1's found by Thread %d is %d", i, T[i]);
    }
    return parallel_count;
};