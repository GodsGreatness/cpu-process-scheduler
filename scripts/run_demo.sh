#!/usr/bin/env bash
# CPU Process Scheduler - Quick Demo Runner (Linux / macOS)
set -e

echo "================================================="
echo " CPU Process Scheduling Simulator - Quick Demo  "
echo "================================================="

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
cd "$ROOT_DIR"

if [ ! -f "bin/cpu_scheduler" ]; then
    echo "[Build] Executables not found. Compiling with make..."
    make all
fi

echo -e "\n[1/3] Running Comprehensive Unit Test Suite..."
./bin/run_tests

echo -e "\n[2/3] Simulating SRTF on Preemption Workload..."
./bin/cpu_scheduler --file workloads/preemption_test.txt --algorithm srtf

echo -e "\n[3/3] Running Comparative Benchmark on Heavy Concurrency Workload..."
./bin/cpu_scheduler --file workloads/heavy_concurrency.txt --compare

echo -e "\n>>> Demo execution completed successfully! <<<"
