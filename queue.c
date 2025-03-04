#include "essentials.h"

void stai_la_coada(Coada* coada, Arbore* nod) {
    /* The function receives a pointer to the queue as a parameter to avoid occupying the stack with too much memory.
       These operations do not require a pointer to the "Queue" structure.
       Since we are working with dynamically allocated variables, pointers to them are already retained. */
    if (!coada->final) {
        /* "If the if statement entered here", it means the queue is not allocated, but only the pointers to its start and end.
           The first node of the queue is allocated, to which "final" points. */
        coada->final = (queue*)calloc(1, sizeof(queue));
        DIE(coada->final == NULL, "Could not allocate a queue element.\n")
        //Then, "start" will actually point to the first node in the entire queue.
        coada->start = coada->final;
        coada->final->nod = nod;
        return;
    } else {
        /* Executing these commands means there are other nodes in the queue,
           and the new node must be added to the end of the queue. */
        coada->final->next = (queue*)calloc(1, sizeof(queue));
        DIE(coada->final->next == NULL, "Could not allocate a queue element.\n")
        coada->final->next->nod = nod;
        coada->final = coada->final->next;
        return; 
    }
}

void afisare_arbore(Arbore* arbore, FILE* out) {
    Coada* coada = (Coada*)calloc(1, sizeof(Coada));
    DIE(coada == NULL, "Could not allocate queue.\n")

    stai_la_coada(coada, arbore);
    /* The root node of the tree is introduced into the queue. */
    while (coada->start) {
        /* As long as the queue has elements, or in other words,
           for each node in the tree,
           the following commands are executed. */
        queue *temp = coada->start;
        if (!temp->nod->tip) {
            /* If the node has descendants, they must be added to the queue,
               which is done in the "for" loop below.
               The node type is written to the file. */
            fwrite(&temp->nod->tip, sizeof(unsigned char), 1, out);
            for (int i = 0; i < 4; i++) {
                stai_la_coada(coada, temp->nod->copii[i]);
            }
        } else {
            /* If these commands are executed, it means the current node is terminal.
               Relevant information about this node is written to the file. */
            fwrite(&temp->nod->tip, sizeof(unsigned char), 1, out);
            fwrite(&temp->nod->culoare, sizeof(Pixel), 1, out);
            /* The command above is valid because the size of the "Pixel" structure
               is 3 bytes, exactly the number of data types in the structure. */
        }
        /* The current node in the queue is no longer useful, moving to the next one. */
         coada->start = coada->start->next;
        /* Free the allocated memory for this node. */
        free(temp);
    }
    free(coada);
}

void eliberare_arbore(Arbore* arbore) {
    /* This is a recursive function that frees the memory allocated for each node in the tree.
       No check is needed for an empty tree, because:
           1) We are working with a non-empty tree.
           2) The if statement below will not call this function for the children of terminal nodes. */
    if (arbore->copii) {
        /* If the node is not terminal, the function is called recursively for its children. */
        for (int i = 0; i < 4; i++) {
            eliberare_arbore(arbore->copii[i]);
        }
        /* After executing the function for the children of this node, the array of pointers to children is freed. */
        free(arbore->copii);
    }
    /* The memory allocated for the node is freed, regardless of its type. */
    free(arbore);
}
