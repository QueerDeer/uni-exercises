#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    struct flock fl;

    fd = open(argv[2], O_RDWR);
    if (fd == -1)
    {
        printf ("Can't open file \n");
        return 1;
    }

    fl.l_type = F_WRLCK;
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

        fl.l_type = F_UNLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = atoi(argv[3]);
        fl.l_len = atoi(argv[4]);
        if (fcntl(fd, F_SETLK, &fl) == -1)
            printf("Free mem error.\n");
    }

    return 0;
}
