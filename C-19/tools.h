
#ifndef TOOLS_H
#define TOOLS_H

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <aio.h>
#include <signal.h>
#include <iostream>
#include <string.h>
#include <errno.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define errMsg(msg)  do { perror(msg); } while (0)
#define IO_SIGNAL SIGUSR1

struct ioRequest {
    int reqNum;
    int status;
    struct aiocb *aiocbp;
};

static volatile sig_atomic_t gotSIGQUIT = 0;
extern char *destiny_file;

void quitHandler(int sig);
void aioSigHandler(int sig, siginfo_t *si, void *ucontext);

#endif /* TOOLS_H */

