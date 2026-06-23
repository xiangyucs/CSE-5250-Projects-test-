# 工作指令：对 CSE5250 起始码应用分发前修复

> 给本地 Claude（cowork）执行。基于 Debian 12 x86_64 / 32 核的评估结论。
> 配套文档：`INSTRUCTOR_EVALUATION.md`（实测结果）、`CHANGES_DETAILED.md`（逐项理由）。

你在一个 **CSE-5250-Projects- 仓库的本地克隆**里工作，文件是**干净的学生起始码**。请先建分支再改：`git checkout -b pre-distribution-fixes`。逐项应用下面的编辑，每项给了精确的 before/after。完成后按末尾"验证"跑一遍。**只改下列文件，不要改其它。**

> 注意：有一处（改动 1）需要老师二选一，默认走方案 B；若老师要方案 A 再加那段代码改动。

## 改动 3 — P3：修复 part3 OpenMP 越界（最重要）
**文件 `Project3_Transformer_Attention/gpt149.py`**，在第 9 行 `import torch` **之前**插入两行（放文件最顶部最稳）。

找到开头：
```python
import argparse
```
替换为：
```python
import os
os.environ.setdefault("OMP_NUM_THREADS", "8")  # CSUSB: cap OpenMP team to part3 temp's 8 rows (prevents OOB on many-core machines)
import argparse
```

## 改动 4 — P3：accessor 改为按值传参（避免传计算下标编译失败）
**文件 `Project3_Transformer_Attention/module.cpp`**。

找到：
```cpp
inline float fourDimRead(std::vector<float> &tensor, int &x, int &y, int &z, int &b, 
```
替换为：
```cpp
inline float fourDimRead(std::vector<float> &tensor, int x, int y, int z, int b, 
```

找到：
```cpp
inline void fourDimWrite(std::vector<float> &tensor, int &x, int &y, int &z, int &b, 
        const int &sizeX, const int &sizeY, const int &sizeZ, float &val) {
```
替换为：
```cpp
inline void fourDimWrite(std::vector<float> &tensor, int x, int y, int z, int b, 
        const int &sizeX, const int &sizeY, const int &sizeZ, float val) {
```
（只动签名，函数体 stub 不变。）

## 改动 5 — P3：补缺失的 include
**文件 `Project3_Transformer_Attention/module.cpp`**。

找到：
```cpp
#include <immintrin.h>
```
替换为：
```cpp
#include <immintrin.h>
#include <cmath>
#include <algorithm>
#include <omp.h>
```

## 改动 6 — P3：修 `-N` 参数 help 笔误
**文件 `Project3_Transformer_Attention/gpt149.py`**。

找到：
```python
    parser.add_argument("-N", default="1024", help="Flash Attention Br Size")
```
替换为：
```python
    parser.add_argument("-N", default="1024", help="Sequence length N")
```

## 改动 2 — P1：prog1 Part 5 文案（去掉对核数的硬假设）
**文件 `Project1_SIMD_and_Multicore/README.md`**。

找到（第 101 行）：
```
5. Now run your improved code with 16 threads. Is performance noticably greater than when running with eight threads? Why or why not? 
```
替换为：
```
5. Now run your improved code with a thread count equal to your machine's physical core count, then with double that. Does performance keep improving past your core count? Why or why not? (Relate your answer to your own CPU's physical core count and hyper-threading — not a fixed number.)
```

## 改动 1 — P1：prog3 Part 1 "找 bug"文案（老师二选一）
**默认 = 方案 B（纯文案，低风险）**：**文件 `Project1_SIMD_and_Multicore/README.md`**，找到第 158–159 行：
```
You will make a simple fix to Program 3 which is written in a combination of
C++ and ISPC (the error causes a performance problem, not a correctness one).
```
替换为：
```
Program 3 is written in a combination of C++ and ISPC. In Part 1 you will
compile and run it and reason about its SIMD speedup; in Part 2 you will tune
the number of ISPC tasks to use multiple cores.
```
> 若老师选**方案 A（保留找 bug 练习）**：保留上面这段原文，并把 `prog3_mandelbrot_ispc/mandelbrot.ispc` 第 29 行 `foreach (j = 0 ... height, i = 0 ... width)` 改成 `foreach (i = 0 ... width, j = 0 ... height)`（故意制造 strided SIMD 的性能 bug 供学生修）。**做 A 就不要做 B。**

## 改动 7、8 — 文案补充（可选）
- `Project1_SIMD_and_Multicore/SETUP_CSUSB.md`：在 prog6 段落加一句"请从可写目录运行 `./kmeans`（它会在当前目录生成 data.dat）"。
- `Project2_Task_Scheduling/tests/README.md`：加一句"跑单个测试用 `./runtasks -n <核数> <test>`，或 harness 用 `-t <test...>` 跑子集；完整 harness 跑全套较慢"。

---

## 验证（改完必须跑）
```bash
# P3：必须在多核(>8核)机器上验证 part3 不再越界
cd Project3_Transformer_Attention
python3 gpt149.py part1   # 能编译、不崩
python3 gpt149.py part2
python3 gpt149.py part3   # 改动3生效：即使本机>8核也不崩
python3 gpt149.py part4   # 编译通过（改动4/5生效）
# P1：确认仍能编译
cd ../Project1_SIMD_and_Multicore/prog1_mandelbrot_threads && make
cd ../prog3_mandelbrot_ispc && make   # 方案A改了.ispc的话重点验证它能编译
```
> 注：P3 需要 venv 里的 torch + ninja；`gpt149.py` 自动编译 module.cpp。验证 part1-4 时**起始码的 attention 函数还是空的会报 INCORRECT** —— 这正常，你只需确认**能编译通过、不崩溃、不再因 OpenMP 越界而段错误**；正确性是学生填完才有。

完成后 `git commit` 并按需 push。**这些是对起始码的改动，与任何解答无关。**
