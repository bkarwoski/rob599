#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc == 1){
        fprintf(stderr, "usage: ./cryptogram <encrypt|decrypt> <password> <text>\n");
        return 1;
    }

    return 0;
}
