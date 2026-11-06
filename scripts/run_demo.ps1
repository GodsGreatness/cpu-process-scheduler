# CPU Process Scheduler - Quick Demo Runner (Windows PowerShell)
$ErrorActionPreference = "Stop"
Write-Host "=================================================" -ForegroundColor Cyan
Write-Host " CPU Process Scheduling Simulator - Quick Demo  " -ForegroundColor Cyan
Write-Host "=================================================" -ForegroundColor Cyan

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$rootDir = Split-Path -Parent $scriptDir
Set-Location $rootDir

if (-not (Test-Path "bin\cpu_scheduler.exe")) {
    Write-Host "[Build] Executables not found. Compiling with make..." -ForegroundColor Yellow
    make all
}

Write-Host "`n[1/3] Running Comprehensive Unit Test Suite..." -ForegroundColor Green
& "bin\run_tests.exe"

Write-Host "`n[2/3] Simulating SRTF on Preemption Workload..." -ForegroundColor Green
& "bin\cpu_scheduler.exe" --file workloads\preemption_test.txt --algorithm srtf

Write-Host "`n[3/3] Running Comparative Benchmark on Heavy Concurrency Workload..." -ForegroundColor Green
& "bin\cpu_scheduler.exe" --file workloads\heavy_concurrency.txt --compare

Write-Host "`n>>> Demo execution completed successfully! <<<" -ForegroundColor Cyan
