/*
 * ===============================================================================
 * PRIORITY PREEMPTIVE WITH ROUND ROBIN TIE-BREAKING - SKELETON
 * ===============================================================================
 * 
 * ALGORITHM OVERVIEW:
 * -------------------
 * This is a complex scheduling algorithm that combines priority-based preemptive
 * scheduling with Round Robin for tie-breaking. The process with the highest
 * priority (lowest priority number) executes. When multiple processes share the
 * same highest priority, Round Robin with time quantum is used among them.
 * 
 * KEY CHARACTERISTICS:
 * - Preemptive: Higher priority arrival causes IMMEDIATE preemption
 * - Priority-based: Lower priority number = Higher priority
 * - Round Robin for ties: Processes with same priority use RR scheduling
 * - Time quantum: Used only when multiple processes have same priority
 * 
 * PRIORITY CONVENTION:
 * --------------------
 * IMPORTANT: Lower priority number = Higher priority
 * Example: Priority 1 > Priority 2 > Priority 3
 * 
 * BEHAVIOR (as per lecture slides):
 * ---------------------------------
 * 1. Always execute the process(es) with highest priority (lowest number)
 * 2. If multiple processes have the same highest priority, use Round Robin among them
 * 3. When a higher priority process arrives, immediately preempt current process
 * 4. When preempted process's priority becomes highest again, it continues execution
 * 
 * TIE-BREAKING RULE:
 * ------------------
 * 1. Priority Value (primary) - select lowest priority number
 * 2. If same priority, use Round Robin with time quantum (secondary)
 * 3. Process ID for ordering within same priority level (tertiary)
 * 
 * IMPORTANT FORMULAS:
 * -------------------
 * - Completion Time = Time when process finishes execution
 * - Turnaround Time (TAT) = Completion Time - Arrival Time
 * - Waiting Time (WT) = Turnaround Time - Burst Time
 * 
 * ALGORITHM STEPS:
 * ----------------
 * 1. Reset all process states
 * 2. Initialize current_time = 0, completed = 0
 * 3. Main scheduling loop (while completed < num_processes):
 *    a. Find the highest priority (minimum priority number) among arrived processes
 *    b. Collect all processes with that highest priority into ready_processes array
 *    c. If no processes ready:
 *       - CPU is idle, jump to next arrival
 *    d. If processes ready:
 *       - Apply Round Robin among processes with same highest priority
 *       - For each process in the RR cycle:
 *         * Execute for min(time_quantum, remaining_time)
 *         * Check for higher priority arrivals during execution (preemption)
 *         * If process completes, record completion_time
 *         * If higher priority process arrives, break RR cycle and restart
 * 4. Call display_results with algorithm name "PRIORITY_PREEMPTIVE_RR"
 * 
 * PREEMPTION RULES:
 * -----------------
 * - Preemption occurs when a process with HIGHER priority (lower number) arrives
 * - Preemption is checked during execution (time unit by time unit)
 * - When preempted, the current process is suspended until its priority is highest again
 * 
 * IMPLEMENTATION HINTS:
 * ---------------------
 * - Execute time unit by time unit to check for preemptions
 * - Maintain a list of processes at the current highest priority level
 * - Use Round Robin rotation only among processes with same priority
 * - After executing one process, re-evaluate priorities (new arrivals may change highest)
 * - Use min_value() helper function for calculating execution time
 * 
 * COMMON PITFALLS TO AVOID:
 * -------------------------
 * - Don't forget priority convention (lower number = higher priority)
 * - Check for higher priority arrivals DURING execution, not just before
 * - Round Robin applies only among processes with SAME priority
 * - Re-evaluate priorities after each process execution
 * - Don't execute entire time quantum if higher priority process arrives
 * 
 * ===============================================================================
 */

#include "CPU_scheduler.h"

/* ========================================================================================*/
/**
 * @brief Priority Preemptive with Round Robin Tie-Breaking
 * 
 * @param ctx Pointer to the scheduler context containing all process information
 * @param time_quantum Time slice for Round Robin among same-priority processes
 * 
 * This function implements priority-based preemptive scheduling with Round Robin
 * used for tie-breaking when multiple processes have the same priority.
 */
void priority_preemptive_rr(SchedulerContext *ctx)
{
    
    int time_quantum = ctx->time_quantum;
    if (time_quantum <= 0) {
        time_quantum = 2;  // safe default
    }


    reset_process_states(ctx);

    int n = ctx->num_processes;
    int current_time = 0;
    int completed = 0;

    for (int i = 0; i < n; i++) {
        ctx->processes[i].remaining_time = ctx->processes[i].burst_time;
    }


    while (completed < n) {

        int highest_priority = INT_MAX;
        int ready_processes[MAX_PROCESSES];
        int ready_count = 0;


        for (int i = 0; i < n; i++) {
            Process *p = &ctx->processes[i];
            if (p->arrival_time <= current_time && p->remaining_time > 0) {
                if (p->priority < highest_priority) {
                    highest_priority = p->priority;
                    ready_count = 0;
                    ready_processes[ready_count++] = i;
                } else if (p->priority == highest_priority) {
                    ready_processes[ready_count++] = i;
                }
            }
        }

        if (ready_count == 0) {
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                Process *p = &ctx->processes[i];
                if (p->remaining_time > 0 &&
                    p->arrival_time > current_time &&
                    p->arrival_time < next_arrival) {
                    next_arrival = p->arrival_time;
                }
            }

            if (next_arrival == INT_MAX) {
                break;
            }

            current_time = next_arrival;
            continue;
        }

        for (int i = 0; i < ready_count - 1; i++) {
            for (int j = 0; j < ready_count - 1 - i; j++) {
                int idx_a = ready_processes[j];
                int idx_b = ready_processes[j + 1];
                if (ctx->processes[idx_a].pid > ctx->processes[idx_b].pid) {
                    int tmp = ready_processes[j];
                    ready_processes[j] = ready_processes[j + 1];
                    ready_processes[j + 1] = tmp;
                }
            }
        }

        
        bool higher_priority_arrived = false;

        for (int r = 0; r < ready_count && completed < n; r++) {
            int idx = ready_processes[r];
            Process *p = &ctx->processes[idx];

            if (p->remaining_time <= 0) {
                continue;
            }

            int run_for = (p->remaining_time < time_quantum)
                            ? p->remaining_time
                            : time_quantum;

            for (int t = 0; t < run_for; t++) {
        
                current_time++;
                p->remaining_time--;

                if (p->remaining_time == 0) {
                    p->completion_time = current_time;
                    completed++;
                }

                for (int j = 0; j < n; j++) {
                    Process *q = &ctx->processes[j];
                    if (q->arrival_time == current_time &&
                        q->remaining_time > 0 &&
                        q->priority < highest_priority) {
                        higher_priority_arrived = true;
                        break;
                    }
                }

                if (higher_priority_arrived) {
                    break;
                }

                if (p->remaining_time == 0) {
                    break;
                }
            }

            if (higher_priority_arrived) {
                break;  
            }
        }
    }

    display_results(ctx, "PRIORITY_PREEMPTIVE_WITH_RR");
}

/* ========================================================================================*/
/*
 * TESTING YOUR IMPLEMENTATION:
 * ----------------------------
 * 1. Compile: make
 * 2. Run: ./scheduler < Testing/Testcases/input1.txt
 * 3. Compare output with Testing/Expected_output/output1.txt 
 * 
 * Key Points:
 * - At time 1, P2 (priority=1) preempts P1 (priority=2)
 * - At time 2, P3 (priority=1) joins, P2 and P3 use RR
 * - P2 executes for quantum=3, then P3 gets its turn
 * - After P3 completes, P2 finishes its remaining 1 unit
 * - P1 resumes only after all priority=1 processes complete
 * 
 * PREEMPTION EXAMPLE:
 * -------------------
 * Time 0: P1 (priority=3) starts
 * Time 2: P2 (priority=1) arrives → IMMEDIATELY preempts P1
 * Time 5: P3 (priority=2) arrives → Does NOT preempt P2 (1 < 2)
 * Time 7: P2 completes → P3 executes (priority=2 > P1's priority=3)
 * Time 10: P3 completes → P1 resumes
 * 
 * ROUND ROBIN WITHIN SAME PRIORITY:
 * ----------------------------------
 * If at time 5, we have:
 *   P1: Priority=2, Remaining=4
 *   P2: Priority=2, Remaining=6
 *   P3: Priority=2, Remaining=3
 * 
 * With time_quantum=3:
 * - P1 executes for 3 (remaining=1)
 * - P2 executes for 3 (remaining=3)
 * - P3 executes for 3 (completes)
 * - P1 executes for 1 (completes)
 * - P2 executes for 3 (completes)
 * 
 * DEBUGGING TIPS:
 * ---------------
 * - Print which process executes at each time unit
 * - Print current highest priority at each scheduling decision
 * - Track which processes are in the ready list for RR
 * - Verify preemptions occur when higher priority process arrives
 * - Check that RR rotation happens only among same-priority processes
 * - Trace timeline to ensure it matches expected execution order
 * 
 * COMMON MISTAKES:
 * ----------------
 * - Not checking for preemption during execution (only before)
 * - Applying RR to all processes instead of only same-priority ones
 * - Not re-evaluating priorities after each process execution
 * - Forgetting priority convention (lower number = higher priority)
 * - Executing entire quantum even when higher priority arrives
 * - Not breaking out of RR loop when priorities change
 */