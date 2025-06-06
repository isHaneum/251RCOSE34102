
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

const char* alg_names[7] = {"NONE", "FCFS", "RR", "SJF", "PRIORITY", "PREEMPTIVE_SJF", "PREEMPTIVE_PRIORITY" };
#define MAX_PROCESSES 10
void set_schedule(Process procs[], int n){// scheduling후 필요 작업
    print_gantt_chart();//간트출력
    clear_gantt_log();//간트차트 초기화
    print_turnaround_time(procs, n);//turnaround 출력
    print_waiting_time(procs, n);//wating time 출력
    init_processes(procs, n);//프로세스 초기화


}
int main(int argc, char* argv[]) {
   srand((unsigned)time(NULL));
   int schedule_mode = 0;
   printf("Enter Process number(MAX %d): ", MAX_PROCESSES);// 프로세스 갯수 입력
   
   int num_processes;
   scanf("%d", &num_processes);
   Process procs[num_processes];

   create_processes(procs, num_processes);
   print_processes(procs, num_processes);

    //변수 설정
   int max_time     = 200;// max는 200ms정
   int time_quantum = rand() % 9 + 2; //  // 2~10 랜덤 시간

   while(1){// 스케줄링 모드 선택 algorithm switch
      printf("Scheduling Mode\n0. EXIT\t 1. FCFS\t 2. Round Robin\t 3. SJF\t 4. Priority\t 5. Preemptive SJF\t 6. Preemptive Priority\t 7. EVALUATION\t 8. MLQ\n:");
      scanf("%d", &schedule_mode);
      init_processes(procs, num_processes);//초기화

      if (schedule_mode == 0) {
         printf("TERMINATED\n");
         break;
        
      }

      
      switch (schedule_mode) {
         case 1://fcfs
            print_processes(procs, num_processes);

            schedule_fcfs(procs, num_processes, max_time);
            set_schedule(procs, num_processes);
            break;
          
         case 2://rr
            print_processes(procs, num_processes);

            schedule_rr(procs, num_processes, time_quantum, max_time);
            set_schedule(procs, num_processes);
            break;
          
         case 3://sjf
            print_processes(procs, num_processes);

            schedule_sjf(procs, num_processes, max_time);
            set_schedule(procs, num_processes);
            break;
          
         case 4://priority
            print_processes(procs, num_processes);

            schedule_priority(procs, num_processes, max_time);
            set_schedule(procs, num_processes);
            break;
         case 5://psjf
            print_processes(procs, num_processes);

            preemptive_sjf(procs, num_processes, max_time);
            set_schedule(procs, num_processes);
            break;
         case 6://pp
            print_processes(procs, num_processes);

            preemptive_priority(procs, num_processes, max_time);
            set_schedule(procs, num_processes);
            break;
         case 7:
         
            printf("\nEVALUATION\n");
            float best_wt = 100, best_tt = 100; 
            int best_wt_idx = 0, best_tt_idx = 0;
            init_processes(procs, num_processes);

            schedule_fcfs(procs, num_processes, max_time);
            if (best_tt > average_turnaround_time(procs, num_processes))  {
               best_tt = average_turnaround_time(procs, num_processes);
               best_tt_idx = 1;
            }
            if (best_wt > average_waiting_time(procs, num_processes)) {
               best_wt = average_waiting_time(procs, num_processes);
               best_wt_idx = 1;
            }
            init_processes(procs, num_processes);

            schedule_rr(procs, num_processes, time_quantum, max_time);
            if (best_tt > average_turnaround_time(procs, num_processes)) {
               best_tt = average_turnaround_time(procs, num_processes);
               best_tt_idx = 2;
            }
            if (best_wt > average_waiting_time(procs, num_processes)) {
               best_wt = average_waiting_time(procs, num_processes);
               best_wt_idx = 2;
            }

            init_processes(procs, num_processes);
            schedule_sjf(procs, num_processes, max_time);
            if (best_tt > average_turnaround_time(procs, num_processes)) {
               best_tt = average_turnaround_time(procs, num_processes);
               best_tt_idx = 3;
            }
            if (best_wt > average_waiting_time(procs, num_processes)) {
               best_wt = average_waiting_time(procs, num_processes);
               best_wt_idx = 3;
            }

            init_processes(procs, num_processes);
            schedule_priority(procs, num_processes, max_time);
            if (best_tt > average_turnaround_time(procs, num_processes)) {
               best_tt = average_turnaround_time(procs, num_processes);
               best_tt_idx = 4;
            }
            if (best_wt > average_waiting_time(procs, num_processes)) {
               best_wt = average_waiting_time(procs, num_processes);
               best_wt_idx = 4;
            }

            init_processes(procs, num_processes);
            preemptive_sjf(procs, num_processes, max_time);
            if (best_tt > average_turnaround_time(procs, num_processes)) {
               best_tt = average_turnaround_time(procs, num_processes);
               
               best_tt_idx = 5;
            }
            if (best_wt > average_waiting_time(procs, num_processes)) {
               best_wt = average_waiting_time(procs, num_processes);
               best_wt_idx = 5;
            }

            init_processes(procs, num_processes);
            preemptive_priority(procs, num_processes, max_time);
            if (best_tt > average_turnaround_time(procs, num_processes)) {
               best_tt = average_turnaround_time(procs, num_processes);
               best_tt_idx = 6;
            }
            if (best_wt > average_waiting_time(procs, num_processes)) {
               best_wt = average_waiting_time(procs, num_processes);
               best_wt_idx = 6;
            }
            printf("\nBest turnaround time: %2f ms --%s--\n", best_tt, alg_names[best_tt_idx]);
            printf("\nBest waiting time: %2f ms --%s--\n", best_wt, alg_names[best_wt_idx]);
            init_processes(procs, num_processes);
            clear_gantt_log();
            break;
         case 8:
            schedule_mlq(procs, num_processes, max_time);
            printf("\nMultilevel Queue Scheduling\n");
            clear_gantt_log();
            init_processes(procs, num_processes);
            break;
         default://잘못된 값
            printf("Re Enter\n");
            continue;
      }
   }
   return 0;

}
