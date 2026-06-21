# CSE5250 Programming Assignments — CSUSB Adapted Editions

These are the three core programming assignments for CSE5250 *Parallel Computing*
(Prof. Xiangyu Li, CSUSB, online), adapted from Stanford CS149 so that **every
student can complete them entirely on their own laptop, at zero cloud cost.**

The originals are untouched (still zipped in `../projects/`). All edits live in
these `projects_csusb/` copies and are marked in code with a `CSUSB` comment.

Recommended sequence: **Project 1 → Project 2 → Project 3** (SIMD/multicore → scheduling
→ DNN attention optimization). None of the three requires a GPU or a cloud VM.

---

## Stanford branding removed (穿帮 cleanup)

All Stanford/CS149 traces were stripped from the three projects: course name
(CS149 → CSE5250), repo/lecture URLs, `myth`/AFS data paths, Gradescope → Canvas,
Ed → course discussion board, SUNet ID → campus ID, instructor names (Kayvon),
the `kayvon` model option, and all due dates (you set those in Canvas). The
prog2 intrinsics library was renamed `CS149intrin`/`__cs149_*`/`CS149Logger` →
`CSE5250intrin`/`__cse5250_*`/`CSE5250Logger` (and the READMEs updated to match).
The AWS `cloud_readme.md` and its console screenshots were deleted; Project 2's
README now states the course is zero-cloud. All remaining images were visually
inspected and are clean technical figures. A full-tree scan confirms zero
residual branding.

## What changed, and why (the four difficulty reductions you asked for)

### 1. Environment / build friction removed
- **Project 1 `data.dat` no longer needs Stanford's network.** prog6 (k-means)
  originally pulled an ~800MB file from Stanford AFS. It now **auto-generates an
  equivalent dataset locally on first run** (and caches it). A standalone
  generator (`make gen && ./gen_data`) is also included. Generation is
  deterministic, so every student gets identical data — grading stays fair.
- **Cross-platform Makefiles.** Project 1 Makefiles now auto-detect the CPU
  (`x86_64` vs Apple Silicon `arm64`) and pick the right compiler/ISPC flags, so
  they build on Intel Mac, Apple Silicon Mac, and Linux/WSL2 without manual edits.
- **Modern-compiler fixes.** Added missing `#include <cstring>` / `<cstdlib>`
  that newer g++/clang now require (the original code failed to compile on
  current toolchains).
- **Project 3 builds on any laptop.** `gpt149.py` originally hard-coded x86 flags
  (`-mavx`) and required Stanford's reference `.so` (x86_64-Linux only). It now
  selects compile flags per platform (drops `-mavx` on ARM, uses the correct
  OpenMP flags on macOS) and treats the staff reference module as **optional**,
  falling back to checking correctness against PyTorch.

### 2. Teaching scaffolding added
- **Project 1 prog6** (`kmeansThread.cpp`): a HINT block telling students to profile
  the three loop functions, identify `computeAssignments` as the hotspot, and
  parallelize that one function with `std::thread` + the `start`/`end` fields.
- **Project 2** (`part_a/tasksys.cpp`): a HINT block laying out the recommended
  build order (Spawn → ThreadPool-Spinning → ThreadPool-Sleeping) and what each
  one does. `part_b/tasksys.cpp` has a design sketch for the dependency graph.

### 3. Reduced workload (suggested required set — your call as instructor)
- **Project 1:** prog6's default dataset is now ~160MB / 200k points (was ~800MB /
  1M points), so runs finish in seconds. Set `DATA_M = 1000000` in `main.cpp`
  (or `./gen_data 1000000`) to restore the original size.
- **Project 2:** treat **Part A as required, Part B as extra credit.** Part B
  (async + dependency graph) is the hardest, most time-consuming piece and is
  not needed to teach the core thread-pool concepts.
- **Project 3:** Parts 1–3 cover the key ideas (naive → blocked → fused+OpenMP).
  Consider making **Part 4 (FlashAttention)** and the **ISPC extra credit**
  optional.

### 4. Relaxed, laptop-fair grading
- Grade **correctness first.** For performance, judge **speedup relative to the
  serial baseline on the student's own machine**, and ask students to report
  their core count. This way a 4-core laptop and a 16-core machine are scored on
  the same scale — no absolute thresholds tied to Stanford/AWS hardware.
- Project 2 already compares student-vs-reference *on the same machine*, which is
  self-calibrating; if the prebuilt reference binary won't run on a given
  laptop, fall back to relative-vs-serial.

---

## Platform setup matrix (zero cloud)

| Student laptop | How to run all three |
|---|---|
| **Windows** | Install **WSL2 + Ubuntu** (free, local). Inside it you get a normal x86-64 Linux toolchain; everything below works, including the prebuilt Linux reference binaries. |
| **Intel Mac** | Use the native toolchain (`xcode-select --install`); `brew install ispc libomp` for the ISPC/OpenMP parts. |
| **Apple Silicon Mac** | Native arm64 toolchain; `brew install ispc libomp`. The CSUSB Makefile changes auto-select ARM/NEON flags. |
| **Linux** | `sudo apt install build-essential`; download ISPC for the ISPC parts. |

Per-assignment build/run commands are in each folder's `SETUP_CSUSB.md`.

---

## Verification status (built & run on an arm64 Linux box during adaptation)
- **Project 1**: prog1, prog2, prog6 compile and run on ARM; prog6 auto-generates
  `data.dat` (154MB) with no download and the standalone generator produces
  byte-identical output (confirmed `cmp`-identical → deterministic grading).
- **Project 2**: part_a compiles and runs (correctness + timing) on ARM.
- **Project 3**: `gpt149.py` passes syntax check; the platform flag-selection and
  the optional-reference fallback were verified in isolation. A full end-to-end
  run needs PyTorch, which could not be installed in the adaptation sandbox —
  please do one smoke-test run on a real laptop (instructions in its SETUP).
