
#include "God.h"

God::God() {
    create();
}

God::~God() {
}

void God::create() {
    thread_t main_thr;

    main_thr = thr_self();
    printf("Main thread = %u\n", main_thr);

    if (thr_create(NULL, 0, sub_b, NULL, THR_SUSPENDED | THR_NEW_LWP, &thr_b))
        fprintf(stderr, "Can't create thr_b\n"), exit(1);

    if (thr_create(NULL, 0, sub_a, (void *) thr_b, THR_NEW_LWP, &thr_a))
        fprintf(stderr, "Can't create thr_a\n"), exit(1);

    if (thr_create(NULL, 0, sub_c, (void *) main_thr, THR_NEW_LWP, &thr_c))
        fprintf(stderr, "Can't create thr_c\n"), exit(1);

    printf("Main Created threads A:%u B:%u C:%u\n", thr_a, thr_b, thr_c);
    printf("Main Thread exiting...\n");
    thr_exit((void *) main_thr);
}
