/**
 * @file fsm.h
 * @brief an implementation for a FSM in C, this file contains 
 * all definitions required for FSM.
 * @author Ankur Shrivastava
 */

// forward declaration
struct fsm_object;

/**
 * @struct fsm_state fsm.h "fsm.h"
 * @brief Stores information regarding state
 */
struct fsm_state{
    /**
     * Stores the name of the state
     */
    char *name;
    /**
     * stores the function pointer for the state
     */
    void (*function)(struct fsm_object* ,int, void**);
    /**
     * pointer to the next state
     */
    struct fsm_state *next;
}; 

/**
 * @struct fsm_object fsm.h "fsm.h"
 * @brief stores info regarding state machine
 */
struct fsm_object{
    /**
     * pointer to the linked list of fsm_state structures
     */
    struct fsm_state * fsm_base;
    /**
     * name of current FSM state
     */
    struct fsm_state * fsm_cur_state;
    /**
     * number of argument passed to the nest state
     */
    char * fsm_cur_state_name;
    /**
     * pointer to current FSM state
     */
    int fsm_arg_num;
    /**
     * values of arguments passed to the next state
     */
    void ** fsm_arg_value;

    /**
     * specifies if this FSM is running in an fsm loop or not
     * setting it to zero will terminate the fsm
     */
    int fsm_running;
};

/**
 * Function to initialize the FSM
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 */
int fsm_init(struct fsm_object *obj);

/**
 * The FSM entry point, this is where execution of code begins in FSM.
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 */
int fsm_main(struct fsm_object *obj);

/**
 * Execution of next state takes place here
 * @details function fsm_next can be used without fsm_main when we want to handle
 * state execution and not rely on fsm_main 's loop
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 */
int fsm_next_state(struct fsm_object *obj);

/**
 * Function to add a new state to the FSM.
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 * @param state name of the state to be added.
 * @param fun name of the function to be executed for this state
 */
int fsm_add(struct fsm_object *obj, char *state, void (*fun)(struct fsm_object *, int, void **) );

/**
 * Function to add a default state to FSM.
 * @details Adds a default state to FSM, this is the function called at the start of the FSM
 * or in case of error, with the appropriate error code
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 * @param fun name of the function to be executed for this state
 */
int fsm_default(struct fsm_object *obj, void (*fun)(struct fsm_object *, int, void **) );

/**
 * Function to remove a state from the FSM.
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 * @param state name of state to be removed
 */
int fsm_remove(struct fsm_object *obj, char *state);

/**
 * Function to change state.
 * @details changes state to the new specified state, if the state does not exist returns error,
 * state change is not triggered till function calling fsm_to_state returns
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 * @param state name of state to change to
 * @param num number of arguments
 * @param arg arguments
 */
int fsm_to_state(struct fsm_object *obj, char *state, int num, void** arg);

/**
 * Function for FSM termination
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 */
void fsm_terminate(struct fsm_object *obj);

/**
 * Function clean the fsm_object cleaning (free) the states inside the objects
 * @param obj pointer to structure of type fsm_object, which defines the FSM
 */
void fsm_delete(struct fsm_object *obj);
