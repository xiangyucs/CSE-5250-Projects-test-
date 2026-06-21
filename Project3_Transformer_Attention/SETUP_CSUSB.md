# Project 3 (Transformer Attention) — Setup & Run (CSUSB, zero cloud)

Implement and optimize the attention mechanism of a Transformer in C++ (plugged
into a small NanoGPT). **Runs on CPU only — no GPU, no cloud.**

## Install (once)
You need Python 3, PyTorch (CPU build), a C++ compiler, and OpenMP.

> **Use a virtual environment.** On recent Debian/Ubuntu/WSL2, a bare
> `pip3 install torch` is blocked by PEP 668 ("externally-managed environment").
> Create a venv first (Debian/Ubuntu also need the `python3.11-venv` apt package):
> ```bash
> python3 -m venv ~/cse5250-venv
> source ~/cse5250-venv/bin/activate      # then run the pip/python commands below in it
> ```
> Also install **ninja** in the venv (`pip install ninja`) — PyTorch's JIT loader
> that compiles `module.cpp` requires it, or `gpt149.py` errors out immediately.

- **Windows:** WSL2 + Ubuntu, then:
  ```bash
  sudo apt install build-essential python3-pip
  pip3 install torch ninja --index-url https://download.pytorch.org/whl/cpu
  ```
- **Linux:** same as above.
- **Intel / Apple Silicon Mac:**
  ```bash
  xcode-select --install
  brew install libomp           # Apple clang has no OpenMP by default
  pip3 install torch ninja
  ```

## Run
```bash
python3 gpt149.py part0        # warm-up: PyTorch matmul + softmax
python3 gpt149.py part1        # your naive attention
python3 gpt149.py part2        # blocked matmul
python3 gpt149.py part3        # fused attention + OpenMP
python3 gpt149.py part4        # FlashAttention (optional / EC)
python3 gpt149.py 4Daccess     # warm-up tensor accessor test
```
`gpt149.py` compiles your `module.cpp` automatically and picks the correct
compile flags for your platform.

## What to implement (in `module.cpp`)
Warm-up accessors → Part 1 naive → Part 2 blocked → Part 3 fused + OpenMP →
Part 4 FlashAttention. See the README for the math and per-part skeletons.

**Suggested required set for this course:** Parts 1–3. Part 4 (FlashAttention)
and the ISPC extra credit are optional.

## CSUSB changes you should know about
- The original required Stanford's prebuilt reference module (`module_ref.so`,
  x86_64-Linux only) and an SSH cluster. We made the reference **optional**:
  your code's **correctness is checked against PyTorch** either way. On
  non-x86-Linux machines the "reference" timing column simply mirrors your run.
  - **Note on `module_ref.so` and PyTorch versions.** The prebuilt `.so` was
    compiled against an older PyTorch ABI, so on a recent PyTorch (e.g. 2.x) it
    fails to load even on x86_64-Linux (`ImportError: undefined symbol:
    ...at::_ops::zeros::call...`). This is expected and harmless — `gpt149.py`
    catches it, prints `[CSUSB] Staff reference module not available...`, and
    falls back to the PyTorch correctness check. You just won't get a separate
    reference timing column. (To restore it, the `.so` would have to be
    recompiled against your installed PyTorch version.)
- Compile flags are chosen per platform (no x86 `-mavx` on Apple Silicon; proper
  OpenMP flags on macOS).
- The ISPC extra credit still targets x86 AVX2; on Apple Silicon it would need a
  NEON target. Since it's optional, leave it unless you want to attempt it.

## Grading (laptop-fair)
Correctness against PyTorch is pass/fail. For performance, report your core
count and the speedup of each part over your Part 1 baseline.
