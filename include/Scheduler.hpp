#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "Process.hpp"
#include "GanttChart.hpp"
#include "Metrics.hpp"
#include <vector>
#include <string>

class Scheduler {
protected:
    std::vector<Process> incomingProcesses;
    std::vector<Process> finishedProcesses;
    GanttChart ganttChart;
    int currentTime;
    int totalBusyTime;

public:
    Scheduler() : currentTime(0), totalBusyTime(0) {}
    virtual ~Scheduler() = default;

    void addProcess(const Process& p) {
        incomingProcesses.push_back(p);
    }

    void loadProcesses(const std::vector<Process>& list) {
        incomingProcesses = list;
    }

    virtual void simulate() = 0;
    virtual std::string getName() const = 0;

    const GanttChart& getGanttChart() const { return ganttChart; }
    const std::vector<Process>& getFinishedProcesses() const { return finishedProcesses; }
    int getCurrentTime() const { return currentTime; }
    int getTotalBusyTime() const { return totalBusyTime; }

    SystemMetrics getMetrics() const {
        return MetricsCalculator::compute(finishedProcesses, currentTime);
    }

    void reset() {
        finishedProcesses.clear();
        ganttChart.clear();
        currentTime = 0;
        totalBusyTime = 0;
    }
};

#endif // SCHEDULER_HPP
