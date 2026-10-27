#ifndef SJF_HPP
#define SJF_HPP

#include "Scheduler.hpp"

class SJFScheduler : public Scheduler {
public:
    void simulate() override;
    std::string getName() const override { return "Shortest Job First (SJF - Non-Preemptive)"; }
};

#endif // SJF_HPP
