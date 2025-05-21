#ifndef GANTT_H
#define GANTT_H

#include <stdbool.h>
#include "process.h"  // for ID_LEN

typedef struct {
    char   id[ID_LEN];
    int    start_time;
    int    end_time;
    bool   is_io;
} GanttEntry;

void log_execution(const char* id, int start, int end, bool is_io);
void print_gantt_chart(void);
void clear_gantt_log(void);


#endif // GANTT_H