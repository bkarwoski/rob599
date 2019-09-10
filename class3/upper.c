#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("usage: ./upper <text>\n");
        return 1;
    }
    //printf("input is: %s\n", argv[1]);

    for (int i = 1; i < argc; i++)
    {
        for (int j = 0; j < strlen(argv[i]); j++)
        {
if(argv[i][j] > 91){
    argv[i][j] -= 32;
}
            printf("%c\n", argv[i][j]);
        }
    }

    return 0;
}