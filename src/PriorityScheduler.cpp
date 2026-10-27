#include "PriorityScheduler.hpp"
#include <algorithm>
#include <climits>

void PriorityScheduler::simulate() {
    reset();
    std::vector<Process> procs = incomingProcesses;
    int n = static_cast<int>(procs.size());
    std::vector<int> timeInReady(n, 0);
    int completed = 0;
    currentTime = 0;

    int currentRunningIdx = -1;
    int segmentStart = 0;

    while (completed < n) {
        // Apply priority aging if enabled
        if (enableAging && agingInterval > 0) {
            for (int i = 0; i < n; ++i) {
                if (procs[i].arrivalTime <= currentTime && procs[i].remainingTime > 0 && i != currentRunningIdx) {
                    timeInReady[i]++;
                    if (timeInReady[i] >= agingInterval) {
                        if (procs[i].priority > 0) {
                            procs[i].priority--; // Boost priority
                        }
                        timeInReady[i] = 0;
                    }
                }
            }
        }

        // Select process with highest priority (lowest numerical value)
        int bestIdx = -1;
        int highestPriority = INT_MAX;

        for (int i = 0; i < n; ++i) {
            if (procs[i].arrivalTime <= currentTime && procs[i].remainingTime > 0) {
                if (procs[i].priority < highestPriority) {
                    highestPriority = procs[i].priority;
                    bestIdx = i;
                } else if (procs[i].priority == highestPriority) {
                    if (procs[i].arrivalTime < procs[bestIdx].arrivalTime) {
                        bestIdx = i;
                    }
                }
            }
        }

        if (bestIdx == -1) {
            if (currentRunningIdx != -1) {
                ganttChart.addSegment(procs[currentRunningIdx].pid, procs[currentRunningIdx].name, segmentStart, currentTime);
                currentRunningIdx = -1;
            }

            int nextArrival = INT_MAX;
            for (int i = 0; i < n; ++i) {
                if (procs[i].remainingTime > 0 && procs[i].arrivalTime > currentTime) {
                    nextArrival = std::min(nextArrival, procs[i].arrivalTime);
                }
            }

            ganttChart.addSegment(-1, "IDLE", currentTime, nextArrival);
            currentTime = nextArrival;
            continue;
        }

        if (!preemptive) {
            // Non-preemptive mode: run until completion
            Process& p = procs[bestIdx];
            p.startTime = currentTime;
            int finishTime = currentTime + p.burstTime;
            ganttChart.addSegment(p.pid, p.name, currentTime, finishTime);

            currentTime = finishTime;
            p.completionTime = currentTime;
            p.remainingTime = 0;
            p.state = ProcessState::TERMINATED;
            p.calculateMetrics();
            finishedProcesses.push_back(p);
            totalBusyTime += p.burstTime;
            completed++;
        } else {
            // Preemptive mode
            if (currentRunningIdx != bestIdx) {
                if (currentRunningIdx != -1) {
                    ganttChart.addSegment(procs[currentRunningIdx].pid, procs[currentRunningIdx].name, segmentStart, currentTime);
                }
                currentRunningIdx = bestIdx;
                segmentStart = currentTime;
            }

            if (procs[bestIdx].startTime == -1) {
                procs[bestIdx].startTime = currentTime;
            }

            // Step 1 unit of time (fine-grained for aging & preemption accuracy)
            procs[bestIdx].remainingTime -= 1;
            currentTime += 1;
            totalBusyTime += 1;

            if (procs[bestIdx].remainingTime == 0) {
                procs[bestIdx].completionTime = currentTime;
                procs[bestIdx].state = ProcessState::TERMINATED;
                procs[bestIdx].calculateMetrics();
                finishedProcesses.push_back(procs[bestIdx]);
                completed++;

                ganttChart.addSegment(procs[bestIdx].pid, procs[bestIdx].name, segmentStart, currentTime);
                currentRunningIdx = -1;
            }
        }
    }
}
