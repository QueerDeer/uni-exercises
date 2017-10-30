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
        return 2;
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
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    fd = open(argv[2], O_RDWR);
    if (fd == -1)
    {
        printf ("Can't open file \n");
        return 1;
    }


    fcntl(fd, F_GETLK, &fl);
    if ((fl.l_type == F_RDLCK && *argv[1] == 'r') || (fl.l_type == F_WRLCK && *argv[1] == 'w'))
    {
        printf ("Already locked by another process\n");
        return 0;
    }
    else
    {
        if (lseek(fd, atoi(argv[3]), SEEK_SET) == -1)
            printf("lseek error.\n");

        if (*argv[1] == 'w')
        {
            char buffer[atoi(argv[4])];
            read (0, buffer, atoi(argv[4]));
            if (write(fd, buffer, sizeof(buffer)) != sizeof(buffer))
                printf("Write error.\n");
        }

        else if (*argv[1] == 'r')
        {
            char buffer[atoi(argv[4])];
            if (read(fd, buffer, atoi(argv[4])) != atoi(argv[4]))
                printf("Read error.\n");
            else
                printf("%s\n", buffer);
        }

    }

    return 0;
}
