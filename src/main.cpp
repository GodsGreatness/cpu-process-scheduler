#include "FCFS.hpp"
#include "SJF.hpp"
#include "SRTF.hpp"
#include "RoundRobin.hpp"
#include "PriorityScheduler.hpp"
#include "Metrics.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <iomanip>

std::vector<Process> loadWorkloadFromFile(const std::string& filepath) {
    std::vector<Process> processes;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[Error] Could not open workload file: " << filepath << "\n";
        return processes;
    }

    std::string line;
    // Format: PID Name ArrivalTime BurstTime [Priority]
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        int pid;
        std::string name;
        int arrival, burst, prio = 0;

        if (ss >> pid >> name >> arrival >> burst) {
            if (!(ss >> prio)) {
                prio = 0;
            }
            processes.emplace_back(pid, name, arrival, burst, prio);
        }
    }
    return processes;
}

std::vector<Process> getDefaultWorkload() {
    return {
        Process(1, "P1", 0, 8, 3),
        Process(2, "P2", 1, 4, 1),
        Process(3, "P3", 2, 9, 4),
        Process(4, "P4", 3, 5, 2)
    };
}

void printHelp() {
    std::cout << "CPU Process Scheduling Simulator\n";
    std::cout << "Usage: ./cpu_scheduler [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --file <path>        Load workload from file (format: PID Name Arrival Burst [Priority])\n";
    std::cout << "  --algorithm <algo>   Select algorithm: fcfs, sjf, srtf, rr, priority (default: all via --compare)\n";
    std::cout << "  --quantum <int>      Time quantum for Round Robin (default: 2)\n";
    std::cout << "  --preemptive         Enable preemption for priority scheduling\n";
    std::cout << "  --aging              Enable dynamic priority aging\n";
    std::cout << "  --compare            Run all algorithms and output comparison leaderboard\n";
    std::cout << "  --help               Display this help message\n\n";
}

void runSingle(Scheduler& scheduler, const std::vector<Process>& workload) {
    std::cout << "\n=======================================================\n";
    std::cout << " Algorithm: " << scheduler.getName() << "\n";
    std::cout << "=======================================================\n";

    scheduler.loadProcesses(workload);
    scheduler.simulate();

    scheduler.getGanttChart().printChart(std::cout);
    MetricsCalculator::printTable(scheduler.getFinishedProcesses(), scheduler.getMetrics(), std::cout);
}

void runComparison(const std::vector<Process>& workload, int quantum) {
    std::cout << "\n=======================================================\n";
    std::cout << " Running Multi-Algorithm Comparative Benchmark\n";
    std::cout << " Total Processes: " << workload.size() << "\n";
    std::cout << "=======================================================\n";

    std::vector<std::unique_ptr<Scheduler>> schedulers;
    schedulers.push_back(std::make_unique<FCFSScheduler>());
    schedulers.push_back(std::make_unique<SJFScheduler>());
    schedulers.push_back(std::make_unique<SRTFScheduler>());
    schedulers.push_back(std::make_unique<RoundRobinScheduler>(quantum));
    schedulers.push_back(std::make_unique<PriorityScheduler>(false, false));
    schedulers.push_back(std::make_unique<PriorityScheduler>(true, true, 4));

    struct Result {
        std::string name;
        SystemMetrics metrics;
    };

    std::vector<Result> results;

    for (auto& s : schedulers) {
        s->loadProcesses(workload);
        s->simulate();
        results.push_back({s->getName(), s->getMetrics()});
    }

    std::cout << "\n+-----------------------------------------------------+---------------+---------------+---------------+-------------+\n";
    std::cout << "| Algorithm                                           | Avg Turnaround| Avg Waiting   | Avg Response  | CPU Util %  |\n";
    std::cout << "+-----------------------------------------------------+---------------+---------------+---------------+-------------+\n";

    for (const auto& r : results) {
        std::cout << "| " << std::left << std::setw(51) << r.name.substr(0, 51) << " | "
                  << std::right << std::fixed << std::setprecision(2)
                  << std::setw(13) << r.metrics.averageTurnaroundTime << " | "
                  << std::setw(13) << r.metrics.averageWaitingTime << " | "
                  << std::setw(13) << r.metrics.averageResponseTime << " | "
                  << std::setw(10) << r.metrics.cpuUtilization << "% |\n";
    }
    std::cout << "+-----------------------------------------------------+---------------+---------------+---------------+-------------+\n\n";
}

int main(int argc, char* argv[]) {
    std::string filePath = "";
    std::string algo = "compare";
    int quantum = 2;
    bool isPreemptive = false;
    bool isAging = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filePath = argv[++i];
        } else if (arg == "--algorithm" && i + 1 < argc) {
            algo = argv[++i];
        } else if (arg == "--quantum" && i + 1 < argc) {
            quantum = std::stoi(argv[++i]);
        } else if (arg == "--preemptive") {
            isPreemptive = true;
        } else if (arg == "--aging") {
            isAging = true;
        } else if (arg == "--compare") {
            algo = "compare";
        } else if (arg == "--help") {
            printHelp();
            return 0;
        }
    }

    std::vector<Process> workload;
    if (!filePath.empty()) {
        workload = loadWorkloadFromFile(filePath);
        if (workload.empty()) {
            std::cerr << "[Warning] Workload file empty or missing. Falling back to default workload.\n";
            workload = getDefaultWorkload();
        }
    } else {
        workload = getDefaultWorkload();
    }

    if (algo == "fcfs") {
        FCFSScheduler s;
        runSingle(s, workload);
    } else if (algo == "sjf") {
        SJFScheduler s;
        runSingle(s, workload);
    } else if (algo == "srtf") {
        SRTFScheduler s;
        runSingle(s, workload);
    } else if (algo == "rr") {
        RoundRobinScheduler s(quantum);
        runSingle(s, workload);
    } else if (algo == "priority") {
        PriorityScheduler s(isPreemptive, isAging);
        runSingle(s, workload);
    } else {
        runComparison(workload, quantum);
    }

    return 0;
}
