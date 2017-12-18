
#include "tools.h"

char *destiny_file;

void quitHandler(int sig) {
    gotSIGQUIT = 1;
}

void aioSigHandler(int sig, siginfo_t *si, void *ucontext) {
    write(STDOUT_FILENO, "I/O completion signal received\n", 31);

    struct ioRequest *ioReq = (ioRequest*) (si->si_value.sival_ptr);
    struct aiocb aiocb;
    char buf[(int) ioReq->aiocbp->aio_nbytes];

    int fd = open(destiny_file, O_RDWR | O_APPEND);
    if (fd == -1) {
        printf("Error at open(): %s\n", strerror(errno));
        exit(1);
    }

    memcpy(buf, (const void*) (ioReq->aiocbp->aio_buf),
            (int) ioReq->aiocbp->aio_nbytes * sizeof (char));
    memset(&aiocb, 0, sizeof (struct aiocb));

    aiocb.aio_fildes = fd;
    aiocb.aio_buf = buf;
    aiocb.aio_nbytes = (int) ioReq->aiocbp->aio_nbytes;

    if (aio_write(&aiocb) == -1)
        errExit("aio_write");

    printf("    for writing in file with descriptor %d progress ",
            aiocb.aio_fildes);
    #sleep(10);
    while (true) {
        switch (aio_error(&aiocb)) {
            case 0:
                printf("I/O succeeded\n");
                break;
            case EINPROGRESS:
                printf(".");
                continue;
            case ECANCELED:
                printf("Canceled\n");
                break;
            default:
                errMsg("aio_error");
                break;
        }
        break;
    }

    int err = aio_error(&aiocb);
    int ret = aio_return(&aiocb);

    if (err != 0)
        errExit("aio_error");

    if (ret != (int) aiocb.aio_nbytes)
        errExit("aio_return");

    close(fd);
}

