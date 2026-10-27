#include "Metrics.hpp"
#include <iomanip>
#include <algorithm>

SystemMetrics MetricsCalculator::compute(const std::vector<Process>& processes, int totalSimulationTime) {
    SystemMetrics metrics{};
    metrics.totalProcesses = static_cast<int>(processes.size());
    metrics.totalSimulationTime = totalSimulationTime;

    if (processes.empty() || totalSimulationTime <= 0) {
        return metrics;
    }

    double totalTurnaround = 0;
    double totalWaiting = 0;
    double totalResponse = 0;
    int totalBurst = 0;

    for (const auto& p : processes) {
        totalTurnaround += p.turnaroundTime;
        totalWaiting += p.waitingTime;
        totalResponse += p.responseTime;
        totalBurst += p.burstTime;
    }

    metrics.averageTurnaroundTime = totalTurnaround / processes.size();
    metrics.averageWaitingTime = totalWaiting / processes.size();
    metrics.averageResponseTime = totalResponse / processes.size();
    metrics.totalBusyTime = totalBurst;
    metrics.cpuUtilization = (static_cast<double>(totalBurst) / totalSimulationTime) * 100.0;
    metrics.throughput = static_cast<double>(processes.size()) / totalSimulationTime;

    return metrics;
}

void MetricsCalculator::printTable(const std::vector<Process>& processes, const SystemMetrics& metrics, std::ostream& out) {
    std::vector<Process> sorted = processes;
    std::sort(sorted.begin(), sorted.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });

    out << "\n+-----+----------+--------------+------------+----------+-----------------+-----------------+--------------+---------------+\n";
    out << "| PID | Process  | Arrival Time | Burst Time | Priority | Completion Time | Turnaround Time | Waiting Time | Response Time |\n";
    out << "+-----+----------+--------------+------------+----------+-----------------+-----------------+--------------+---------------+\n";

    for (const auto& p : sorted) {
        out << "| " << std::setw(3) << p.pid << " | "
            << std::setw(8) << p.name << " | "
            << std::setw(12) << p.arrivalTime << " | "
            << std::setw(10) << p.burstTime << " | "
            << std::setw(8) << p.priority << " | "
            << std::setw(15) << p.completionTime << " | "
            << std::setw(15) << p.turnaroundTime << " | "
            << std::setw(12) << p.waitingTime << " | "
            << std::setw(13) << p.responseTime << " |\n";
    }
    out << "+-----+----------+--------------+------------+----------+-----------------+-----------------+--------------+---------------+\n";

    out << std::fixed << std::setprecision(2);
    out << "\n=== System Performance Summary ===\n";
    out << "  - Total Simulation Time : " << metrics.totalSimulationTime << " units\n";
    out << "  - CPU Busy Time         : " << metrics.totalBusyTime << " units\n";
    out << "  - CPU Utilization       : " << metrics.cpuUtilization << " %\n";
    out << "  - Throughput            : " << metrics.throughput << " processes/unit time\n";
    out << "  - Average Turnaround    : " << metrics.averageTurnaroundTime << " units\n";
    out << "  - Average Waiting Time  : " << metrics.averageWaitingTime << " units\n";
    out << "  - Average Response Time : " << metrics.averageResponseTime << " units\n\n";
}
