#ifndef ROUND_ROBIN_HPP
#define ROUND_ROBIN_HPP

#include "Scheduler.hpp"

class RoundRobinScheduler : public Scheduler {
private:
    int timeQuantum;

public:
    explicit RoundRobinScheduler(int quantum = 2) : timeQuantum(quantum) {}
    void setTimeQuantum(int quantum) { timeQuantum = quantum; }
    int getTimeQuantum() const { return timeQuantum; }

    void simulate() override;
    std::string getName() const override {
        return "Round Robin (RR, Quantum = " + std::to_string(timeQuantum) + ")";
    }
};

#endif // ROUND_ROBIN_HPP
