# CSE5250 三个 Project — 教师评估报告（分发前）

> ⚠️ **INSTRUCTOR ONLY — 请勿分发给学生**（含答案相关的评估结论）。
>
> 以"学生身份"完整做完三个 project，实测于 **Debian 12 (bookworm) x86_64 / 32 核**（CSUSB 标准环境）。
> 性能数字均为本机实测；加速比按"相对自己串行基线"（laptop-fair 评分口径）。
> 评估日期：2026-06-22。

---

## 1. 总评

| Project | 可完成性 | 正确性 | 性能达标 | 分发前需改动 |
|---|---|---|---|---|
| **P1 SIMD & Multicore** | ✅ 全部可做 | ✅ 全过 | ✅ 超目标 | 2 项建议改（文案）+ 数项 LOW |
| **P2 Task Scheduling** | ✅ | ✅ 全过且优于参考 | ✅ | 无需改（仅 1 项 INFO）|
| **P3 Transformer Attention** | ✅ Part1-3 + 可选 Part4 | ✅ 全过 | ✅ | 1 项建议改 + 1 项 EC 坑 + LOW |

**结论：三个 project 都能在标准 Debian 12 / WSL2 环境完整做完、达到性能目标，可以分发。** 建议先处理第 5 节"建议改"里的少量文案/起始码问题，可显著减少学生卡壳。

---

## 2. 环境与工具链（本次评估所用）

- 机器：Debian 12 x86_64，**32 核**。无免密 sudo（apt 安装需手动）。
- 编译：g++ 12.2、make、OpenMP（libgomp）—— 系统自带。
- ISPC 1.28.1：本地解压到 `~/tools/`，加入 PATH（P1 的 prog3/4/5 需要）。
- PyTorch：venv `~/torch-venv`（torch 2.12.1+cpu、numpy、scikit-learn、**ninja**）。P3 与 P1 prog6 画图用它。
- Python 包安装在 Debian 12 受 PEP 668 限制，必须用 venv（已在各 SETUP 注明）。

---

## 3. 各 Project 实测结果

### P1 — SIMD & Multicore（串行基线均本机 32 核测得）

**prog1 mandelbrot（std::thread，interleaved 行映射）— 正确**

| 线程 | view1 | view2 |
|---|---|---|
| 2 | 2.00x | — |
| 3 | 2.99x | — |
| 4 | 3.97x | — |
| 8 | **7.89x** | **7.86x** |
| 16 | 15.55x | 15.32x |

8 线程超过 7-8x 目标，接近线性。3 线程无 dip（interleaved 已消除负载不均）。

**prog2 vecintrin — 正确（clampedExp + arraySum 额外分都过）**；利用率随宽度增大而降：

| VECTOR_WIDTH | 2 | 4 | 8 | 16 |
|---|---|---|---|---|
| 利用率 | 84.9% | 79.9% | 77.4% | 76.2% |

**prog3 mandelbrot ISPC** — SIMD 5.85x；调优 task 数（height/2）后 **110.71x**（远超 32x 目标）。

**prog4 sqrt** — 三种输入：

| 输入 | SIMD | tasks |
|---|---|---|
| baseline（随机） | 4.94x | 97.1x |
| max（全 2.999） | **6.66x** | 125.4x |
| min（每 gang 一个） | **0.91x**（<1，发散惩罚） | 11.9x |

**prog5 saxpy** — tasks 2.13x，**84 GB/s**（内存带宽受限）。

**prog6 kmeans** — 串行 17736 ms → 并行 **3328 ms = 5.33x**（目标 2.1x）；聚类图正常。

**P1 学生工作量预估：约 10–14 小时（小量编码 + 大量分析）。**

### P2 — Task Scheduling（学生 vs 参考二进制，`-n 32`，PERF<1 即优于参考，全 OK）

| 测试 | Spawn | Spin | Sleep |
|---|---|---|---|
| super_light | 0.88 | 0.13 | 0.93 |
| ping_pong_unequal | 0.88 | 0.75 | 0.93 |
| recursive_fibonacci | 0.85 | 0.82 | 0.79 |
| mandelbrot_chunked | — | 0.69 | 0.55 |
| math_operations_in_tight_for_loop | 0.41 | 0.36 | 1.06 |
| spin_between_run_calls | — | 0.81 | 0.56 |

三个调度类**全部通过正确性、整体优于参考二进制**。`spin_between_run_calls` 中 Spin≈串行（忙等）、Sleep 省一半，正是该测试要展示的差异，并且它有效暴露了 spinning 实现的跨-epoch 同步 bug（评估时确实踩到并修复）。

**P2 Part A 学生工作量预估：约 8–12 小时（同步正确性是难点，尤其 Sleeping）。**

### P3 — Transformer Attention（32 核，NUM_THREADS=8，正确性对照 PyTorch）

| Part | 正确性 | cpu time | vs part1 |
|---|---|---|---|
| part1 naive | ✅ | 141 ms | 1.00x |
| part2 blocked | ✅ | 94 ms | 1.50x |
| part3 fused + OpenMP | ✅ | **31.6 ms** | **4.47x** |
| part4 flash（可选 EC） | ✅ | 131 ms | 1.08x |

优化递进清晰。SETUP 的 venv+ninja、module_ref 回退两条注记经实测准确（module_ref.so 在 torch 2.12 下确实加载失败并优雅回退到 PyTorch 校验）。

**P3 Part1-3 学生工作量预估：约 6–10 小时。**

---

## 4. 正确性总结

- **P1**：prog1/3/4/5/6 输出正确（prog1 与串行匹配；prog6 聚类图合理）；prog2 两项校验通过。
- **P2**：三类全部通过 harness 正确性，且性能优于参考。
- **P3**：4Daccess + part1/2/3/4 全部对照 PyTorch 通过。

---

## 5. 分发前改动清单（按优先级）

### 建议改（会让多个学生卡壳）
1. **【P1 prog3 文案矛盾，中】** README Program 3 Part 1 让学生"修一个性能 bug（非正确性）"，但起始 `mandelbrot.ispc` 的 `mandelbrot_ispc` 没有可修的 bug——唯一性能杠杆（task 数）被明确分到 Part 2。**学生会找一个不存在的 bug。** 建议：要么起始码故意把 foreach 顺序写差，要么调和 Part1/Part2 措辞。
2. **【P1 prog1 Part5 假设，中】** README 暗示"16 线程 ≈ 8 线程（超线程饱和）"——32 核机器**实测不成立**（16→15.5x 仍线性）。改成"相对你自己的物理核数比较"。
3. **【P3 part3 OpenMP team 越界，中】** gpt149.py 把 part3 的 `temp` 开成 `(8,N)` 且 `torch.set_num_threads(8)`，但后者只管 ATen，不管学生的 `#pragma omp parallel for`——本机默认开 32 线程，`omp_get_thread_num()` 可达 31 → 切片越界 → **依赖核数的间歇崩溃/错误，极难 debug**。建议：README part3 提示学生把 team 限到 temp 行数（`num_threads(temp.size(0))`），或在 gpt149.py 按实际核数开 temp。

### 可选改（影响较小或仅 EC/体验）
4. **【P3 accessor 签名，低-中，仅影响 Part4/EC】** 起始 `module.cpp` 的 `fourDimRead/Write` 用非 const `int &x,&y,&z,&b`，传计算下标（如 part4 的 `iStart+ii`）会编译报错。**必做 Part1-3 只传普通循环变量，不受影响**；Part4/flash 及任何写 `i+1` 的学生会中招。建议改为按值传 `int`。
5. **【P3 缺 include，低】** 起始 `module.cpp` 缺 `<cmath>`(std::exp)、`<algorithm>`(std::min)、`<omp.h>`(skeleton 自己调了 omp_get_thread_num)。建议补上。
6. **【起始码不干净，低】** 仓库提交里含 `.o`/二进制/`.ppm`/`.log`/`.png`。学生改头文件（如 prog2 VECTOR_WIDTH）时可能用到陈旧 `.o`。建议 ship 干净（make clean）并在宽度扫描说明里提醒 `make clean`。
7. **【P3 文案笔误，低】** gpt149.py 的 `-N` help 写成 "Flash Attention Br Size"（实为序列长度）；part1 函数名 myNaiveAttention 但 profiler 标签 myUnfusedAttention。
8. **【P1 prog6，信息】** data.dat 首次运行写到当前目录，只读检出树会失败——可加一句提示。
9. **【P2 harness，信息】** 完整 harness 跑全套 >7 min；可在 tests/README 提示用 `-t <test>` 跑子集。

### 无需改
- P2 整体扎实；harness + 参考二进制在 Debian 12 正常；`spin_between_run_calls` 是有效压力测试。
- SETUP_CSUSB.md 的 venv/ninja/module_ref 注记经实测准确。
- prog2/4/5/6、P2 三类、P3 part1-3 题面清晰可解。

---

## 6. 复现说明 / 仓库状态

- 解答代码（含修复）保存在评估机的 `~/CSE-5250-Projects-solutions/`（**在学生仓库之外**，不污染、不入库）。
- 评估时把解答写进了 working tree 的起始文件；committed 的起始码（commit `1d6352e`）是**干净的**。还原：`git checkout .`。
- 复现性能：`export PATH=$PATH:~/tools/ispc-v1.28.1-linux/bin`，各 prog 目录 `make` 后按本报告的运行矩阵跑；P3 用 `~/torch-venv/bin/python gpt149.py partN`（PATH 里要有 ninja）。
- 上面"建议改"的第 3/4/5 项若要应用到**起始码**，需在干净起始码上单独做（与解答区分）。
