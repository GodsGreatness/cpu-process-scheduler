#ifndef PRIORITY_SCHEDULER_HPP
#define PRIORITY_SCHEDULER_HPP

#include "Scheduler.hpp"

class PriorityScheduler : public Scheduler {
private:
    bool preemptive;
    bool enableAging;
    int agingInterval; // Number of time units waiting before priority is boosted

public:
    explicit PriorityScheduler(bool isPreemptive = false, bool aging = false, int interval = 5)
        : preemptive(isPreemptive), enableAging(aging), agingInterval(interval) {}

    void setPreemptive(bool p) { preemptive = p; }
    void setAging(bool a, int interval = 5) {
        enableAging = a;
        agingInterval = interval;
    }

    void simulate() override;
    std::string getName() const override {
        std::string n = preemptive ? "Preemptive Priority" : "Non-Preemptive Priority";
        if (enableAging) {
            n += " (with Aging interval " + std::to_string(agingInterval) + ")";
        }
        return n;
    }
};

#endif // PRIORITY_SCHEDULER_HPP
