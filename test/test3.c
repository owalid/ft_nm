# include <stdio.h>

void random_number(int *array, int size)
{
    int i = 0;

    for (; i < size; i++)
    {
        array[i] = rand() % 100;
    }
}

int main(int argc, char* argv[])
{
    int i = 0;
    char *str = "hello world";
    int array[10];

    bzero(array, 10);
    printf("hello world %d", 1);

    for (; i < argc; i++)
    {
        printf("%s", argv[i]);
    }

    random_number(array, 10);

    for (i = 0; i < 10; i++)
    {
        printf("%d", array[i]);
    }
}

