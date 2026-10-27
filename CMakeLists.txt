cmake_minimum_required(VERSION 3.15)
project(CPUProcessScheduler VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(CORE_SOURCES
    src/GanttChart.cpp
    src/Metrics.cpp
    src/FCFS.cpp
    src/SJF.cpp
    src/SRTF.cpp
    src/RoundRobin.cpp
    src/PriorityScheduler.cpp
)

# Main Executable
add_executable(cpu_scheduler src/main.cpp ${CORE_SOURCES})

# Test Executable
add_executable(run_tests tests/test_scheduler.cpp ${CORE_SOURCES})

enable_testing()
add_test(NAME UnitTests COMMAND run_tests)
