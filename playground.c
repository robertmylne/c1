
#include <stdio.h>

int changeNumber(int* number)
{
    *number = 10;

    return 0;
}

int main()
{
    int number = 5;

    changeNumber(&number);

    printf("%i", number);

    return 0;
}