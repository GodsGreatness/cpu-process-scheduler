#include "FCFS.hpp"
#include "SJF.hpp"
#include "SRTF.hpp"
#include "RoundRobin.hpp"
#include "PriorityScheduler.hpp"
#include <iostream>
#include <cassert>
#include <vector>

void testFCFS() {
    std::cout << "[Test] Running FCFS Test... ";
    FCFSScheduler s;
    s.addProcess(Process(1, "P1", 0, 5));
    s.addProcess(Process(2, "P2", 2, 3));
    s.addProcess(Process(3, "P3", 4, 1));
    s.simulate();

    auto procs = s.getFinishedProcesses();
    assert(procs.size() == 3);
    
    // P1: arrives 0, starts 0, finishes 5 (WT = 0, TAT = 5)
    // P2: arrives 2, starts 5, finishes 8 (WT = 3, TAT = 6)
    // P3: arrives 4, starts 8, finishes 9 (WT = 4, TAT = 5)
    assert(procs[0].completionTime == 5);
    assert(procs[1].completionTime == 8);
    assert(procs[2].completionTime == 9);
    assert(s.getCurrentTime() == 9);
    std::cout << "PASSED\n";
}

void testSJF() {
    std::cout << "[Test] Running SJF Test... ";
    SJFScheduler s;
    s.addProcess(Process(1, "P1", 0, 7));
    s.addProcess(Process(2, "P2", 2, 4));
    s.addProcess(Process(3, "P3", 4, 1));
    s.addProcess(Process(4, "P4", 5, 4));
    s.simulate();

    auto procs = s.getFinishedProcesses();
    assert(procs.size() == 4);

    // At t=7 (when P1 finishes), P2 (rem=4), P3 (rem=1), P4 (rem=4) have arrived.
    // Shortest is P3 (len 1) -> finishes at 8.
    // Next is P2 (arrived 2, burst 4) vs P4 (arrived 5, burst 4) -> P2 runs first -> finishes at 12.
    // Next is P4 -> finishes at 16.
    auto findProc = [&](int pid) -> Process {
        for (const auto& p : procs) if (p.pid == pid) return p;
        throw std::runtime_error("Process not found");
    };

    assert(findProc(1).completionTime == 7);
    assert(findProc(3).completionTime == 8);
    assert(findProc(2).completionTime == 12);
    assert(findProc(4).completionTime == 16);
    std::cout << "PASSED\n";
}

void testSRTFPreemption() {
    std::cout << "[Test] Running SRTF Preemption Test... ";
    SRTFScheduler s;
    s.addProcess(Process(1, "P1", 0, 8));
    s.addProcess(Process(2, "P2", 1, 4));
    s.addProcess(Process(3, "P3", 2, 9));
    s.addProcess(Process(4, "P4", 3, 5));
    s.simulate();

    auto procs = s.getFinishedProcesses();
    assert(procs.size() == 4);

    auto findProc = [&](int pid) -> Process {
        for (const auto& p : procs) if (p.pid == pid) return p;
        throw std::runtime_error("Process not found");
    };

    // P1 starts at 0. At t=1, P2 arrives with burst 4 < P1's remaining 7. P2 preempts P1.
    // P2 runs to completion at t=5.
    assert(findProc(2).completionTime == 5);
    assert(findProc(2).startTime == 1);
    assert(findProc(2).responseTime == 0); // 1 - 1 = 0
    std::cout << "PASSED\n";
}

void testRoundRobin() {
    std::cout << "[Test] Running Round Robin Test... ";
    RoundRobinScheduler s(2); // Quantum = 2
    s.addProcess(Process(1, "P1", 0, 5));
    s.addProcess(Process(2, "P2", 0, 3));
    s.addProcess(Process(3, "P3", 0, 1));
    s.simulate();

    // t=0: P1(5), P2(3), P3(1)
    // [0-2]: P1 runs (rem 3)
    // [2-4]: P2 runs (rem 1)
    // [4-5]: P3 runs (finishes at 5)
    // [5-7]: P1 runs (rem 1)
    // [7-8]: P2 runs (finishes at 8)
    // [8-9]: P1 runs (finishes at 9)
    auto procs = s.getFinishedProcesses();
    auto findProc = [&](int pid) -> Process {
        for (const auto& p : procs) if (p.pid == pid) return p;
        throw std::runtime_error("Process not found");
    };

    assert(findProc(3).completionTime == 5);
    assert(findProc(2).completionTime == 8);
    assert(findProc(1).completionTime == 9);
    assert(s.getCurrentTime() == 9);
    std::cout << "PASSED\n";
}

void testPriorityAging() {
    std::cout << "[Test] Running Priority with Aging Test... ";
    // Low priority process arrived early should not starve when high-priority jobs run
    PriorityScheduler s(true, true, 2); // Preemptive with aging interval 2
    s.addProcess(Process(1, "LowPrio", 0, 10, 5));
    s.addProcess(Process(2, "HighPrio", 1, 4, 1));
    s.simulate();

    auto procs = s.getFinishedProcesses();
    assert(procs.size() == 2);
    assert(s.getCurrentTime() == 14);
    std::cout << "PASSED\n";
}

void testIdleCPU() {
    std::cout << "[Test] Running CPU Idle Gap Test... ";
    FCFSScheduler s;
    s.addProcess(Process(1, "P1", 2, 3)); // Arrives at t=2 (t=0..2 is IDLE)
    s.addProcess(Process(2, "P2", 10, 4)); // Arrives at t=10 (t=5..10 is IDLE)
    s.simulate();

    assert(s.getCurrentTime() == 14);
    assert(s.getTotalBusyTime() == 7);
    auto metrics = s.getMetrics();
    assert(metrics.cpuUtilization == 50.0); // 7 / 14 * 100 = 50%
    std::cout << "PASSED\n";
}

int main() {
    std::cout << "\n=========================================\n";
    std::cout << "  CPU Scheduler Simulator Test Suite\n";
    std::cout << "=========================================\n";

    testFCFS();
    testSJF();
    testSRTFPreemption();
    testRoundRobin();
    testPriorityAging();
    testIdleCPU();

    std::cout << "\n>>> ALL 6 UNIT TESTS PASSED SUCCESSFULLY! <<<\n\n";
    return 0;
}
