#include "essentials.h"

void refac_mat(Pixel **mat, Arbore *arbore, int x, int y, unsigned dim) {
    if (!arbore->tip) {
        /* If the tree's type is 0, then it is an internal node and doesn't contain values of interest.
           The function is called for the children of the current node.
           Clearly, the matrix is divided into 4 equal submatrices (by size). */
        refac_mat(mat, arbore->copii[0], x, y, dim / 2);
        refac_mat(mat, arbore->copii[1], x, y + dim / 2, dim / 2);
        refac_mat(mat, arbore->copii[2], x + dim / 2, y + dim / 2, dim / 2);
        refac_mat(mat, arbore->copii[3], x + dim / 2, y, dim / 2);
    } else {
        /* When we reach here, it means the current node is terminal,
           and the current submatrix will be assigned the "color" of the node. */
        for (int i = x; i < x + dim; i++) {
            for (int j = y; j < y + dim; j++) {
                mat[i][j].R = arbore->culoare.R;
                mat[i][j].G = arbore->culoare.G;
                mat[i][j].B = arbore->culoare.B;
            }
        }
    }
    return;
}

void citire_arbore(char* argv[]) {
    unsigned dim = 0;
    FILE* in = fopen(argv[2], "rb");
    DIE(in == NULL, "Could not open input file.\n")

    FILE* out = fopen(argv[3], "wb");
    DIE(out == NULL, "Could not open output file.\n")

    /* The matrix (image) size is read */
    fread(&dim, sizeof(unsigned), 1, in);
    
    /* The tree is built using a queue, where the tree's nodes are added one by one */
    Coada *coada = (Coada*)calloc(1, sizeof(Coada));
    DIE(coada == NULL, "Could not allocate queue.\n")

    Arbore *arbore = (Arbore*)calloc(1, sizeof(Arbore));
    DIE(arbore == NULL, "Could not allocate tree's root.\n")
    
    stai_la_coada(coada, arbore);
    while (coada->start) {
        fread(&coada->start->nod->tip, sizeof(unsigned char), 1, in);
        queue *temp = coada->start;
        if (!temp->nod->tip) {
            /* If the node type indicates it's internal, the node's children will be added at the end of the queue */
            temp->nod->copii = (Arbore**)calloc(4, sizeof(Arbore*));
            /* Allocating the array of pointers for the current node's children */
            DIE(temp->nod->copii == NULL, "Could not allocate a node's children array.\n")

            for (int i = 0; i < 4; i++) {
                temp->nod->copii[i] = (Arbore*)calloc(1, sizeof(Arbore));
                DIE(temp->nod->copii[i] == NULL, "Could not allocate a node.\n")
                /* Allocating each child and adding it to the queue */
                stai_la_coada(coada, temp->nod->copii[i]);
            }
            coada->start = coada->start->next;
            /* The work with the current node is finished, its allocated memory will be freed */
            free(temp);
        } else {
            /* If we reach here, it means the current node is terminal and we will read its colors */
            fread(&temp->nod->culoare, sizeof(Pixel), 1, in);
            temp->nod->tip = 1;
            /* Marking the node as terminal by modifying the "tip" field */
            coada->start = coada->start->next;
            /* Moving to the next node in the queue, because terminal nodes have no children */
            free(temp);
        }
    }
    /* Freeing the memory for the structure that holds pointers to the start and end of the queue
       because after the "while" loop ends, the queue is empty */
    free(coada);

    Pixel **mat = (Pixel**)calloc(dim, sizeof(Pixel*));
    DIE(mat == NULL, "Could not allocate matrix.\n")

    /* Dynamically allocating the matrix */
    for (int i = 0; i < dim; i++) {
        mat[i] = (Pixel*)calloc(dim, sizeof(Pixel));
        DIE(mat[i] == NULL, "Could not allocate a matrix line.\n")
    }

    /* Calling the matrix construction function */
    refac_mat(mat, arbore, 0, 0, dim);

    char string[] = "P6\n";
    fprintf(out, "%s", string);
    fprintf(out, "%u %u\n", dim, dim);
    fprintf(out, "255\n");
    /* Writing the header for a .ppm file */
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            while (fwrite(&mat[i][j], sizeof(Pixel), 1, out) == 0);
        }
        free(mat[i]);
    }
    /* Freeing the memory used in the program */
    free(mat);
    eliberare_arbore(arbore);
    fclose(in);
    fclose(out);
}
