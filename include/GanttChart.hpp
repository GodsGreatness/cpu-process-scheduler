#ifndef GANTT_CHART_HPP
#define GANTT_CHART_HPP

#include <vector>
#include <string>
#include <iostream>

struct GanttSegment {
    int pid;
    std::string name;
    int startTime;
    int endTime;

    GanttSegment(int p, std::string n, int start, int end)
        : pid(p), name(std::move(n)), startTime(start), endTime(end) {}
};

class GanttChart {
private:
    std::vector<GanttSegment> segments;

public:
    void addSegment(int pid, const std::string& name, int start, int end);
    void printChart(std::ostream& out = std::cout) const;
    const std::vector<GanttSegment>& getSegments() const { return segments; }
    void clear() { segments.clear(); }
};

#endif // GANTT_CHART_HPP
