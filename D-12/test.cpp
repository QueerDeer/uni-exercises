#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


#define SHARED_MEMORY_OBJ_SIZE 128
#define SHM_CREATE 1
#define SHM_READ 2
#define SHM_CLOSE 0

void usage(const char * s, int argc) {

    printf("only %i args, ooh\n", argc);
    printf("Usage: %s <create|write|write|unlink> /shared_memory_object_name\n",
            s);
}

int main (int argc, char ** argv) {
    int shm, len, cmd, mode = 0;
    void *addr;  
    struct flock lock;
    char buffer[SHARED_MEMORY_OBJ_SIZE];

    if ( argc < 3 ) {
        usage(argv[0], argc);
        return 1;
    }
    
    memset(&lock, 0, sizeof(lock));

    if ( (!strcmp(argv[1], "create") || !strcmp(argv[1], "write")) ) {
        mode = O_CREAT;
        cmd = SHM_CREATE;
        lock.l_type = F_WRLCK;       
    } else if ( ! strcmp(argv[1], "read" ) ) {
        cmd = SHM_READ;
        lock.l_type = F_RDLCK; 
    } else if ( ! strcmp(argv[1], "unlink" ) ) {
        cmd = SHM_CLOSE;
    } else {
        usage(argv[0], argc);
        return 1;
    }

    if ( (shm = shm_open(argv[2], mode|O_RDWR, 0777)) == -1 ) {
        perror("shm_open");
        return 1;
    }

    if ( cmd == SHM_CREATE ) {
        if ( ftruncate(shm, SHARED_MEMORY_OBJ_SIZE+1) == -1 ) {
            perror("ftruncate");
            return 1;
        }
    }

    addr = mmap(0, SHARED_MEMORY_OBJ_SIZE+1, PROT_WRITE|PROT_READ, MAP_SHARED, 
                shm, 0);
    if ( addr == (char*)-1 ) {
        perror("mmap");
        return 1;
    }
    
    //fcntl(shm, F_SETLKW, &lock);
    int was_locked = fcntl(shm, F_SETLK, &lock);
    if (was_locked && cmd) {
        perror("fcntl");
        return 1;
    }

    switch ( cmd ) {
    case SHM_CREATE:
        fgets(buffer, SHARED_MEMORY_OBJ_SIZE, stdin);
        len = strlen(buffer);
        len = (len<=SHARED_MEMORY_OBJ_SIZE)?len:SHARED_MEMORY_OBJ_SIZE;
        memcpy(addr, buffer, len);
        printf("Shared memory filled in. You may run '%s read %s' to see it.\n",
                argv[0], argv[2]);
        break;
    case SHM_READ:
        printf("Got from shared memory: %s\n", (char *)addr);
        getchar();
        break;
    }

    lock.l_type = F_UNLCK;
    fcntl(shm, F_SETLK, &lock);
    munmap(addr, SHARED_MEMORY_OBJ_SIZE);
    close(shm);

    if ( cmd == SHM_CLOSE ) {
        shm_unlink(argv[2]);
    }

    return 0;
}
