
#define AMOUNT 10

#include "routine.h"

void my_handler(int s) {
    printf("Caught signal %d\n", s);
    exit(1);
}

void * sub_routine(void *arg) {

    for (;;) {
        thread_t ret_thr;
        void *status;

        std::srand(time(NULL));

        int dice = std::rand() % 21 + 1;

        for (int i = 0; i < 10000000 * (int) thr_self(); ++i);

        int dice2 = std::rand() % (population + 1);

        switch (dice) {

            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                if (population < AMOUNT-1) {
                    population++;
                    if (!thr_create(NULL, 0, sub_routine, 0, THR_NEW_LWP,
                            &folks[population])) {
                        printf("Creature%d: Eject creature%d\n",
                                (int) thr_self() - 2, population);
                    }
                }

                break;

            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
                if (folks[dice2] != NULL) {
                    if (!thr_continue(folks[dice2])) {
                        printf("Creature%d: Kick creature%d\n",
                            (int) thr_self() - 2, dice2);
                    }
                }

                break;

            case 13:
            case 14:
            case 15:
                if ((thr_self() - 2)) {
                    thr_create(NULL,0,tickling,(void *) thr_self(), THR_NEW_LWP,
                            &ret_thr);
                    printf("Creature%d: Go to sleep\n", (int) thr_self() - 2);
                    thr_suspend(thr_self());
                }

                break;

            case 16:
            case 17:
            case 18:
                if (folks[dice2] != NULL && dice2 != thr_self() - 2) {
                    printf("Creature%d: try to eat creature%d\n",
                            (int) thr_self() - 2, dice2);

                    if (!thr_join(folks[dice2], (thread_t *) &ret_thr, &status))
                        printf("Creature%d was eaten by %d-th, yelping '%d!'\n",
                            (int) folks[dice2] - 2, (int) thr_self() - 2,
                            (int) status);
                    else printf ("but no way\n");
                }

                break;

            case 19:
            case 20:
            case 21:
                printf("Creature%d: Gone dead\n", (int) thr_self() - 2);
                thr_exit((void *) ((int) thr_self() - 2));

                break;
        }
    }
}

void * tickling(void *arg) {
    thread_t thr_sleeper = (thread_t) arg;
    
    for (long int i = 0; i < LONG_MAX; ++i);
    
    thr_continue(thr_sleeper);
    printf ("Creature%d were waked up by itself\n", (int) thr_sleeper - 2);
    thr_exit(NULL);
}
