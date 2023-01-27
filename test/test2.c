# include <stdio.h>

int main(int argc, char* argv[])
{
    int i = 0;
    char *str = "hello world";

    printf("hello world %d", 1);

    for (; i < argc; i++)
    {
        printf("%s", argv[i]);
    }
}
