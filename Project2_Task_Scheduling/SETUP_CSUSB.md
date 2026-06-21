# Project 2 (Task Scheduling) — Setup & Run (CSUSB, zero cloud)

Build a C++ task-execution library (thread pool + scheduler). Pure CPU, no GPU,
no cloud. Works on any multi-core laptop.

## Install (once)
- **Windows:** WSL2 + Ubuntu, then `sudo apt install build-essential python3`.
- **Mac (Intel or Apple Silicon):** `xcode-select --install`.
- **Linux:** `sudo apt install build-essential python3`.

## Build & run
```bash
cd part_a
make
./runtasks -n 8 <test_name>          # run one test with 8 worker threads
python3 ../tests/run_test_harness.py # full correctness + performance harness
```
Replace `8` with your core count. See `tests/README.md` for test names.

## What to implement
**Part A (required):** three classes in `part_a/tasksys.cpp`, in this order —
`TaskSystemParallelSpawn` → `TaskSystemParallelThreadPoolSpinning` →
`TaskSystemParallelThreadPoolSleeping`. The Serial class is provided as a
correctness reference. See the HINT block at the top of the file.

**Part B (extra credit in this course):** async launches + dependency graph in
`part_b/tasksys.cpp`. See the design sketch in that file. Skip if your
instructor assigned Part A only.

## Notes on grading (laptop-fair)
- The harness compares your implementation to a prebuilt reference binary **on
  your own machine**, so the comparison is fair regardless of core count.
- The shipped Linux reference binaries need a reasonably recent glibc. On a
  current Ubuntu/WSL2 or macOS they run fine. If one won't run on your machine,
  grade correctness + speedup relative to the Serial baseline instead.
- Performance is judged on relative speedup; report your core count.
