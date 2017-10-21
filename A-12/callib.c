#include <stdio.h>

void __attribute__((constructor)) init() {
    printf ("i'm lib, i was loaded explicity\n");
}

void __attribute__((destructor)) fini() {
    printf ("i'm lib, i was closed\n");
}

void testFunc(int *i) {
    printf("temporary text\n");
    *i+=5;
    return;
}
