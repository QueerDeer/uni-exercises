#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <errno.h>


int main(int argc, char *argv[])
{
    int mode;
    struct stat buffer;

    if(stat("testfile",&buffer)<0)
    {
        perror("stat(2)");
        return 2 ;
    }

    mode = buffer.st_mode;
    mode &= ~(S_IEXEC>>3);
    mode |= S_ISGID;

    if(chmod("testfile",mode)<0)
    {
        perror("chmod(2)");
        return 1;
    } 
    
    int fd;
    struct flock fl;

    fd = open(argv[2], O_RDWR);
    if (fd == -1)
    {
        printf ("Can't open file \n");
        return 1;
    }

    if (*argv[1] == 'w')
        fl.l_type = F_WRLCK;
    else if (*argv[1] == 'r')
        fl.l_type = F_RDLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = atoi(argv[3]);
    fl.l_len = atoi(argv[4]);

    if (fcntl(fd, F_SETLK, &fl) == -1)
    {
        if (errno == EACCES || errno == EAGAIN)
            printf ("Already locked by another process\n");
        else
            printf("Lock mem error.\n");
    }
    else
    {

        if (argv[5] == NULL)
            while(1){}
        else
            sleep(atoi(argv[5]));

        fl.l_type = F_UNLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = atoi(argv[3]);
        fl.l_len = atoi(argv[4]);
        if (fcntl(fd, F_SETLK, &fl) == -1)
            printf("Free mem error.\n");
    }

    return 0;
}
