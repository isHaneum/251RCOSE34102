
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"
#include "ready_queue.h"
#include "waiting_queue.h"
#include "scheduler.h"
#include "gantt.h"
#include "turnaround_time.h"
#include <string.h>
#include "find_best.h"

const char* alg_names[4] = { "FCFS", "RR", "SJF", "PRIORITY" };

void set_schedule(Process procs[]){
    //print_processes(procs, MAX_PROCESSES);
    print_gantt_chart();
    clear_gantt_log();
    init_processes(procs, MAX_PROCESSES);
    print_turnaround_time(procs, MAX_PROCESSES);
    print_waiting_time(procs, MAX_PROCESSES);
}
int main(int argc, char* argv[]) {
    srand((unsigned)time(NULL));

    //프로세스 생성, 초기화
    Process procs[MAX_PROCESSES];
    create_processes(procs, MAX_PROCESSES);
    init_processes(procs, MAX_PROCESSES);
    print_processes(procs, MAX_PROCESSES);

    //변수 설정정
    int max_time     = 100;
    int time_quantum = 5;

    // 3) 스케줄링 모드 선택 및 실행
    // FCFS 모드
    schedule_fcfs(procs, MAX_PROCESSES, max_time);
    set_schedule(procs);
    // Round Robin 모드 사용 시

    
    schedule_rr(procs, MAX_PROCESSES, time_quantum, max_time);
    set_schedule(procs);

    //SJF 모드
    schedule_sjf(procs, MAX_PROCESSES, max_time);
    set_schedule(procs);

    // Priority 모드
    schedule_priority(procs, MAX_PROCESSES, max_time);
    set_schedule(procs);

    // 5) 메모리 해제
    //free(procs);

    /*int t_min = argmin(avg_turnaround, ALG_NUM);
    int t_max = argmax(avg_turnaround, ALG_NUM);
    int w_min = argmin(avg_waiting,   ALG_NUM);
    int w_max = argmax(avg_waiting,   ALG_NUM);

    printf("Turnaround time: min=%s(%.2f)  max=%s(%.2f)\n",
       alg_names[t_min], avg_turnaround[t_min],
       alg_names[t_max], avg_turnaround[t_max]);
    printf("Waiting   time: min=%s(%.2f)  max=%s(%.2f)\n",
       alg_names[w_min], avg_waiting[w_min],
       alg_names[w_max], avg_waiting[w_max]);    */
    return 0;
    }