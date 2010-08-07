/**
 * @file confpar.c
 * @brief confpar reads through a config file and sets the options 
 * @details options can be accessed section by section #TODO
 */
#ifndef CONFPAR_H
#define CONFPAR_H

typedef struct confpar_item confpar_item_t;

/**
 * @struct confpar_item confpar.h "confpar.h"
 * @brief stores key value for a config
 */
struct confpar_item
{
    /**
     * key for the item
     */
    char * key;

    /**
     * value corresponding to the key
     */    
    char * value;

    /**
     * pointer to the children of this node if any
     */
    confpar_item_t * children;

    /**
     * next value in the linked list
     */
    confpar_item_t * next;
};

/**
 * @brief stores info regarding the config file
 */
typedef struct
{
    /**
     * full path of the config file
     */
    char * filepath;

    /**
     * root element of the config information
     */
    confpar_item_t *root;
} confpar_t;

/**
 * Function to initialize confpar_item_t to default values
 * @param item the confpar_item_t item to be initialised
 */
void confpar_item_init(confpar_item_t * item);

/**
 * Function to initialize conpar_t object to default values
 * @param confpar confpar_t object to be initialized 
 */
void confpar_object_init(confpar_t * confpar);

/**
 * Function that parses the config file and updates key values pairs
 * @param confpar confpar_t object which will contain all the info after the file is parsed
 */
int confpar_update(confpar_t * confpar);

/**
 * Function to add item to a parent
 * @param item item to be added
 * @param parent parent where item has to be added
 */
int confpar_add_item(confpar_item_t * item, confpar_item_t * parent);

/**
 * Function to retrieve value of a key from the specified section
 * @param section section from which value has to be retrieved
 * @param key key for which value has to be retrieved
 */
char * confpar_get_value(char * section, char * key);

#endif 
