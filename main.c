/**
 * @todo define how config will work with the system and the basic structure
 * @todo get a basic libevent implementation working 
 */
#include "fsm.h"
#include "confpar.h"
#include<stdio.h>

void abc(struct fsm_object *obj, int val,void **arg)
{
    #ifdef DEBUG
    fprintf(stderr,"In default State\n");
    #endif
    // state -> default
    printf("%d\n",val);
    printf("%s\n",obj->fsm_cur_state_name);
    fsm_to_state(obj,"hello",0,NULL);
}

void pqr(struct fsm_object *obj, int val,void **arg)
{
    #ifdef DEBUG
    fprintf(stderr,"In qwerty State\n");
    #endif
    // state -> qwerty
    printf("%d\n",val);
    printf("%s\n",obj->fsm_cur_state_name);
    fsm_to_state(obj,"default",0,NULL);
}


void xyz(struct fsm_object *obj, int val,void **arg)
{    
    #ifdef DEBUG
    fprintf(stderr,"In hello State\n");
    #endif
    // state -> hello
    printf("%d\n",val);
    printf("%s\n",obj->fsm_cur_state_name);
//  fsm_terminate(obj);
    fsm_to_state(obj,"qwerty",0,NULL);
}

/*
int main()
{
    // create FSM object
    struct fsm_object obj;
    // initialize it
    fsm_init(&obj);
    // set default function
    fsm_default(&obj,abc);
    // add more states
    fsm_add(&obj,"qwerty",pqr);
    fsm_add(&obj,"hello",xyz);
    // starte the main FSM loop
    //fsm_main(&obj);
    fsm_main(NULL);
    confpar_t conf;
    confpar_object_init(&conf);    
    conf.filepath = "abc.xml";
    confpar_update(&conf);
    return 0;
}
*/
