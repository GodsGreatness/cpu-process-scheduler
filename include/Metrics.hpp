#ifndef METRICS_HPP
#define METRICS_HPP

#include "Process.hpp"
#include <vector>
#include <iostream>

struct SystemMetrics {
    double averageTurnaroundTime;
    double averageWaitingTime;
    double averageResponseTime;
    double cpuUtilization;
    double throughput;
    int totalSimulationTime;
    int totalBusyTime;
    int totalProcesses;
};

class MetricsCalculator {
public:
    static SystemMetrics compute(const std::vector<Process>& processes, int totalSimulationTime);
    static void printTable(const std::vector<Process>& processes, const SystemMetrics& metrics, std::ostream& out = std::cout);
};

#endif // METRICS_HPP
