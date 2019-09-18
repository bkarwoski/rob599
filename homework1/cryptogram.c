#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 1){
        fprintf(stderr, "usage: ./cryptogram <encrypt|decrypt> <password> <text>\n");
        return 1;
    }
    if (strcmp(argv[1], "encrypt")==0){

    }
    if (strcmp(argv[1], "decrypt")==0){

    }
    else {
        fprintf(stderr, "expected command encrypt or decrypt.\n");
        return 1;
    }

    return 0;
}
