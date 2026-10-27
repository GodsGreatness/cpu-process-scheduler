#include "RoundRobin.hpp"
#include <queue>
#include <algorithm>
#include <climits>

void RoundRobinScheduler::simulate() {
    reset();
    std::vector<Process> procs = incomingProcesses;
    int n = static_cast<int>(procs.size());
    std::vector<bool> inQueue(n, false);
    std::queue<int> readyQueue;
    currentTime = 0;
    int completed = 0;

    // Sort by arrival time initially
    std::vector<int> sortedIndices(n);
    for (int i = 0; i < n; ++i) sortedIndices[i] = i;
    std::sort(sortedIndices.begin(), sortedIndices.end(), [&](int a, int b) {
        if (procs[a].arrivalTime != procs[b].arrivalTime) {
            return procs[a].arrivalTime < procs[b].arrivalTime;
        }
        return procs[a].pid < procs[b].pid;
    });

    size_t arrivalIdx = 0;

    auto checkNewArrivals = [&](int untilTime) {
        while (arrivalIdx < sortedIndices.size()) {
            int pIdx = sortedIndices[arrivalIdx];
            if (procs[pIdx].arrivalTime <= untilTime) {
                if (!inQueue[pIdx] && procs[pIdx].remainingTime > 0) {
                    readyQueue.push(pIdx);
                    inQueue[pIdx] = true;
                }
                arrivalIdx++;
            } else {
                break;
            }
        }
    };

    checkNewArrivals(currentTime);

    while (completed < n) {
        if (readyQueue.empty()) {
            if (arrivalIdx < sortedIndices.size()) {
                int nextTime = procs[sortedIndices[arrivalIdx]].arrivalTime;
                ganttChart.addSegment(-1, "IDLE", currentTime, nextTime);
                currentTime = nextTime;
                checkNewArrivals(currentTime);
            }
            continue;
        }

        int currIdx = readyQueue.front();
        readyQueue.pop();

        if (procs[currIdx].startTime == -1) {
            procs[currIdx].startTime = currentTime;
        }

        int slice = std::min(timeQuantum, procs[currIdx].remainingTime);
        int startTime = currentTime;
        currentTime += slice;
        totalBusyTime += slice;
        procs[currIdx].remainingTime -= slice;

        ganttChart.addSegment(procs[currIdx].pid, procs[currIdx].name, startTime, currentTime);

        // Check for processes that arrived during this time slice BEFORE re-queuing current process
        checkNewArrivals(currentTime);

        if (procs[currIdx].remainingTime > 0) {
            readyQueue.push(currIdx);
        } else {
            procs[currIdx].completionTime = currentTime;
            procs[currIdx].state = ProcessState::TERMINATED;
            procs[currIdx].calculateMetrics();
            finishedProcesses.push_back(procs[currIdx]);
            completed++;
        }
    }
}
