---
title: "使用 gcov 和 lcov 进行代码覆盖率测试"
date: 2022-04-22T11:43:21+08:00
lastmod: 2022-04-22T11:43:21+08:00
tags: ["testing", "c", "build-system"]
summary: "演示如何用 gcc 的 gcov 工具和 lcov 前端生成 C 项目的代码覆盖率报告，包含共享库场景。"
---

## 测试代码

[`test.c`](test.c) — 被测的库代码：

{{< code-file file="test.c" lang="c" >}}

[`main.c`](main.c) — 测试入口：

{{< code-file file="main.c" lang="c" >}}

注意 `main.c` 的两个分支分别调用 `func1` 和 `func2`——覆盖率报告会反映哪些分支被执行了。

## Makefile

配套构建脚本见 [`Makefile`](Makefile)：

{{< code-file file="Makefile" lang="makefile" >}}

关键编译选项：

- `-fprofile-arcs`：插入计数器，运行时记录每个 arc（分支）的执行次数
- `-ftest-coverage`：生成 `.gcno` 文件，记录代码结构信息

## 执行流程

```bash
$ make
```

这会依次：

1. 编译带覆盖率插桩的共享库和测试程序
2. 执行 `./test 1`（走 `argc == 2` 分支，调用 `func1`）
3. 执行 `./test 2`（走 `argc == 2` 分支，调用 `func1`）
4. 生成 `.gcda` 运行时覆盖率数据
5. `gcov *.c` 生成文本格式的覆盖率报告
6. `lcov` 收集覆盖率数据到 `test.info`
7. `genhtml` 生成 HTML 格式的覆盖率报告到 `result/` 目录

## 生成的文件

| 文件 | 阶段 | 说明 |
|------|------|------|
| `*.gcno` | 编译时 | 代码结构信息（basic block 图） |
| `*.gcda` | 运行时 | 执行计数数据 |
| `*.gcov` | `gcov` | 文本格式覆盖率报告 |
| `test.info` | `lcov` | 统一的覆盖率数据 |
| `result/` | `genhtml` | HTML 覆盖率报告 |

打开 `result/index.html` 即可查看可视化的覆盖率报告，能看到每个文件、每个函数、每一行的覆盖情况。

## 相关文件与实验附件

- 被测库源码：[`test.c`](test.c)
- 测试入口源码：[`main.c`](main.c)
- 自动化构建与测试脚本：[`Makefile`](Makefile)
