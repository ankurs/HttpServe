/**
 * @file fsm.c
 * @brief an implementation for a FSM in C, this file contains 
 * implementation of definitions.
 * @author Ankur Shrivastava
 */

#include "fsm.h"
#include<stdlib.h>
#include<string.h>

#include "debug.h"

/**
 * Function to initialize the FSM
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 */
int fsm_init(struct fsm_object *obj)
{
    LOG("initializing  FSM\n");
    //initialize everything to Null or 0
    obj->fsm_base = NULL;
    obj->fsm_cur_state_name = NULL;
    obj->fsm_arg_num = 0;
    obj->fsm_arg_value = NULL;
    obj->fsm_running = 0;
    return 0;
}

/**
 * Execution of next state takes place here
 * @details function fsm_next can be used without fsm_main when we want to handle
 * state execution and not rely on fsm_main 's loop
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 */
int fsm_next_state(struct fsm_object *obj)
{
    if (obj == NULL)
    {
        LOG("FSM Object passed to '%s' is NULL\n",__func__);
        return -1;
    }
    struct fsm_state *tmp = obj->fsm_base;
    if ((obj->fsm_base==NULL)||(obj->fsm_cur_state_name==NULL))
    {        
        LOG("FSM Not Initialised OR Terminated!!\n");
        return -1;
    }
    while ((tmp->name != obj->fsm_cur_state_name)&&(tmp!=NULL))
        tmp = tmp->next;
    if (tmp == NULL)
    {
        LOG("Error: State '%s' NOT FOUND for execution !!\n",obj->fsm_cur_state_name);
        return -1;
    }
    LOG("executing next state -> %s\n",tmp->name);
    tmp->function(obj,obj->fsm_arg_num,obj->fsm_arg_value);
    LOG("execution of state complete -> %s\n",tmp->name);
    return 0;
}

/**
 * The FSM entry point, this is where execution of code begins in FSM.
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 */
int fsm_main(struct fsm_object *obj)
{
    LOG("Starting FSM main loop\n");
    if (obj == NULL)
    {
        LOG("FSM object passed to fsm_main loop is NULL\n");
        return -1;
    }
    obj->fsm_running = 1;
    while (!fsm_next_state(obj) && obj->fsm_running != 0);
    return 0;
}

/**
 * Function to add a new state to the FSM.
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 * @param state name of the state to be added.
 * @param fun name of the function to be executed for this state
 */
int fsm_add(struct fsm_object *obj, char *state, void (*fun)(struct fsm_object *, int ,void **) )
{
    struct fsm_state *tmp = obj->fsm_base;
    struct fsm_state *new_state = malloc(sizeof(struct fsm_state));
    LOG("Adding new state '%s'\n",state);
    while(tmp->next)
        tmp = tmp->next;
    new_state->name = state;
    new_state->function = fun;
    new_state->next = NULL;
    tmp->next = new_state;
    return 0;
}

/**
 * Function to remove a state from the FSM.
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 * @param state name of state to be removed
 */
int fsm_remove(struct fsm_object *obj,char *state)
{
    if (!strcmp(state,"default"))
    {
        LOG("Error: Cannot remove default state!!!\n");
        return -1;
    }
    struct fsm_state *to_del;
    struct fsm_state *tmp=obj->fsm_base;
    while((tmp->next!=NULL)&&(strcmp(tmp->next->name,state)))
        tmp=tmp->next;
    if (tmp == NULL)
    {
        LOG("Error: State '%s' NOT FOUND while trying to remove it\n", state);
        return -1;
    }
    to_del = tmp->next;
    tmp->next = tmp->next->next;
    free(to_del);
    LOG("State '%s' removed\n", state);
    return 0;
}

/**
 * Function to change state.
 * @details changes state to the new specified state, if the state does not exist returns error,
 * state change is not triggered till function calling fsm_to_state returns
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 * @param state name of state to change to
 * @param num number of arguments
 * @param arg arguments
 */
int fsm_to_state(struct fsm_object *obj, char *state, int num, void** arg)
{
    struct fsm_state *tmp=obj->fsm_base;
    while((tmp!=NULL)&&(strcmp(tmp->name,state)))
        tmp=tmp->next;
    if (tmp == NULL)
    {
        LOG("New state '%s' NOT FOUND while setting next state!\n",state);
        return -1;
    }
    LOG("Current State- %s\nNext State- %s\n", obj->fsm_cur_state_name, state);
    obj->fsm_cur_state = tmp;
    obj->fsm_cur_state_name = tmp->name;
    obj->fsm_arg_num = num;
    obj->fsm_arg_value=arg;
    return 0;
}

/**
 * Function to add a default state to FSM.
 * @details Adds a default state to FSM, this is the function called at the start of the FSM
 * or in case of error, with the appropriate error code
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 * @param fun name of the function to be executed for this state
 */
int fsm_default(struct fsm_object *obj, void (*fun)(struct fsm_object *, int ,void **) )
{
    obj->fsm_base = malloc(sizeof(struct fsm_state));
    obj->fsm_base->name="default";
    obj->fsm_base->function = fun;
    obj->fsm_base->next = NULL;
    // set current state to default
    obj->fsm_cur_state = obj->fsm_base;
    obj->fsm_cur_state_name = obj->fsm_base->name;
    LOG("Default State Added!\n");
    return 0;
}

/**
 * Function for FSM deletion
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 */
void fsm_delete(struct fsm_object *obj)
{
    // delete all states to prevent memory leek
    struct fsm_state *tmp = obj->fsm_base;
    struct fsm_state *to_del=tmp;
    LOG("Deleting FSM\n");
    while(tmp)
    {
        to_del = tmp;
        tmp=tmp->next;
        free(to_del);
    }
    // reset FSM base to NULL causes while loop in fsm_main to quit
    // terminating the program
    obj->fsm_cur_state = NULL;
    obj->fsm_cur_state_name = NULL;
    obj->fsm_base = NULL;
    obj->fsm_running = 0;
}
