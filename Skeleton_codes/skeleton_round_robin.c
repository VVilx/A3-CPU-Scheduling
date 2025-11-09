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
typedef struct {
    int items[QUEUE_MAX];   // Array to store process indices
    int front, rear;        // Front and rear pointers
} Queue;

/**
 * @brief Initialize the queue
 * Set front and rear to -1 to indicate empty queue
 */
static void init_queue(Queue *q) {
    // TODO: Initialize front and rear to -1
    
}

/**
 * @brief Check if queue is empty
 * @return true if queue is empty, false otherwise
 */
static bool is_empty(Queue *q) {
    // TODO: Return true if queue is empty
    return false;
}

/**
 * @brief Add an element to the rear of the queue
 * @param q Pointer to the queue
 * @param pid Process index to enqueue
 * 
 * HINT: Handle wrap-around using modulo operator
 * HINT: Check for queue full condition: (rear + 1) % QUEUE_MAX == front
 */
static void enqueue(Queue *q, int pid) {
    // TODO: Check if queue is full, if so return
    
    // TODO: If queue is empty, set front = rear = 0
    
    // TODO: Otherwise, advance rear with wrap-around: rear = (rear + 1) % QUEUE_MAX
    
    // TODO: Store the process index at rear position
    
}

/**
 * @brief Remove and return the front element from the queue
 * @param q Pointer to the queue
 * @return Process index, or -1 if queue is empty
 * 
 * HINT: If front == rear after removal, queue becomes empty (set front = rear = -1)
 */
static int dequeue(Queue *q) {
    // TODO: Check if queue is empty, return -1
    
    // TODO: Get the item at front
    
    // TODO: If front == rear, queue becomes empty (set both to -1)
    
    // TODO: Otherwise, advance front with wrap-around: front = (front + 1) % QUEUE_MAX
    
    // TODO: Return the dequeued item
    return -1;
}

/* ===============================================================================
 * ROUND ROBIN SCHEDULER
 * ===============================================================================
 */

/**
 * @brief Round Robin Scheduling Algorithm
 * 
 * @param ctx Pointer to the scheduler context containing all process information
 * @param time_quantum Time slice allocated to each process
 * 
 * This function implements the Round Robin scheduling algorithm with the given
 * time quantum. Processes are executed in a circular fashion, each getting
 * time_quantum units of CPU time before being preempted.
 */
void round_robin(SchedulerContext *ctx, int time_quantum)
{
    // Input validation
    if (time_quantum <= 0 || ctx->num_processes <= 0) return;

    // Step 1: Reset process states
    reset_process_states(ctx);

    int n = ctx->num_processes;
    int time = 0;                               // Current time
    int completed = 0;                          // Count of completed processes
    int remaining_time[MAX_PROCESSES];          // Track remaining time for each process
    bool is_completed[MAX_PROCESSES] = {false}; // Track completion status
    bool in_ready_queue[MAX_PROCESSES] = {false}; // Track if process is in queue

    // Step 2: Sort processes by arrival time, then by PID (tie-breaker)
    // TODO: Implement sorting logic similar to FCFS
    // HINT: Sort ctx->processes array directly (arrival_time → pid)
    

    // Step 3: Initialize remaining time array AFTER sorting
    // TODO: Copy burst_time to remaining_time for each process
    

    // Step 4: Initialize the ready queue
    Queue q;
    init_queue(&q);

    // Step 5: Enqueue all processes that arrive at time 0
    // TODO: Loop through all processes
    //       If arrival_time == 0, enqueue process index and mark in_ready_queue[i] = true
    

    // Step 6: Main scheduling loop
    while (completed < n) {
        // Case 1: Queue is empty - CPU is idle
        if (is_empty(&q)) {
            // TODO: Find the next arrival time among incomplete processes
            // HINT: Loop through all processes, find minimum arrival_time > current time
            
            
            // TODO: Jump time to next_arrival_time
            
            
            // TODO: Enqueue all processes arriving at this time
            // HINT: Check if arrival_time == time AND not completed AND not in queue
            
            
            continue;
        }

        // Case 2: Dequeue the front process
        // TODO: Dequeue a process index from the queue
        int idx = -1; // Replace -1 with dequeue(&q)
        
        // TODO: Mark this process as removed from queue
        

        Process *p = &ctx->processes[idx];

        // TODO: Calculate execution time for this process
        // HINT: Execute for minimum of (remaining_time, time_quantum)
        int exec_time = 0;
        
        // TODO: Store start time for checking new arrivals
        int start_time = time;
        
        // TODO: Update remaining_time and advance current time
        
        

        // TODO: Check if process completes
        // HINT: If remaining_time[idx] == 0, mark as completed and store completion_time
        

        // TODO: Enqueue any processes that arrived during execution
        // CRITICAL: This must happen BEFORE re-enqueuing the current process
        // HINT: Check if (arrival_time > start_time && arrival_time <= time)
        

        // TODO: Re-enqueue current process if not complete
        // HINT: Check if remaining_time[idx] > 0, then enqueue and mark in_ready_queue
        
    }

    // Step 7: Display results
    display_results(ctx, "Round-Robin (RR)");
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