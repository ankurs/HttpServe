/**
 * @file confpar.c
 * @brief confpar reads through a config file and sets the options 
 * @details options can be accessed section by section #TODO
 */
#ifndef CONFPAR_H
#define CONFPAR_H

#define MAX_BUFFER_SIZE 102400 // thats 100 KB

typedef struct confpar_item confpar_item_t;

struct confpar_item
{
    char * key;
    char * value;
    confpar_item_t * children;
    confpar_item_t * next;
};

typedef struct
{
    char * filepath;
    confpar_item_t *root;
} confpar_t;

void confpar_item_init(confpar_item_t * item);
void confpar_object_init(confpar_t * confpar);
int confpar_update(confpar_t * confpar);

#endif 
