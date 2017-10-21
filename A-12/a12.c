#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char ** argv) {
    void *handle;
    void (*test)(int *);
    char ch;

    printf("e for enableing lib, d for disabling lib, i for testing lib's function, q for quitting\n");
    while ((ch = getchar()) != 'q') {
        if (ch == 'e') {
            handle = dlopen("libcall.so", RTLD_LAZY);

            if (!handle) {
                printf ("handle-error %s\n", dlerror());
                continue;
            }
            printf ("handle success\n");

            test = dlsym(handle, "testFunc");

            if (!test) {
                printf ("sym-error %s\n", dlerror());
                continue;
            }
            printf ("sym success\n");
        }
        if (ch == 'd') {
            dlclose(handle);
            printf ("lib was closed succesfully or was never exist\n");
            test = NULL;
        }
        if (ch == 'i' && test) {
            printf ("let's go\n");
            int i = atoi(argv[1]);
            (*test)(&i);
            printf ("result is %d\n", i);
        }
    }
    return 0;
}
