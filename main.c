#include "essentials.h"

int main(int argc, char* argv[]) {
    if (!strncmp(argv[1], "-c", strlen("-c"))) {
        analiza_mat(argv);
    } else if (!strncmp(argv[1], "-d", strlen("-d"))){
        citire_arbore(argv);
    } else {
        fprintf(stderr, "Unknown %s arg!\n", argv[1]);
        exit(-1);
    }
    return 0;
}