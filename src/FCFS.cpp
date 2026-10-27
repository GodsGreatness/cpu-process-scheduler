#include "FCFS.hpp"
#include <algorithm>

void FCFSScheduler::simulate() {
    reset();
    std::vector<Process> queue = incomingProcesses;

    std::sort(queue.begin(), queue.end(), [](const Process& a, const Process& b) {
        if (a.arrivalTime != b.arrivalTime) {
            return a.arrivalTime < b.arrivalTime;
        }
        return a.pid < b.pid;
    });

    currentTime = 0;

    for (auto& proc : queue) {
        if (currentTime < proc.arrivalTime) {
            ganttChart.addSegment(-1, "IDLE", currentTime, proc.arrivalTime);
            currentTime = proc.arrivalTime;
        }

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
