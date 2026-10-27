#ifndef FCFS_HPP
#define FCFS_HPP

#include "Scheduler.hpp"

class FCFSScheduler : public Scheduler {
public:
    void simulate() override;
    std::string getName() const override { return "First-Come, First-Served (FCFS)"; }
};

#endif // FCFS_HPP
