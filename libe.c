#ifdef _EVENT_HAVE_PTHREADS
int evthread_use_pthreads(void);
#define EVTHREAD_USE_PTHREADS_IMPLEMENTED
#endif

#include "debug.h"
#include<event2/event.h>
#include<event2/listener.h>
#include<stdio.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#include<dlfcn.h>


void callback (struct evconnlistener *listener, evutil_socket_t sock,
        struct sockaddr *addr, int len, void *ptr)
{
    LOG("Got A Connection\n");
}

int main()
{
    struct event_base *ev_base = event_base_new();
    if (ev_base == NULL)
    {
        LOG("event_base structure returned by libevent is NULL\n");
        return -1;
    }

    #ifdef DEBUG
    // log all the supported methods
    const char ** methods = event_get_supported_methods();
    int i = 0;
    for (i = 0; methods[i] != NULL; ++i)
    {
        LOG("Support for '%s' detected\n",methods[i]); 
    }     
    // log supported features
    enum event_method_feature f;
    f = event_base_get_features(ev_base);
    if ((f & EV_FEATURE_ET))
            LOG("Edge-triggered events are supported.\n");
    if ((f & EV_FEATURE_O1))
            LOG("O(1) event notification is supported.\n");
    if ((f & EV_FEATURE_FDS))
            LOG("All FD types are supported.\n");
    // log method in use
    const char *method = event_base_get_method(ev_base);
    if (method != NULL)
    {
        LOG("Method '%s' in use by libevent\n",method);
    }
    else
    {
        LOG("No Methods in use by libevent\n");
        return -1;
    }
    #endif

    // Test code for libevent

    struct timeval time_val;
    time_val.tv_sec = 1;
    time_val.tv_usec = 0;

    //initialize the socket address
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9900);

    struct evconnlistener * ev_list;
    ev_list = evconnlistener_new_bind(ev_base, callback, NULL,            
            LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 10, (struct sockaddr *) &addr,
            sizeof(struct sockaddr_in));
    evconnlistener_enable(ev_list);

    // dynamic 
    void *lib_handle;
    void (*fn)(int);
    char *error;

    lib_handle = dlopen("sharedtest.so.1.0",RTLD_LAZY);
    {
        if (!lib_handle)
        {
            fprintf(stderr,"%s\n",dlerror());
            return -1;
        }
    }

    fn = dlsym(lib_handle,"stest");
    if ((error = dlerror()) != NULL)
    {
        fprintf("%s\n",error);
        return -1;        
    }

    (*fn)(10);
    dlclose(lib_handle);
    // done dynamic

    event_base_dispatch(ev_base);

    /*
    while (1)
    {
        // set timeout to 1 sec
        event_base_loopexit(ev_base, &time_val);
        event_base_dispatch(ev_base);
        LOG("Tick\n");
    } 
    */

    /* NON BLOCKING IO
     #include<fcntl.h>
     if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
             flags = 0;
     fcntl(fd, F_SETFL, flags | O_NONBLOCK);
     */


    // free the event_base structure
    event_base_free(ev_base);
    return 0;
}
