#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
int main (int argc, char *argv[]) {
    if(argc < 2 || argc == 1) {
        printf("compilier failed, use 1 argument only\n");
        return 1;
    }
    char *stg = argv[1];
    int size = strlen(argv[1]);
    char *last3 = stg + size - 3;
    if(strcmp(last3, "scl") != 0) {
        printf("File name extension incorrect\n");
        return 1;
    }
    lexer_start(argv[1]);
    return 0;
}