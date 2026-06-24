# CSE5250 分发前冒烟测试报告

> ⚠️ **INSTRUCTOR ONLY。** 在 Debian 12 x86_64 / 32 核服务器上对 `pre-distribution-fixes` 分支
> 做的最后编译/运行验证（该机器有 ISPC 与 CPU 版 PyTorch，是开发沙箱缺的两样东西）。
> 全部为干净起始码；纯核验，未修改任何文件。日期：2026-06-23。

**分支**：`pre-distribution-fixes`（`b34902a`）

---

## Project 1 — SIMD & Multicore

| 子项 | 编译 | 运行 | 说明 |
|---|---|---|---|
| prog1_mandelbrot_threads | ✅ | ✅ | stub，"does not match serial"（预期）；HINT 完整 |
| prog2_vecintrin | ✅ | ✅ | stub，verification Failed（预期）；HINT 完整 |
| **★prog3_mandelbrot_ispc** | ✅ | ✅ | **`launch[numTasks]` 在 ISPC 下正常编译；serial==ispc==task-ispc 三图逐字节一致（输出正确）**；5.86x / 11.5x |
| **★prog4_sqrt** | ✅ | ✅ | 4.93x SIMD / 96x tasks |
| **★prog5_saxpy** | ✅ | ✅ | 81 GB/s，2.1x tasks |
| prog6_kmeans | ✅ | ✅ | **data.dat 自动生成 154MB（无需网络）**，kmeans 跑完；HINT 完整 |

**P1：全 PASS。** 从未编译过的 `.ispc` 改动（写死 `launch[2]` → 变量 `launch[numTasks]`）编译运行均正确。

## Project 2 — Task Scheduling

| 子项 | 编译 | harness | correctness |
|---|---|---|---|
| part_a | ✅ | ✅ 可跑 | Serial + 全部 PASS |
| part_b | ✅ | ✅ 可跑 | Serial + 全部 PASS |

**P2：全 PASS。** 参考二进制 `runtasks_ref_linux` 本机可运行。注：stub 把并行类退化成串行，跑重负载测试（如 ping_pong_equal）很慢、完整 harness >7min——属 stub 预期，非 bug。

## ★ Project 3 — Transformer Attention（最关键）

| 子项 | 编译 | 崩溃? | correctness |
|---|---|---|---|
| 4Daccess | ✅ | 无 | INCORRECT（stub 预期） |
| part0 | ✅ | 无 | ✅ PASS（纯 PyTorch） |
| part1 naive | ✅ | 无 | INCORRECT（stub 预期） |
| part2 blocked | ✅ | 无 | INCORRECT（stub 预期） |
| **★part3 fused+OMP** | ✅ | **无段错误** | INCORRECT（stub 预期） |
| part4 flash | ✅ | 无 | INCORRECT（stub 预期） |

**三处修复全部在位且生效：**
1. ✅ **module.cpp 干净编译**——accessor 改按值传（`module.cpp:31,36`）、补了 `<cmath>/<algorithm>/<omp.h>`（`8-10`）。part4 那个原来传 `iStart+ii` 的编译错误已消除（整个 module.cpp 含 part4 编译通过）。
2. ✅ **★part3 在 32 核上不越界**——`gpt149.py:2` 的 `os.environ.setdefault("OMP_NUM_THREADS","8")` 兜底在位；stub 状态下 omp pragma 是注释（学生待加），串行跑 `thread_num==0` 不越界；skeleton 新增行内警告 "omp_get_thread_num() stays within temp — do not raise it"。
3. ✅ **module_ref 缺失时回退 PyTorch 校验**正常工作。

## Branding 扫描
✅ **0 条残留**（Stanford/CS149/Kayvon/Gradescope/SUNet/AFS/myth，已排除改编日志 README_CSUSB.md）。

---

## ⚠️ 需要知道的两点（都不是 bug）

1. **P3 各 part 的 correctness 是 INCORRECT，这是预期的，不是失败。** module.cpp 是**学生 stub**（attention 函数体只有示例注释，没实现）。冒烟测试能确认的是：能编译、能运行、不崩溃、OOB 已修；正确性要等学生填完实现才会 PASS。**part0 是唯一会 PASS 的**（纯 PyTorch，不依赖学生代码）。

2. **★part3 OOB 兜底有个残留边角（可选加固）**：`setdefault` 让用户显式设的 `OMP_NUM_THREADS` 优先。实测 `OMP_NUM_THREADS=32 python gpt149.py part3` 不崩——因为 stub 的 pragma 被注释（串行）。一旦学生**既加了 `#pragma omp parallel for` 又手动 `export OMP_NUM_THREADS=32`**，仍可能越界。当前有行内警告兜底，够用；若想彻底防住，可把 `temp` 按 `os.cpu_count()` 开行数，或用 `os.environ["OMP_NUM_THREADS"]="8"`（强制而非 setdefault）。

## 改编引入的 bug
**未发现。** 所有"失败"项都是学生未实现的 stub（预期）；★ 三处高风险改动（prog3 .ispc、P3 accessor/include、P3 OMP 兜底）实测均正确。

---

## 测试环境
- ISPC 1.28.1（`~/tools/`，加入 PATH）— P1 prog3/4/5
- PyTorch 2.12.1+cpu venv（`~/torch-venv`，含 torch/numpy/ninja）— P3，需 ninja 做 JIT 编译
- g++ 12.2、make、OpenMP（libgomp）系统自带
