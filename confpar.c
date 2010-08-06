#include "confpar.h"

#include<stdio.h>
#include<stdlib.h>

void confpar_item_init(confpar_item_t * item)
{
    if (item !=NULL)
    {
        item->key = NULL;
        item->value = NULL;
        item->children = NULL;
        item->next = NULL;
        #ifdef DEBUG
        fprintf(stderr,"Item initialised\n");
        #endif
    }
    #ifdef DEBUG
    else
        fprintf(stderr,"Item to be initialised was NULL\n");
    #endif
}

void confpar_object_init(confpar_t * confpar)
{
    if (confpar != NULL)
    {
        confpar->filepath = NULL;
        confpar->root = NULL;
        #ifdef DEBUG
        fprintf(stderr,"Config Parser Object initialised\n");
        #endif
    }
    #ifdef DEBUG
    else
        fprintf(stderr,"Config Parser Object to be initialised was NULL\n");
    #endif

}

int confpar_update(confpar_t * confpar)
{
    if (confpar->filepath == NULL)
        return -1;
    #ifdef DEBUG
    fprintf(stderr,"Reading from Config file %s\n",confpar->filepath);
    #endif
    size_t buffsize = 0;
    char * buffer;
    buffer = malloc(1024);
    FILE *f = fopen(confpar->filepath,"r");
    if (f == NULL)
    {
        #ifdef DEBUG
        fprintf(stderr,"Error while reading from Config file %s\n",confpar->filepath);
        #endif
        return -1;
    }
    size_t buffersize = 0;
    size_t count = 0;
    while (1)
    {
        count = fread(buffer, 1024, 1, f);
        if (count < 1)
            break;
        buffersize += count;
        buffer = realloc(buffer,buffersize+1024);
    }
    buffer[count] = '\0';
    #ifdef DEBUG
    fprintf(stderr,"Config file %s was read\n%s",confpar->filepath,buffer);
    #endif
   
    return buffsize;
}

