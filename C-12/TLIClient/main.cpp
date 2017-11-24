#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <tiuser.h>

int main(int argc, char *argv[]) {
    struct sockaddr_in server;
    struct t_call *tCall;
    char sendMsg[1024];
    char recvMsg[1024];
    int sock, flags, addrLength;


    if ((sock = t_open("/dev/tcp", O_RDWR, (struct t_info *) NULL)) < 0) {
        t_error("t_open");
        exit(1);
    }

    if (t_bind(sock, (struct t_bind *) NULL, (struct t_bind *) NULL) < 0) {
        t_error("t_bind");
        exit(1);
    }


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(57007);
    addrLength = sizeof (server);


    if (NULL == (tCall = (struct t_call *) t_alloc(sock, T_CALL, T_ADDR))) {
        t_error("t_call");
        exit(1);
    }


    tCall->addr.maxlen = addrLength;
    tCall->addr.len = addrLength;
    tCall->addr.buf = (char *) &server;
    tCall->opt.len = 0;
    tCall->udata.len = 0;


    if (0 > t_connect(sock, tCall, (struct t_call *) NULL)) {
        t_error("t_connect");
        exit(1);
    }

    while (strncasecmp(sendMsg, "exit", 4) != 0) {
        fgets(sendMsg, 1024, stdin);

        if (0 > t_snd(sock, sendMsg, sizeof (sendMsg), 0)) {
            t_error("t_snd");
            exit(1);
        }


        flags = 0;
        if (0 > t_rcv(sock, &recvMsg, sizeof (recvMsg), &flags)) {
            t_error("t_rcv");
            exit(1);
        }
       
        printf("%s\n", recvMsg);
        memset(recvMsg, 0, sizeof(recvMsg));

    }

    t_close(sock);
    return 0;
}