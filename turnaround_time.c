#include <stdio.h>
#include <stdlib.h>
#include "turnaround_time.h"

static void calculate_turnaround_time(Process* p) {
    p->turnaround_time = p->completed_time - p->arrival_time;  // return_time이 없다면 start->end 기준으로 계산
}

static void calculate_waiting_time(Process* p) {
    p->waiting_time = p->turnaround_time - p->burst_time - p->io_burst_count * p->io_burst_time; //
    if (p->waiting_time < 0) {
        p->waiting_time = 0; // 음수일 경우 0으로 설정
    } 
// io burst time을 제외한 waiting time
}
float average_turnaround_time(Process procs[], int n){
    float total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        calculate_turnaround_time(&procs[i]);
        total_turnaround_time += procs[i].turnaround_time;

    }
    printf("Average Turnaround Time: %.2f ms\n", total_turnaround_time / n);
    return (total_turnaround_time) / n;
}
float average_waiting_time(Process procs[], int n){
    float total_waiting_time = 0;
    
    for (int i = 0; i < n; i++) {
        calculate_waiting_time(&procs[i]);
        total_waiting_time += procs[i].waiting_time;
    }
    printf("Average Waiting Time: %.2f ms\n", total_waiting_time / n);
    return (total_waiting_time) / n;
} 


void print_turnaround_time(Process procs[], int n) {
    int total_turnaround_time = 0;

    printf("\n---Process Turnaround Time---\n");
    for (int i = 0; i < n; i++) {
        
        calculate_turnaround_time(&procs[i]);
        printf("%s: %dms\t", procs[i].id, procs[i].turnaround_time);
        total_turnaround_time += procs[i].turnaround_time;

    }
    printf("\nAverage Turnaround Time: %.2f ms\n", (float)total_turnaround_time / n);
}

void print_waiting_time(Process procs[], int n) {
    int total_waiting_time = 0;
    printf("\n---Process Waiting Time---\n");
    
    for (int i = 0; i < n; i++) {
        calculate_waiting_time(&procs[i]);
        printf("%s: %d ms\t", procs[i].id, procs[i].waiting_time);
        total_waiting_time += procs[i].waiting_time;  //
    }
    printf("\nAverage Waiting Time: %.2f ms\n", (float)total_waiting_time / n);
}