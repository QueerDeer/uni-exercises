
#include "tools.h"

int main(int argc, char *argv[]) {
    struct ioRequest *ioList;
    struct aiocb *aiocbList;
    struct sigaction sa;

    int s, j;
    int numReqs;
    int openReqs;
    int size = 0;
    int offset = 0;

    char *files[argc];
    int nfiles = 0;
    int borders[argc];

    if (argc < 2) {
        fprintf(stderr,
                "Usage: %s <source> [-<leftbyte> -<rightbyte>] ... <destiny>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    int i = 1;
    while (i < argc - 1) {
        files[nfiles] = argv[i];

        if (argv[i + 1][0] != '-') {
            borders[nfiles] = 0;
            i += 1;
        } else {
            borders[nfiles] = 10 * atoi(argv[i + 1]) + atoi(argv[i + 2]);
            i += 3;
        }

        nfiles++;
    }
    numReqs = nfiles;
    destiny_file = argv[argc - 1];


    ioList = (ioRequest*) calloc(numReqs, sizeof (struct ioRequest));
    if (ioList == NULL)
        errExit("calloc");

    aiocbList = (aiocb*) calloc(numReqs, sizeof (struct aiocb));
    if (aiocbList == NULL)
        errExit("calloc");


    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);

    sa.sa_handler = quitHandler;
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
        errExit("sigaction");

    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sa.sa_sigaction = aioSigHandler;
    if (sigaction(IO_SIGNAL, &sa, NULL) == -1)
        errExit("sigaction");


    for (j = 0; j < numReqs; j++) {
        ioList[j].reqNum = j;
        ioList[j].status = EINPROGRESS;
        ioList[j].aiocbp = &aiocbList[j];

        ioList[j].aiocbp->aio_fildes = open(files[j], O_RDONLY);
        if (ioList[j].aiocbp->aio_fildes == -1)
            errExit("open");
        printf("opened %s on descriptor %d\n", files[j],
                ioList[j].aiocbp->aio_fildes);


        if (borders[j] == 0) {
            struct stat st;
            stat(files[j], &st);
            size = st.st_size;
            offset = 0;
        } else {
            size = -(borders[j] % 10) + (borders[j] / 10);
            offset = -borders[j] / 10;
        }

        ioList[j].aiocbp->aio_buf = malloc(size);
        if (ioList[j].aiocbp->aio_buf == NULL)
            errExit("malloc");

        ioList[j].aiocbp->aio_nbytes = size;
        ioList[j].aiocbp->aio_reqprio = 0;
        ioList[j].aiocbp->aio_offset = offset;
        ioList[j].aiocbp->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        ioList[j].aiocbp->aio_sigevent.sigev_signo = IO_SIGNAL;
        ioList[j].aiocbp->aio_sigevent.sigev_value.sival_ptr =
                &ioList[j];

        s = aio_read(ioList[j].aiocbp);
        if (s == -1)
            errExit("aio_read");
    }

    openReqs = numReqs;

    while (openReqs > 0) {
        //sleep(10);

        if (gotSIGQUIT) {

            printf("got SIGQUIT; canceling I/O requests: \n");

            for (j = 0; j < numReqs; j++) {
                if (ioList[j].status == EINPROGRESS) {
                    printf("    Request %d on descriptor %d:", j,
                            ioList[j].aiocbp->aio_fildes);
                    s = aio_cancel(ioList[j].aiocbp->aio_fildes,
                            ioList[j].aiocbp);
                    if (s == AIO_CANCELED)
                        printf("I/O canceled\n");
                    else if (s == AIO_NOTCANCELED)
                        printf("I/O not canceled\n");
                    else if (s == AIO_ALLDONE)
                        printf("I/O all done\n");
                    else
                        errMsg("aio_cancel");
                }
            }

            gotSIGQUIT = 0;
        }


        printf("aio_error():\n");
        for (j = 0; j < numReqs; j++) {
            if (ioList[j].status == EINPROGRESS) {
                printf("    for request %d (descriptor %d) progress: ",
                        j, ioList[j].aiocbp->aio_fildes);
                ioList[j].status = aio_error(ioList[j].aiocbp);

                switch (ioList[j].status) {
                    case 0:
                        printf("I/O succeeded\n");
                        break;
                    case EINPROGRESS:
                        printf(".");
                        break;
                    case ECANCELED:
                        printf("Canceled\n");
                        break;
                    default:
                        errMsg("aio_error");
                        break;
                }

                if (ioList[j].status != EINPROGRESS)
                    openReqs--;
            }
        }
    }

    printf("All I/O requests completed\n");


    printf("aio_return():\n");
    for (j = 0; j < numReqs; j++) {
        ssize_t s;

        s = aio_return(ioList[j].aiocbp);
        printf("    for request %d (descriptor %d): %ld\n",
                j, ioList[j].aiocbp->aio_fildes, (long) s);
    }

    exit(EXIT_SUCCESS);
}

