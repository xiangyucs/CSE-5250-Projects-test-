# Project 1 (SIMD & Multicore) — Setup & Run (CSUSB, zero cloud)

CPU SIMD + multithreading performance analysis. No GPU, no cloud.

## Install (once)
- **Windows:** install WSL2 + Ubuntu, then `sudo apt install build-essential`.
- **Mac (Intel or Apple Silicon):** `xcode-select --install`, then
  `brew install ispc` (only needed for prog3/4/5).
- **Linux:** `sudo apt install build-essential`; download ISPC from
  https://ispc.github.io/downloads.html and put its `bin/` on your PATH.

The Makefiles auto-detect x86_64 vs Apple Silicon — no manual flag editing.

## Build & run each program
```bash
cd prog1_mandelbrot_threads && make && ./mandelbrot      # std::thread speedup
cd prog2_vecintrin         && make && ./myexp           # simulated SIMD (no ISPC)
cd prog3_mandelbrot_ispc   && make && ./mandelbrot_ispc  # needs ISPC
cd prog4_sqrt              && make && ./sqrt             # needs ISPC
cd prog5_saxpy             && make && ./saxpy            # needs ISPC
cd prog6_kmeans            && make && ./kmeans           # see note below
```

## prog6 (k-means) — the data just works now
You do **not** need Stanford's network. On first run, `./kmeans` notices
`data.dat` is missing and generates it locally (~160MB, a few seconds), then
caches it. To pre-build it instead:
```bash
make gen && ./gen_data          # default ~160MB (200k points)
./gen_data 1000000              # optional: original ~800MB workload
```
Data generation is deterministic — every student gets identical bytes.

## What to implement
prog6: profile the three functions in `kmeansThread.cpp`, then parallelize the
hotspot (`computeAssignments`) with `std::thread`. See the HINT block in that
file. Other progs: follow the README; only small code edits, lots of analysis.

## Grading (laptop-fair)
Report your core count and speedup **relative to the 1-thread/serial run on your
own machine**. No absolute targets tied to specific hardware.
