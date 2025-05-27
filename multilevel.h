#ifndef MULTILEVEL_H
#define MULTILEVEL_H

#include "process.h"
#include "ready_queue.h"
#include "waiting_queue.h"
#include "gantt.h"

void schedule_mlq(Process procs[], int n, int max_time);
#endif  // MULTILEVEL_H