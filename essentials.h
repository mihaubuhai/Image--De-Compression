#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct culori {
    unsigned char R, G, B;
} Pixel;

typedef struct arbore Arbore;

struct arbore {
    Pixel culoare;
    unsigned char tip;
    Arbore **copii;
};

typedef struct coada queue;

struct coada {
    Arbore *nod;
    queue *next;
};

typedef struct queue {
    queue *start, *final;
} Coada;

#define DIE(val, msg)           \
    if (val) {                  \
        fprintf(stderr, msg);    \
        exit(-1);               \
    }                           \

#define PATRAT(x) (x) * (x)

void analiza_mat(char* argv[]);

void citire_arbore(char* argv[]);

void divide_mat(Pixel**, unsigned, int, int, int, Arbore*);

void afisare_arbore(Arbore*, FILE*);

void eliberare_arbore(Arbore*);

void stai_la_coada(Coada*, Arbore*);

void eliberare_arbore(Arbore*);