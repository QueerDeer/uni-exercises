
#ifndef ROUTINE_H
#define ROUTINE_H

#define AMOUNT 10

#include <cstdlib>
#include <stdio.h>
#include <thread.h>
#include <signal.h>
#include <limits.h>

void my_handler(int s);

void *sub_routine(void *);
void *tickling(void *);

static int population = 0;
static thread_t folks[AMOUNT];

#endif /* ROUTINE_H */

