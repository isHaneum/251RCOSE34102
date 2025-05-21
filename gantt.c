#include <stdio.h>
#include <string.h>
#include "gantt.h"

#define MAX_GANTT_ENTRIES 1024

static GanttEntry gantt_log[MAX_GANTT_ENTRIES];
static int        gantt_idx = 0;

void log_execution(const char* id, int start, int end, bool is_io) {
    if (gantt_idx >= MAX_GANTT_ENTRIES) return;
    strncpy(gantt_log[gantt_idx].id, id, ID_LEN-1);
    gantt_log[gantt_idx].id[ID_LEN-1] = '\0';
    gantt_log[gantt_idx].start_time = start;
    gantt_log[gantt_idx].end_time   = end;
    gantt_log[gantt_idx].is_io      = is_io;
    gantt_idx++;
}

void print_gantt_chart(void) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < gantt_idx; i++) {
        int dur = gantt_log[i].end_time - gantt_log[i].start_time;
        if (gantt_log[i].is_io)
            printf(" |%s(%d,io)", gantt_log[i].id, dur);
        else
            printf("|  %s(%d)  ", gantt_log[i].id, dur);
    }
    printf("|\n");
    for (int i = 0; i < gantt_idx; i++) {
        if (gantt_log[i].is_io)
            printf("%-9d ", gantt_log[i].start_time);
        else
            printf("%-9d ", gantt_log[i].start_time);
    }
    if (gantt_idx > 0)
        printf("%d\n", gantt_log[gantt_idx - 1].end_time);
}

void clear_gantt_log(void) {
    gantt_idx = 0;
}