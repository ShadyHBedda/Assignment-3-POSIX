#include "count_race.h"

int length = 100;
int count;

int count1s (int *arrayInput)
{
    int i;
    count = 0;

    for (i=0; i<length; i++)
    {
        if (arrayInput[i] == 1)
        {
            count++;
        }
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
    printf("\nNumber of 1's is %d", count1s(array));
    printf("\nNumber of 1's found by race_count is %d", race_count(array, length, 10));

}