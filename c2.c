#include "essentials.h"

void analiza_mat(char* argv[]) {
    /* This function is similar to the one in "cerinta1.c",
       except that it writes the image size and the colors of each terminal node in the tree to a binary file, given as an argument to the executable. */
    char c = 0;
    FILE* in = fopen(argv[3], "rt");
    DIE(in == NULL, "Could not open input file.\n")

    FILE* out = fopen(argv[4], "wb");
    DIE(out == NULL, "Could not open output file\n")

    Arbore *arbore = (Arbore*)calloc(1, sizeof(Arbore));
    DIE(arbore == NULL, "Could not allocate tree's root.\n")

    int prag = (int)strtol(argv[2], NULL, 10);
    DIE(prag == __LONG_MAX__ || prag == -__LONG_MAX__ - 1, "Given threshold is invalid.\n")

    while (c != ' ') {
        c = fgetc(in);
    }
    unsigned dim = 0;
    fscanf(in, "%u", &dim);
    fseek(in, 5, SEEK_CUR);

    Pixel **mat = (Pixel**)calloc(dim, sizeof(Pixel*));
    DIE(mat == NULL, "Could not allocate matrix.\n")

    int i, j;
    for (i = 0; i < dim; i++) {
        mat[i] = (Pixel*)calloc(dim, sizeof(Pixel));
        DIE(mat[i] == NULL, "Could not allocate a matrix line.\n")
        
        for (j = 0; j < dim; j++) {
            fread(&mat[i][j], sizeof(Pixel), 1, in);
        }
    }

    // Writing the image size to the compressed file
    fwrite(&dim, sizeof(unsigned), 1, out);

    divide_mat(mat, dim, 0, 0, prag, arbore);

    afisare_arbore(arbore, out);    
    eliberare_arbore(arbore);

    for (i = 0; i < dim; i++) {
        free(mat[i]);
    }
    free(mat);
    fclose(out);
    fclose(in);
}
