# CPU Scheduling Algorithms & Theory Reference

This document details the scheduling policies, mathematical formulations, and trade-offs implemented in this simulator.

---

## 1. Process Metrics & Mathematical Formulations

Given a set of processes $P = \{P_1, P_2, \dots, P_n\}$, each process $P_i$ is characterized by:
- $AT_i$: Arrival Time
- $BT_i$: Burst Time (CPU execution requirement)
- $ST_i$: Start Time (first moment CPU is allocated to $P_i$)
- $CT_i$: Completion Time (instant execution finishes)

### Key Formulations:
1. **Turnaround Time ($TAT$):**
   $$TAT_i = CT_i - AT_i$$
   The total elapsed time from job submission to completion.

2. **Waiting Time ($WT$):**
   $$WT_i = TAT_i - BT_i$$
   The total amount of time a process spends waiting in the ready queue.

3. **Response Time ($RT$):**
   $$RT_i = ST_i - AT_i$$
   The time elapsed from submission until the first CPU burst begins. Critical metric in interactive and time-sharing systems.

4. **CPU Utilization:**
   $$\text{Utilization} = \left(\frac{\sum_{i=1}^n BT_i}{\text{Total Simulation Time}}\right) \times 100\%$$

5. **Throughput:**
   $$\text{Throughput} = \frac{n}{\text{Total Simulation Time}}$$

---

## 2. Algorithms Implemented

### A. First-Come, First-Served (FCFS)
- **Policy:** Non-preemptive FIFO queue.
- **Advantage:** Minimal scheduling overhead, simple implementation.
- **Disadvantage:** Susceptible to the **Convoy Effect**, where short jobs stall behind a long CPU-bound process, spiking average waiting time.

### B. Shortest Job First (SJF)
- **Policy:** Non-preemptive. The ready process with the smallest initial burst time is dispatched.
- **Advantage:** Mathematically optimal in minimizing average waiting time for batch systems when all jobs arrive simultaneously.
- **Disadvantage:** Cannot preempt longer processes if a shorter one arrives later; requires advance knowledge of CPU burst lengths.

### C. Shortest Remaining Time First (SRTF)
- **Policy:** Preemptive SJF. If a newly arrived process requires less remaining CPU time than the currently executing process, the CPU is preempted.
- **Advantage:** Yields the lowest average turnaround and waiting times in dynamic arrival scenarios.
- **Disadvantage:** Context-switch overhead and potential starvation for long-running processes under continuous high arrival rates.

### D. Round Robin (RR)
- **Policy:** Preemptive time-sliced scheduling. Each process is allocated a fixed slice of CPU time ($\text{Quantum } q$). When the slice expires, the process is preempted and returned to the tail of the ready queue.
- **Advantage:** Starvation-free, predictable latency, and optimal average response time for interactive/multi-user systems.
- **Trade-off:**
  - $q \to \infty$: Degenerates into FCFS.
  - $q \to 0$: Excessive context switches dominate CPU cycles.

### E. Priority Scheduling (with Starvation Prevention / Aging)
- **Policy:** Each process is assigned an integer priority (lower value = higher priority).
- **Modes:** Both Non-Preemptive and Preemptive modes are supported.
- **Starvation Mitigation (Aging):** In static priority scheduling, low-priority processes can suffer indefinite postponement (starvation). Our engine incorporates an **Aging Mechanism**: after waiting $k$ time units in the ready queue without CPU allocation, a process's numerical priority is automatically boosted, guaranteeing eventual execution.
