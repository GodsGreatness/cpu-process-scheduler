#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <iostream>

enum class ProcessState {
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

struct Process {
    int pid;
    std::string name;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;          // Lower numerical value = higher priority (standard POSIX convention)
    
    // Performance Metrics
    int startTime;         // First time process was allocated CPU
    int completionTime;    // Time when execution finished
    int turnaroundTime;    // completionTime - arrivalTime
    int waitingTime;       // turnaroundTime - burstTime
    int responseTime;      // startTime - arrivalTime
    
    ProcessState state;

    Process(int id, std::string pName, int arrival, int burst, int prio = 0)
        : pid(id),
          name(std::move(pName)),
          arrivalTime(arrival),
          burstTime(burst),
          remainingTime(burst),
          priority(prio),
          startTime(-1),
          completionTime(-1),
          turnaroundTime(0),
          waitingTime(0),
          responseTime(-1),
          state(ProcessState::NEW) {}

    void calculateMetrics() {
        turnaroundTime = completionTime - arrivalTime;
        waitingTime = turnaroundTime - burstTime;
        responseTime = startTime - arrivalTime;
    }
};

#endif // PROCESS_HPP
