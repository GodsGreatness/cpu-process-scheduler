#include "SRTF.hpp"
#include <algorithm>
#include <climits>

void SRTFScheduler::simulate() {
    reset();
    std::vector<Process> procs = incomingProcesses;
    int completed = 0;
    int n = static_cast<int>(procs.size());
    currentTime = 0;

    int currentRunningIdx = -1;
    int segmentStart = 0;

    while (completed < n) {
        // Find process with shortest remaining time among arrived processes
        int shortestIdx = -1;
        int minRemaining = INT_MAX;

        for (int i = 0; i < n; ++i) {
            if (procs[i].arrivalTime <= currentTime && procs[i].remainingTime > 0) {
                if (procs[i].remainingTime < minRemaining) {
                    minRemaining = procs[i].remainingTime;
                    shortestIdx = i;
                } else if (procs[i].remainingTime == minRemaining) {
                    if (procs[i].arrivalTime < procs[shortestIdx].arrivalTime) {
                        shortestIdx = i;
                    }
                }
            }
        }

        if (shortestIdx == -1) {
            // CPU is IDLE
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

        // Context switch or start new segment if running process changes
        if (currentRunningIdx != shortestIdx) {
            if (currentRunningIdx != -1) {
                ganttChart.addSegment(procs[currentRunningIdx].pid, procs[currentRunningIdx].name, segmentStart, currentTime);
            }
            currentRunningIdx = shortestIdx;
            segmentStart = currentTime;
        }

        if (procs[shortestIdx].startTime == -1) {
            procs[shortestIdx].startTime = currentTime;
        }

        // Determine step size (until next process arrives or current completes)
        int step = procs[shortestIdx].remainingTime;
        for (int i = 0; i < n; ++i) {
            if (procs[i].remainingTime > 0 && procs[i].arrivalTime > currentTime) {
                int timeToArrival = procs[i].arrivalTime - currentTime;
                if (timeToArrival < step) {
                    step = timeToArrival;
                }
            }
        }

        procs[shortestIdx].remainingTime -= step;
        currentTime += step;
        totalBusyTime += step;

        if (procs[shortestIdx].remainingTime == 0) {
            procs[shortestIdx].completionTime = currentTime;
            procs[shortestIdx].state = ProcessState::TERMINATED;
            procs[shortestIdx].calculateMetrics();
            finishedProcesses.push_back(procs[shortestIdx]);
            completed++;

            ganttChart.addSegment(procs[shortestIdx].pid, procs[shortestIdx].name, segmentStart, currentTime);
            currentRunningIdx = -1;
        }
    }
}
