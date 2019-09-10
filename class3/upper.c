#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("usage: ./upper <text>\n");
        return 1;
    }
    printf("input is: %s\n", argv[1]);

    return 0;
}