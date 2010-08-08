#include "confpar.h"

#include<stdio.h>
#include<stdlib.h>
#include<libxml/tree.h>
#include<libxml/parser.h>

#include "debug.h"

/**
 * Function to initialize confpar_item_t to default values
 * @param item the confpar_item_t item to be initialised
 */
void confpar_item_init(confpar_item_t * item)
{
    if (item !=NULL)
    {
        item->key = NULL;
        item->value = NULL;
        item->children = NULL;
        item->next = NULL;
        LOG("Config Parser item initialised\n");
    }
    #ifdef DEBUG
    else
        LOG("Config Parser item to be initialised was NULL\n");
    #endif
}

/**
 * Function to initialize conpar_t object to default values
 * @param confpar confpar_t object to be initialized 
 */
void confpar_object_init(confpar_t * confpar)
{
    if (confpar != NULL)
    {
        confpar->filepath = NULL;
        confpar->root = NULL;
        LOG("Config Parser Object initialised\n");
    }
    #ifdef DEBUG
    else
        LOG("Config Parser Object to be initialised was NULL\n");
    #endif

}

#ifdef DEBUG
static void print_element_names(xmlNode * a_node, const xmlChar* parent)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE && parent != NULL) {
            printf("Parent : %s Element Name: '%s' contains %s\n", parent, cur_node->name, cur_node->content);
        }

        print_element_names(cur_node->children, cur_node->name);
    }
}
#endif

/**
 * Function that parses the config file and updates key values pairs
 * @param confpar confpar_t object which will contain all the info after the file is parsed
 */
int confpar_update(confpar_t * confpar)
{
    if (confpar->filepath == NULL)
    {
        LOG("Error: Filepath of file to be parsed was NULL");
        return -1;
    }

    /*
     * check for version mismatch of LIBXML
     */
    LIBXML_TEST_VERSION
    // read the XML config file
    xmlDocPtr doc;
    doc = xmlReadFile(confpar->filepath, NULL, 0);
    if (doc == NULL)
    {
        LOG("Error: Failed to open '%s' to read config\n", confpar->filepath);
        return -1;
    }
    #ifdef DEBUG
    // print the parsed xml document
        xmlNode *xmlRoot;
        xmlRoot = xmlDocGetRootElement(doc);
        print_element_names(xmlRoot, NULL);
    #endif
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 1;
}

