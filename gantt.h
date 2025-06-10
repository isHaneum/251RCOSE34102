#ifndef GANTT_H
#define GANTT_H

#include <stdbool.h>
#include "process.h"  // for ID_LEN

typedef struct {
    char   id[ID_LEN];
    int    start_time;
    int    end_time;
    bool   is_io;
} GanttEntry;//gantt chart를 위한 구조체

void log_store(const char* id, int start, int end, bool is_io);
void print_ganttchart(void);
void clear_ganttlog(void);


#endif // GANTT_H
