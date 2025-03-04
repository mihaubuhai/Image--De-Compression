#include "essentials.h"

void divide_mat(Pixel **mat, unsigned dim, int x, int y, int prag, Arbore* arbore) {
    /* This function is similar to the one in the file "impart_mat_c1.c",
       except that it analyzes each leaf node.
       In this function, only the tree is constructed. */
    int i, j;
    unsigned long rosu = 0, verde = 0, albastru = 0, medie = 0;
    /* The above variables are chosen as "unsigned long"
       to avoid overflow situations. */
    for (i = x; i < x + dim; i++) {
        for (j = y; j < y + dim; j++) {
            rosu += mat[i][j].R;
            verde += mat[i][j].G;
            albastru += mat[i][j].B;
        }
    }
    rosu /= (dim * dim);
    verde /= (dim * dim);
    albastru /= (dim * dim);

    unsigned long cantitate_rosu = 0, cantitate_albastru = 0, cantitate_verde = 0; 
    for (i = x; i < x + dim; i++) {
        for (j = y; j < y + dim; j++) {
            cantitate_rosu = PATRAT(rosu - mat[i][j].R);
            cantitate_albastru = PATRAT(albastru - mat[i][j].B);
            cantitate_verde = PATRAT(verde - mat[i][j].G);
            medie += cantitate_rosu + cantitate_verde + cantitate_albastru;
        }
    }
    medie /= (3 * dim * dim);
    
    if (medie > prag) {
        arbore->copii = (Arbore**)calloc(4, sizeof(Arbore*));
        DIE(arbore->copii == NULL, "Could not allocate the children of a tree's node.\n")

        for (i = 0; i < 4; i++) {
            arbore->copii[i] = (Arbore*)calloc(1, sizeof(Arbore));
            DIE(arbore->copii[i] == NULL, "Could not allocate a node.\n")
        }
        divide_mat(mat, dim / 2, x, y, prag, arbore->copii[0]);
        divide_mat(mat, dim / 2, x, y + dim / 2, prag, arbore->copii[1]);
        divide_mat(mat, dim / 2, x + dim / 2, y + dim / 2, prag, arbore->copii[2]);
        divide_mat(mat, dim / 2, x + dim / 2, y, prag, arbore->copii[3]);
    } else {
        arbore->culoare.R = rosu;
        arbore->culoare.B = albastru;
        arbore->culoare.G = verde;
        arbore->tip = 1;
    }
}
