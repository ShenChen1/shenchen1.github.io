---
title: "用 setjmp/longjmp 实现协程和异常处理"
date: 2019-01-19T22:34:21+08:00
lastmod: 2026-02-26T10:59:10+00:00
tags: ["c", "concurrency"]
summary: "两个 setjmp/longjmp 的实际应用：用户态协程切换和类 try-catch 异常处理机制。"
---

## setjmp/longjmp 简介

`setjmp` 保存当前栈上下文到 `jmp_buf`，返回 0。后续调用 `longjmp` 会恢复这个上下文，使 `setjmp` 再次返回，但返回值变为 `longjmp` 的第二个参数（不会是 0，如果传 0 会变成 1）。

```c
#include <setjmp.h>

void longjmp(jmp_buf env, int val);
void siglongjmp(sigjmp_buf env, int val);
```

`siglongjmp` 与 `longjmp` 类似，区别在于如果对应的 `sigsetjmp` 调用时 `savesigs` 标志非零，`siglongjmp` 还会恢复 signal mask。

## 应用一：用户态协程

利用 `setjmp`/`longjmp` 在两个 "线程" 之间来回切换，模拟协程（完整代码见 [`coroutine.c`](coroutine.c)）：

{{< code-file file="coroutine.c" lang="c" >}}

执行流程：

1. `main` 调用 `setjmp(jmpbuf_th0)` 保存上下文，返回 0
2. `rc0 == 0`，跳过 `thread_1()`，继续到 `setjmp(jmpbuf_th1)`
3. 调用 `thread_0()`，打印后 `longjmp(jmpbuf_th0, 0)`，但 val=0 会变成 1
4. 从 `setjmp(jmpbuf_th0)` 返回，`rc0 == 1`，调用 `thread_1()`
5. `thread_1()` 内 `longjmp(jmpbuf_th1, 0)` → 回到 `setjmp(jmpbuf_th1)` → 调用 `thread_0()`
6. 如此反复，两个 "线程" 交替执行

## 应用二：类 try-catch 异常处理

用 `setjmp`/`longjmp` 模拟 C++ 的 try-catch（完整代码见 [`exception-handling.c`](exception-handling.c)）：

{{< code-file file="exception-handling.c" lang="c" >}}

关键思路：

- `setjmp` 相当于 `try`
- `longjmp` 相当于 `throw`
- 通过 `memcpy` 保存和恢复 `jmp_buf` 实现嵌套的异常处理（类似异常栈）
- `first()` 捕获 `second()` 抛出的 type 3 异常，重映射为 type 1 后继续向 `main` 传播

## 相关文件与实验附件

- 协程模拟源码：[`coroutine.c`](coroutine.c)
- 类 try-catch 异常处理源码：[`exception-handling.c`](exception-handling.c)
