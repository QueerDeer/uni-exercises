#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define SHARED_MEMORY_OBJECT_SIZE 50
#define SHM_CREATE 1
#define SHM_READ  3
#define SHM_CLOSE  4

void usage(const char * s, int argc) {

    printf("only %i args, ooh\n", argc);
    printf("Usage: %s <create|write|write|unlink> ['text'] shared_memory_object_name\n", s);
}

int main (int argc, char ** argv) {
    int shm, len, cmd, mode = 0;
    void *addr;
    char * shared_memory_object_name;

    if ( argc < 3 ) {
        usage(argv[0], argc);
        return 1;
    }

    if ( (!strcmp(argv[1], "create") || !strcmp(argv[1], "write")) && (argc == 4) ) {
        len = strlen(argv[2]);
        len = (len<=SHARED_MEMORY_OBJECT_SIZE)?len:SHARED_MEMORY_OBJECT_SIZE;
        mode = O_CREAT;
        cmd = SHM_CREATE;
        shared_memory_object_name = argv[3];
    } else if ( ! strcmp(argv[1], "read" ) && (argc == 3)) {
        cmd = SHM_READ;
        shared_memory_object_name = argv[2];
    } else if ( ! strcmp(argv[1], "unlink" ) && (argc == 3)) {
        cmd = SHM_CLOSE;
        shared_memory_object_name = argv[2];
    } else {
        usage(argv[0], argc);
        return 1;
    }

    if ( (shm = shm_open(shared_memory_object_name, mode|O_RDWR, 0777)) == -1 ) {
        perror("shm_open");
        return 1;
    }

    if ( cmd == SHM_CREATE ) {
        if ( ftruncate(shm, SHARED_MEMORY_OBJECT_SIZE+1) == -1 ) {
            perror("ftruncate");
            return 1;
        }
    }

    addr = mmap(0, SHARED_MEMORY_OBJECT_SIZE+1, PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
    if ( addr == (char*)-1 ) {
        perror("mmap");
        return 1;
    }

    switch ( cmd ) {
    case SHM_CREATE:
        memcpy(addr, argv[2], len);
        printf("Shared memory filled in. You may run '%s read %s' to see value.\n",
                argv[0], shared_memory_object_name);
        break;
    case SHM_READ:
        printf("Got from shared memory: %s\n", addr);
        break;
    }

    munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
    close(shm);

    if ( cmd == SHM_CLOSE ) {
        shm_unlink(shared_memory_object_name);
    }

    return 0;
}
