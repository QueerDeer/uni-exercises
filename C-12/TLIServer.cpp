#include "types.hpp"
#include "POSIXSpawner.cpp"

int main(int argc, char *argv[]) {
    struct sockaddr_in server, *client;
    struct hostent *hostE;
    struct t_bind tBind;
    struct t_call *tCall;

    char recvMsg[1024];
    int sock, sd, flags, addrLength, nmb;
    pid_t child;


    if (0 > (sock = t_open("/dev/tcp", O_RDWR, (struct t_info *) NULL))) {
        t_error("t_open");
        exit(1);
    }
    
    memset((void*) &server, 0, sizeof (server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(57007);
    addrLength = sizeof (server);

    tBind.addr.maxlen = addrLength;
    tBind.addr.len = addrLength;
    tBind.addr.buf = (char *) &server;
    tBind.qlen = 5;

    if (t_bind(sock, &tBind, (struct t_bind *) NULL) < 0) {
        t_error("t_bind");
        exit(1);
    }


    if (NULL == (tCall = (struct t_call *) t_alloc(sock, T_CALL, T_ADDR))) {
        t_error("t_alloc");
        exit(1);
    }


    for (;;) {
        if (0 > (t_listen(sock, tCall))) {
            t_error("t_listen");
            exit(1);
        }
        
        if (NULL == (client = (struct sockaddr_in *) tCall->addr.buf)) {
            perror("client");
        }
        
        hostE = gethostbyname("Solar");
        printf("\t%s (%s:%hu)\n", hostE->h_name, inet_ntoa(client->sin_addr),
                ntohs(client->sin_port));

        if (0 > (sd = t_open("/dev/tcp", O_RDWR, (struct t_info *) NULL))) {
            t_error("t_open");
            exit(1);
        }

        if (0 > t_bind(sd, (struct t_bind *) NULL, (struct t_bind *) NULL)) {
            t_error("t_bind");
            exit(1);
        }
        if (0 > t_accept(sock, sd, tCall)) {
            t_error("t_accept");
            exit(1);
        }

        if (0 > (child = fork())) exit(1);

        switch (child) {
            case 0:
                t_close(sock);

                printf("[%i] ready\n", (int) getpid());

                while (strncasecmp(recvMsg, "exit", 4) != 0) {
                    flags = 0;
                    if (0 > (nmb = t_rcv(sd, recvMsg, sizeof (recvMsg), &flags))) {
                        perror("t_recv");
                        exit(1);
                    }
                    if (nmb != 0) {
                        printf("[%i] got: %s\n", (int) getpid(), recvMsg);
                    }

                    char sendMsg[1024];
                    execute(recvMsg, sendMsg);

                    if (t_snd(sd, (void*) &sendMsg, sizeof (sendMsg), 0) < 0) {
                        perror("t_snd");
                        exit(1);
                    }
                    memset(sendMsg, 0, sizeof(sendMsg));
                    
                }
                t_close(sd);
                printf("[%i] gone offline\n", (int) getpid());
                exit(0);
                
            default:
                t_close(sd);
        }
    }

    return 0;
}
