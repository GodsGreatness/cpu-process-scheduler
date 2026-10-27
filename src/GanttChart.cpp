#include "GanttChart.hpp"
#include <iomanip>

void GanttChart::addSegment(int pid, const std::string& name, int start, int end) {
    if (start >= end) return;
    
    // Merge contiguous segments of the same process
    if (!segments.empty() && segments.back().pid == pid && segments.back().endTime == start) {
        segments.back().endTime = end;
    } else {
        segments.emplace_back(pid, name, start, end);
    }
}

void GanttChart::printChart(std::ostream& out) const {
    if (segments.empty()) {
        out << "[Empty Gantt Chart]\n";
        return;
    }

    out << "\n=== Gantt Chart Execution Timeline ===\n\n";

    // Top border
    out << "+";
    for (const auto& seg : segments) {
        int width = std::max(6, (seg.endTime - seg.startTime) * 3);
        out << std::string(width, '-') << "+";
    }
    out << "\n";

    // Process names
    out << "|";
    for (const auto& seg : segments) {
        int width = std::max(6, (seg.endTime - seg.startTime) * 3);
        std::string label = (seg.pid == -1) ? "IDLE" : seg.name;
        int padLeft = (width - static_cast<int>(label.length())) / 2;
        int padRight = width - static_cast<int>(label.length()) - padLeft;
        out << std::string(padLeft, ' ') << label << std::string(padRight, ' ') << "|";
    }
    out << "\n";

    // Bottom border
    out << "+";
    for (const auto& seg : segments) {
        int width = std::max(6, (seg.endTime - seg.startTime) * 3);
        out << std::string(width, '-') << "+";
    }
    out << "\n";

    // Timeline timestamps
    out << segments.front().startTime;
    for (const auto& seg : segments) {
        int width = std::max(6, (seg.endTime - seg.startTime) * 3);
        std::string endStr = std::to_string(seg.endTime);
        int spaces = width + 1 - static_cast<int>(endStr.length());
        if (spaces > 0) {
            out << std::string(spaces, ' ') << endStr;
        } else {
            out << " " << endStr;
        }
    }
    out << "\n\n";
}
