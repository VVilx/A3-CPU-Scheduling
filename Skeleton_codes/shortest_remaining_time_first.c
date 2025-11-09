/*
 * ===============================================================================
 * SHORTEST REMAINING TIME (SRT) - PREEMPTIVE SCHEDULING ALGORITHM - SKELETON
 * ===============================================================================
 * 
 * ALGORITHM OVERVIEW:
 * -------------------
 * Shortest Remaining Time (SRT), also known as Shortest Remaining Time First (SRTF)
 * or Preemptive SJF, is a preemptive version of the Shortest Job First algorithm.
 * At each time unit, the process with the shortest remaining time is selected.
 * If a new process arrives with shorter remaining time than the current process,
 * the current process is preempted.
 * 
 * KEY CHARACTERISTICS:
 * - Preemptive: Processes can be interrupted when a shorter job arrives
 * - Optimal for minimizing average waiting time
 * - Can cause starvation for longer processes
 * - Executes in time units (check for preemption at each unit)
 * - More responsive than non-preemptive SJF
 * 
 * TIE-BREAKING RULE:
 * ------------------
 * When multiple processes have the same remaining time:
 * 1. Remaining Time (primary) - select shortest
 * 2. If remaining times are equal, Arrival Time (secondary) - select earliest arrival
 * 3. If both are equal, Process ID (tertiary) - select smallest PID
 * 
 * IMPORTANT FORMULAS (as per lecture slides):
 * -------------------------------------------
 * - Completion Time = Time when process finishes execution (remaining_time becomes 0)
 * - Turnaround Time (TAT) = Completion Time - Arrival Time
 * - Waiting Time (WT) = Turnaround Time - Burst Time
 * 
 * ALGORITHM STEPS:
 * ----------------
 * 1. Reset all process states (this initializes remaining_time = burst_time)
 * 2. Initialize current_time = 0, completed = 0
 * 3. Main scheduling loop (while completed < num_processes):
 *    a. Find process with shortest remaining time among arrived processes
 *       - Apply tie-breaking: remaining_time → arrival_time → pid
 *    b. If no process is ready:
 *       - CPU is idle
 *       - Jump to next arrival time
 *    c. If process found:
 *       - Execute for 1 time unit (preemptive scheduling)
 *       - Decrement remaining_time
 *       - Increment current_time
 *       - If remaining_time becomes 0:
 *         * Record completion_time
 *         * Increment completed counter
 * 4. Call display_results with algorithm name "SRT"
 * 
 * KEY DIFFERENCE FROM SJF:
 * ------------------------
 * - SJF: Non-preemptive, execute entire burst_time at once
 * - SRT: Preemptive, execute 1 time unit, then re-evaluate
 * - SRT allows newly arrived shorter processes to preempt current process
 * 
 * IMPLEMENTATION HINTS:
 * ---------------------
 * - Execute processes one time unit at a time (not entire burst)
 * - Use remaining_time (not burst_time) for comparison
 * - At each time unit, check all arrived processes for shortest remaining time
 * - Update remaining_time after each time unit of execution
 * - Record completion_time only when remaining_time becomes 0
 * 
 * COMMON PITFALLS TO AVOID:
 * -------------------------
 * - Don't execute entire remaining_time at once (must be 1 unit at a time)
 * - Don't forget to check remaining_time (not burst_time) for selection
 * - Apply all three tie-breaking criteria correctly
 * - Don't calculate waiting_time directly
 * - Handle CPU idle periods when no process has arrived
 * 
 * ===============================================================================
 */

#include "CPU_scheduler.h"

/* ========================================================================================*/
/**
 * @brief Shortest Remaining Time (SRT) - Preemptive Scheduling Algorithm
 * 
 * @param ctx Pointer to the scheduler context containing all process information
 * 
 * This function implements the SRT scheduling algorithm. At each time unit,
 * it selects the process with the shortest remaining execution time among
 * all arrived processes. Processes can be preempted when a shorter job arrives.
 */
#include <limits.h>
#include <stdbool.h>

void shortest_remaining_time_first(SchedulerContext *ctx)
{
    reset_process_states(ctx);

    int n = ctx->num_processes;
    int current_time = 0;
    int completed = 0;

    for (int i = 0; i < n; i++) {
        ctx->processes[i].remaining_time = ctx->processes[i].burst_time;
    }


    while (completed < n) {
        int shortest = -1;
        int min_remaining_time = INT_MAX;

        for (int i = 0; i < n; i++) {
            Process *p = &ctx->processes[i];

            if (p->arrival_time <= current_time && p->remaining_time > 0) {
                if (p->remaining_time < min_remaining_time) {
                    min_remaining_time = p->remaining_time;
                    shortest = i;
                } else if (p->remaining_time == min_remaining_time) {
                    Process *best = &ctx->processes[shortest];
                    if (p->arrival_time < best->arrival_time) {
                        shortest = i;
                    } else if (p->arrival_time == best->arrival_time) {
                        if (p->pid < best->pid) {
                            shortest = i;
                        }
                    }
                }
            }
        }
        if (shortest == -1) {
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (ctx->processes[i].remaining_time > 0 &&
                    ctx->processes[i].arrival_time > current_time &&
                    ctx->processes[i].arrival_time < next_arrival) {
                    next_arrival = ctx->processes[i].arrival_time;
                }
            }

            if (next_arrival != INT_MAX) {
                current_time = next_arrival;
                continue;
            } else {
                break;
            }
        }

        ctx->processes[shortest].remaining_time--;
        current_time++;

        if (ctx->processes[shortest].remaining_time == 0) {
            ctx->processes[shortest].completion_time = current_time;
            completed++;
        }
    }

    display_results(ctx, "Shortest-Remaining_Time-First (SRTF)");
}


/* ========================================================================================*/
/*
 * TESTING YOUR IMPLEMENTATION:
 * ----------------------------
 * 1. Compile: make
 * 2. Run: ./scheduler < Testing/Testcases/input1.txt
 * 3. Compare output with Testing/Expected_output/output1.txt
 *  
 * PREEMPTION EXAMPLE:
 * -------------------
 * At time 0: P1 starts (only process)
 * At time 1: P2 arrives with burst=4, P1 has remaining=7
 *            → P2 preempts P1 (4 < 7)
 * At time 2: P3 arrives with burst=9, P2 has remaining=3
 *            → P2 continues (3 < 7 < 9)
 * At time 3: P4 arrives with burst=5, P2 has remaining=2
 *            → P2 continues (2 < 5 < 7 < 9)
 * At time 5: P2 completes, P4 has remaining=5, P1 has remaining=7, P3 has remaining=9
 *            → P4 executes next (5 < 7 < 9)
 * 
 * TIE-BREAKING EXAMPLE:
 * ---------------------
 * If at time 5, two processes have the same remaining time:
 *   P1: Arrival=0, Remaining=3
 *   P2: Arrival=2, Remaining=3
 *   
 * Select P1 (same remaining time, but P1 arrived earlier)
 * 
 * DEBUGGING TIPS:
 * ---------------
 * - Print which process executes at each time unit
 * - Track remaining_time for each process at every step
 * - Verify preemptions occur when shorter process arrives
 * - Check that processes are compared by remaining_time (not burst_time)
 * - Trace the timeline to ensure it matches expected execution order
 * - Verify completion_time is recorded only when remaining_time = 0
 * 
 * COMMON MISTAKES:
 * ----------------
 * - Executing more than 1 time unit (must execute exactly 1 unit for preemption)
 * - Using burst_time instead of remaining_time for comparison
 * - Not applying tie-breaking rules in correct order
 * - Recording completion_time before remaining_time becomes 0
 * - Forgetting to handle CPU idle time
 */