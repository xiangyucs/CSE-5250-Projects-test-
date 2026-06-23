# CSE5250 三个 Project — 详细改动清单（分发前）

> ⚠️ **INSTRUCTOR ONLY — 请勿分发给学生。** 配合 `INSTRUCTOR_EVALUATION.md` 使用。
> 每项给出：文件:行、现状原文、改成什么、为什么。评估机 Debian 12 x86_64 / 32 核实测。
> 日期：2026-06-23。

---

## 🟠 建议改 —— 会让多个学生卡壳

### 改动 1 — P1 prog3：Part 1 让学生找一个不存在的 bug【中】
- **文件**：`Project1_SIMD_and_Multicore/README.md`
- **现状**：
  - 第 158–159 行："You will make a simple fix to Program 3 ... (the error causes a performance problem, not a correctness one)."
  - 第 161–164 行："With the correct fix, you should observe a large speedup..."
  - 但起始 `prog3_mandelbrot_ispc/mandelbrot.ispc:29` 是 `foreach (j = 0 ... height, i = 0 ... width)` —— i（宽度）在内层正好映射 SIMD lane，**布局正确，无 bug 可修**。唯一性能杠杆（task 数）在 Part 2。
- **二选一**：
  - **方案 A（保留找 bug 练习）**：在起始 `mandelbrot.ispc:29` 故意把维度顺序写反成 `foreach (i = 0 ... width, j = 0 ... height)`（SIMD lane 跨行 strided → 慢），让学生改回。这样 Part 1 的"修 bug"叙述就真实了。
  - **方案 B（改文案，更省事）**：删掉 158–159、161–164 的"simple fix / the error"措辞，把 Part 1 改为纯"编译运行 + 分析理论最大加速比"。
- **推荐**：想保留教学意图选 A；只想消除困惑选 B。

### 改动 2 — P1 prog1：Part 5 的"16 线程 ≈ 8 线程"假设【中】
- **文件**：`Project1_SIMD_and_Multicore/README.md`
- **现状**：
  - 第 101 行："5. Now run your improved code with 16 threads. Is performance noticably greater than when running with eight threads? Why or why not?"
  - 第 88–89 行："a typical CPU has a limited number of physical cores (e.g., four) ... roughly twice that many threads ..."
  - 隐含"8 线程已饱和、16≈8"。**32 核机器上实测不成立**（16→15.5x 仍线性），学生"标准答案"会因机器而异。
- **改成（第 101 行）**：
  > "Now run with a thread count equal to your machine's physical core count, then with double that. Does performance keep improving past your core count? Why or why not? (Relate your answer to your own CPU's physical core count and hyper-threading — not a fixed number.)"
- 第 88 行的 "(e.g., four)" 建议软化为 "(varies by machine)"。

### 改动 3 — P3 part3：OpenMP 线程数与 temp 行数不匹配 → 越界【中，运行期最危险】
- **文件**：`Project3_Transformer_Attention/gpt149.py` + 起始 `module.cpp`
- **现状**：
  - `gpt149.py:16-17`：`NUM_THREADS=8` / `torch.set_num_threads(8)` —— 只约束 ATen，**不约束**学生的 `#pragma omp parallel for`。
  - `gpt149.py:116,120`：`temp = torch.zeros((NUM_THREADS, self.N))` = (8, N)。
  - 起始 `module.cpp:198`：skeleton 自带 `temp.index({Slice(omp_get_thread_num(), None)})`。
  - 32 核上 OpenMP 默认开 32 线程 → `omp_get_thread_num()` 可达 31 → 在 8 行 temp 上 `Slice(31)` 取到空 → `ORow[i]` 越界写 → **依赖核数的间歇崩溃/错误，极难 debug**。
- **改成（推荐两处一起做）**：
  1. `gpt149.py` **文件最顶部**（在 `import torch` 之前）加：
     ```python
     import os
     os.environ["OMP_NUM_THREADS"] = "8"   # 让 OpenMP team 与 temp 的 8 行一致
     ```
     这样无论学生怎么写 pragma 都安全。
  2. README part3 加一句提示："你的 OpenMP 线程数不能超过 temp 的行数；建议 `#pragma omp parallel for num_threads(temp.size(0))`。"

---

## 🟡 可选改 —— 影响较小或仅 EC/体验

### 改动 4 — P3 accessor 用非 const `int&`，传计算下标编译报错【低-中，仅 Part4/EC】
- **文件**：起始 `Project3_Transformer_Attention/module.cpp`，第 28、33 行
- **现状**：
  ```cpp
  inline float fourDimRead(std::vector<float> &tensor, int &x, int &y, int &z, int &b, ...)
  inline void  fourDimWrite(std::vector<float> &tensor, int &x, int &y, int &z, int &b, ..., float &val)
  ```
  传右值（如 part4 的 `iStart+ii`）→ "cannot bind non-const lvalue reference to rvalue"。**必做 Part1-3 只传普通循环变量，不受影响**；Part4/flash 及任何写 `i+1` 的学生中招。
- **改成**（按值传）：
  ```cpp
  inline float fourDimRead(std::vector<float> &tensor, int x, int y, int z, int b, ...)
  inline void  fourDimWrite(std::vector<float> &tensor, int x, int y, int z, int b, ..., float val)
  ```
  （后面的 `const int &sizeX...` 保持不变，const 引用能绑右值。）

### 改动 5 — P3 起始 module.cpp 缺 include【低】
- **文件**：起始 `module.cpp`，第 7 行 `#include <immintrin.h>` 之后
- **现状**：缺 `<cmath>`(std::exp)、`<algorithm>`(std::min，Part2/4 用)、`<omp.h>`（**skeleton 自己在 198 行调了 `omp_get_thread_num`** —— 学生照抄就编译失败）。
- **改成**：补三行
  ```cpp
  #include <cmath>
  #include <algorithm>
  #include <omp.h>
  ```

### 改动 6 — P3 文案笔误【低】
- `gpt149.py:322`：`-N` 的 help 写成 `"Flash Attention Br Size"`（从上一行 `-br` 复制来的），`-N` 实为**序列长度**。改成 `help="Sequence length N"`。
- `gpt149.py:89/91`：方法名 `myUnfusedAttention` 但里面是 NAIVE attention（调 `mr.myNaiveAttention`），profiler 标签也是 "NAIVE"。轻微混淆，可统一命名（非必须）。

### 改动 7 — P1 prog6 data.dat 写到当前目录【信息】
- `gen_data.cpp:120` / `utils.cpp` 把 `./data.dat` 读写在**当前工作目录**。学生若从只读检出树运行会失败。可在 SETUP 加一句"从可写目录运行"。

### 改动 8 — P2 完整 harness 跑全套 >7 分钟【信息】
- `Project2_Task_Scheduling/tests/README.md` 可补一句：单测用 `./runtasks -n <核数> <test>`、或 harness 用 `-t <test...>` 跑子集，避免一次跑全套耗时过长。

---

## ✅ 无需改（并修正之前一处误报）
- **~~起始码含 .o/二进制~~ —— 误报，撤销**：实测 `git ls-files` **没有任何** `.o`/prog 二进制/`.ppm`/`.log` 入库，`.gitignore` 已全覆盖。起始码是干净的。（这是评估子进程无法跑 git 时的错误推测，现已核实纠正。）
- P2 整体扎实；harness + 参考二进制在 Debian 12 正常；`spin_between_run_calls` 是有效压力测试。
- SETUP_CSUSB.md 的 venv/ninja/module_ref 注记经实测准确。
- prog2/4/5、P2 三类、P3 part1-3 题面清晰可解。

---

## 改动归属一览（方便分配）

| # | 文件 | 改起始码? | 优先级 |
|---|---|---|---|
| 1 | P1 README（+可选 mandelbrot.ispc） | 文案（A 还需改 .ispc） | 中 |
| 2 | P1 README L101/88 | 文案 | 中 |
| 3 | P3 gpt149.py 顶部 + README | 是 | 中 |
| 4 | P3 module.cpp L28/33 | 是 | 低-中 |
| 5 | P3 module.cpp 顶部 | 是 | 低 |
| 6 | P3 gpt149.py L322 | 是 | 低 |
| 7 | P1 SETUP | 文案 | 信息 |
| 8 | P2 tests/README | 文案 | 信息 |
