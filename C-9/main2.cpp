
#include "routine.h"

int main(int argc, char** argv) {
    
    //signal (SIGINT, my_handler);
    
    thread_t main_thr;  
    main_thr = thr_self();
    
    thr_create(NULL, 0, sub_routine, 0, THR_NEW_LWP, &folks[0]);
    
    thr_exit((void *) main_thr);

    return 0;
}

