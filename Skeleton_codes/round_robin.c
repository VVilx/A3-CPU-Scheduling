/*
 * ===============================================================================
 * ROUND ROBIN (RR) SCHEDULING ALGORITHM - SKELETON
 * ===============================================================================
 *
 * ALGORITHM OVERVIEW:
 * -------------------
 * Round Robin is a preemptive scheduling algorithm that uses time-sharing.
 * Each process gets a fixed time quantum (time slice) to execute. If the process
 * doesn't complete within its quantum, it's preempted and moved to the back of
 * the ready queue.
 *
 * KEY CHARACTERISTICS:
 * - Preemptive: Processes can be interrupted after their time quantum expires
 * - Fair: All processes get equal CPU time in rotation
 * - Uses a circular queue (FIFO) to manage the ready queue
 * - Time quantum is a critical parameter (typically 1-4 time units)
 *
 * TIE-BREAKING RULE:
 * ------------------
 * When multiple processes arrive at the same time:
 * 1. Sort by Arrival Time (primary)
 * 2. If arrival times are equal, sort by Process ID (secondary)
 *
 * IMPORTANT FORMULAS (as per lecture slides):
 * -------------------------------------------
 * - Turnaround Time (TAT) = Completion Time - Arrival Time
 * - Waiting Time (WT) = Turnaround Time - Burst Time
 *
 * ALGORITHM STEPS:
 * ----------------
 * 1. Reset all process states
 * 2. Sort processes by arrival time (then by PID for tie-breaking)
 * 3. Initialize ready queue (circular queue) and tracking arrays
 * 4. Enqueue all processes that arrive at time 0
 * 5. Main scheduling loop (while not all processes completed):
 *    a. If queue is empty:
 *       - Find next arrival time
 *       - Jump to that time
 *       - Enqueue newly arrived processes
 *    b. Dequeue front process from ready queue
 *    c. Execute for min(remaining_time, time_quantum)
 *    d. Update remaining_time and current_time
 *    e. If process completes, record completion_time
 *    f. Enqueue any processes that arrived during execution
 *    g. If current process not complete, re-enqueue it (goes to back)
 * 6. Call display_results with algorithm name "RR"
 *
 * CRITICAL ORDERING RULE:
 * -----------------------
 * When a process completes its time quantum:
 * 1. FIRST enqueue any NEW processes that arrived during/after execution
 * 2. THEN re-enqueue the current process (if not complete)
 * This ensures newly arrived processes don't wait behind the preempted process
 *
 * IMPLEMENTATION HINTS:
 * ---------------------
 * - Implement a circular queue with enqueue/dequeue operations
 * - Use a boolean array to track which processes are already in the queue
 * - Track remaining_time for each process separately
 * - Handle CPU idle time when queue becomes empty
 * - Execute process time unit by time unit OR for entire quantum at once
 *
 * COMMON PITFALLS TO AVOID:
 * -------------------------
 * - Don't add the same process to queue multiple times
 * - Remember to check for new arrivals AFTER executing each time slice
 * - Re-enqueue preempted process AFTER adding newly arrived processes
 * - Don't forget to handle CPU idle periods
 * - Ensure processes are sorted before starting
 *
 * ===============================================================================
 */

#include "CPU_scheduler.h"

#define QUEUE_MAX 1000  // Maximum size for circular queue

/* ===============================================================================
 * CIRCULAR QUEUE IMPLEMENTATION
 * ===============================================================================
 * A circular queue is needed to manage the ready queue in Round Robin.
 * This provides FIFO (First-In-First-Out) behavior with efficient wrap-around.
 */
/*
 * ROUND ROBIN (RR) SCHEDULING ALGORITHM
 */

#include "CPU_scheduler.h"
#include <limits.h>
#include <stdbool.h>

#define QUEUE_MAX 1000  // Maximum size for circular queue

/* =========================================================================
 * CIRCULAR QUEUE
 * ========================================================================= */
typedef struct {
    int items[QUEUE_MAX];
    int front, rear;
} Queue;

static void init_queue(Queue *q) {
    q->front = -1;
    q->rear  = -1;
}

static bool is_empty(Queue *q) {
    return (q->front == -1);
}

static void enqueue(Queue *q, int pid) {
    if ((q->rear + 1) % QUEUE_MAX == q->front) {
        return;
    }

    if (is_empty(q)) {
        q->front = q->rear = 0;
        q->items[q->rear] = pid;
    } else {
        q->rear = (q->rear + 1) % QUEUE_MAX;
        q->items[q->rear] = pid;
    }
}

static int dequeue(Queue *q) {
    if (is_empty(q)) {
        return -1;
    }

    int item = q->items[q->front];

    if (q->front == q->rear) {
        q->front = q->rear = -1;
    } else {
        q->front = (q->front + 1) % QUEUE_MAX;
    }

    return item;
}

/* =========================================================================
 * ROUND ROBIN
 * ========================================================================= */
void round_robin(SchedulerContext *ctx, int time_quantum)
{
    if (time_quantum <= 0 || ctx->num_processes <= 0)
        return;
    reset_process_states(ctx);

    int n = ctx->num_processes;
    int time = 0;
    int completed = 0;

    int remaining_time[MAX_PROCESSES];
    bool is_completed[MAX_PROCESSES]   = {false};
    bool in_ready_queue[MAX_PROCESSES] = {false};
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            Process *a = &ctx->processes[j];
            Process *b = &ctx->processes[j + 1];
            bool swap = false;

            if (a->arrival_time > b->arrival_time) {
                swap = true;
            } else if (a->arrival_time == b->arrival_time &&
                       a->pid > b->pid) {
                swap = true;
            }

            if (swap) {
                Process tmp = *a;
                *a = *b;
                *b = tmp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        remaining_time[i] = ctx->processes[i].burst_time;
    }

    Queue q;
    init_queue(&q);

    for (int i = 0; i < n; i++) {
        if (ctx->processes[i].arrival_time == 0) {
            enqueue(&q, i);
            in_ready_queue[i] = true;
        }
    }

    while (completed < n) {

        if (is_empty(&q)) {
            int next_arrival_time = INT_MAX;

            for (int i = 0; i < n; i++) {
                if (!is_completed[i] &&
                    ctx->processes[i].arrival_time > time &&
                    ctx->processes[i].arrival_time < next_arrival_time) {
                    next_arrival_time = ctx->processes[i].arrival_time;
                }
            }

            if (next_arrival_time != INT_MAX) {
                time = next_arrival_time;
                for (int i = 0; i < n; i++) {
                    if (!is_completed[i] &&
                        ctx->processes[i].arrival_time == time &&
                        !in_ready_queue[i]) {
                        enqueue(&q, i);
                        in_ready_queue[i] = true;
                    }
                }
                continue;
            } else {
                break;
            }
        }

        int idx = dequeue(&q);
        in_ready_queue[idx] = false;

        Process *p = &ctx->processes[idx];
        int exec_time = (remaining_time[idx] < time_quantum)
                        ? remaining_time[idx]
                        : time_quantum;

        int start_time = time;
        time += exec_time;
        remaining_time[idx] -= exec_time;

        for (int i = 0; i < n; i++) {
            if (!is_completed[i] &&
                !in_ready_queue[i] &&
                ctx->processes[i].arrival_time > start_time &&
                ctx->processes[i].arrival_time <= time) {
                enqueue(&q, i);
                in_ready_queue[i] = true;
            }
        }

        if (remaining_time[idx] == 0) {
            is_completed[idx] = true;
            completed++;
            p->completion_time = time;
        } else {
            enqueue(&q, idx);
            in_ready_queue[idx] = true;
        }
    }
    display_results(ctx, "RR");
}


/* ========================================================================================*/
/*
 * TESTING YOUR IMPLEMENTATION:
 * ----------------------------
 * 1. Compile: make
 * 2. Run: ./scheduler < Testing/Testcases/input1.txt
 * 3. Compare output with Testing/Expected_output/output1.txt
 *  
 * DEBUGGING TIPS:
 * ---------------
 * - Print the ready queue contents at each step
 * - Trace execution timeline with timestamps
 * - Verify processes are enqueued/dequeued in correct order
 * - Check that newly arrived processes are added before preempted process
 * - Ensure remaining_time is updated correctly
 * - Verify completion_time matches expected timeline
 */