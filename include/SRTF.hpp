#ifndef SRTF_HPP
#define SRTF_HPP

#include "Scheduler.hpp"

class SRTFScheduler : public Scheduler {
public:
    void simulate() override;
    std::string getName() const override { return "Shortest Remaining Time First (SRTF - Preemptive)"; }
};

#endif // SRTF_HPP
