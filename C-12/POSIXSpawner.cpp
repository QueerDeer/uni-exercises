#include "types.hpp"

extern char **environ;

static void dump_child_stdout(int filedes, char sendMsg[1024]) {
    ssize_t num_read;
    char buf[1];

    printf("child stdout:\n");
    for (int i = 0;; ++i) {
        num_read = read(filedes, buf, sizeof (buf));
        if (num_read > 0) {
            printf("%c", buf[0]);
            sendMsg[i] = buf[0];
        } else {
            break;
        }
    }
    printf("\n");
}

static void execute(char recvMsg[1024], char sendMsg[1024]) {
    int status;
    pid_t pid;
    int out[2];
    posix_spawn_file_actions_t action;

    int counter = 0;
    char *args[16];
    char * pch;
    pch = strtok (recvMsg, " \n");
    while (pch != NULL) {
        printf ("%s\n", pch);
        args[counter] = pch;
        counter++;
        pch = strtok (NULL, " \n");
    }
    args[counter] = NULL;

    posix_spawn_file_actions_init(&action);

    pipe(out);

    posix_spawn_file_actions_adddup2(&action, out[1], STDOUT_FILENO);
    posix_spawn_file_actions_addclose(&action, out[0]);

    status = posix_spawn(&pid, args[0], &action, NULL, args, environ);
    if (status == 0) {
        printf("child pid: %ld\n", pid);
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
        } else {
            if (WIFEXITED(status)) {
                printf("child exit status: %d\n", WEXITSTATUS(status));
            } else {
                printf("child died an unnatural death.\n");
            }

            close(out[1]);
            dump_child_stdout(out[0], sendMsg);
        }
    } else {
        fprintf(stderr, "posix_spawn: %s\n", strerror(status));
        close(out[1]);
    }

    posix_spawn_file_actions_destroy(&action);
}
