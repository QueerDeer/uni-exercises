#include <stdio.h>
#include <dlfcn.h>

int main()
{
    void *handle;
    double (*computExp)(double, int);
    char* (*getCat)();

    handle = dlopen("libcompute.so", RTLD_LAZY);

    if (!handle) {
        printf ("can't handle lib %s\n", dlerror());
        return 1;
    }

    computExp = dlsym(handle, "computExp");
    getCat = dlsym(handle, "getCat");

    if (!computExp || !getCat)
    {
        printf ("can't sym some funcs %s\n", dlerror());
        return 2;
    }

    printf("Let's rise 2 to 18-th power: it should be %f\n", (*computExp)(2,18));
    printf("Let's get the cat out of the library. Here it is: %s !\n", (*getCat)());

    dlclose(handle);

    return 0;
}
