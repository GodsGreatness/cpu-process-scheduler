#include "SJF.hpp"
#include <algorithm>

void SJFScheduler::simulate() {
    reset();
    std::vector<Process> pending = incomingProcesses;
    currentTime = 0;

    while (!pending.empty()) {
        std::vector<size_t> readyIndices;
        for (size_t i = 0; i < pending.size(); ++i) {
            if (pending[i].arrivalTime <= currentTime) {
                readyIndices.push_back(i);
            }
        }

        if (readyIndices.empty()) {
            // Find minimum arrival time of pending processes
            auto minArrivalIt = std::min_element(pending.begin(), pending.end(),
                [](const Process& a, const Process& b) {
                    return a.arrivalTime < b.arrivalTime;
                });
            ganttChart.addSegment(-1, "IDLE", currentTime, minArrivalIt->arrivalTime);
            currentTime = minArrivalIt->arrivalTime;
            continue;
        }

        // Pick shortest burst time
        size_t bestIdx = readyIndices[0];
        for (size_t idx : readyIndices) {
            if (pending[idx].burstTime < pending[bestIdx].burstTime ||
                (pending[idx].burstTime == pending[bestIdx].burstTime && pending[idx].arrivalTime < pending[bestIdx].arrivalTime)) {
                bestIdx = idx;
            }
        }

        Process proc = pending[bestIdx];
        pending.erase(pending.begin() + bestIdx);

        proc.startTime = currentTime;
        int finishTime = currentTime + proc.burstTime;
        ganttChart.addSegment(proc.pid, proc.name, currentTime, finishTime);

        currentTime = finishTime;
        proc.completionTime = currentTime;
        proc.remainingTime = 0;
        proc.state = ProcessState::TERMINATED;
        proc.calculateMetrics();

        totalBusyTime += proc.burstTime;
        finishedProcesses.push_back(proc);
    }
}
