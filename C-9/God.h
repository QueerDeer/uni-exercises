
#ifndef GOD_H
#define GOD_H

#include <cstdlib>
#include <thread.h>
#include <stdio.h>

class God {
public:
    God();
    virtual ~God();

    void create();

};

static thread_t thr_a, thr_b, thr_c;

void *sub_a(void *);
void *sub_b(void *);
void *sub_c(void *);
void *sub_d(void *);
void *sub_e(void *);
void *sub_f(void *);

#endif /* GOD_H */
