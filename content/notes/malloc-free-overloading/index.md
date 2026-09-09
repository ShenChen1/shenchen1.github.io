---
title: "四种 malloc/free 重载方法对比"
date: 2018-10-13T13:01:31+08:00
lastmod: 2020-03-22T21:26:13+08:00
tags: ["linux", "c", "memory", "debugging"]
summary: "对比 Linux 下重载 malloc/free 的四种方法：glibc 弱符号、dlsym 运行时查找、ld --wrap 链接时替换、以及 __malloc_hook。"
---

## 方法一：利用 glibc 的弱符号定义

直接调用 glibc 内部的 `__libc_malloc` 等函数（完整实现见 [`mymalloc1.c`](mymalloc1.c)）：

{{< code-file file="mymalloc1.c" lang="c" >}}

- **优点**：可以直接使用 `LD_PRELOAD`，不需要重新编译
- **缺点**：依赖 glibc 的内部符号，其他 C 库（musl 等）未必有

## 方法二：使用 dlsym 运行时查找

通过 `dlsym(RTLD_NEXT, ...)` 在运行时查找原始函数（完整实现见 [`mymalloc2.c`](mymalloc2.c)）：

{{< code-file file="mymalloc2.c" lang="c" >}}

- **优点**：比较通用，不依赖特定 C 库实现
- **缺点**：直接使用 `LD_PRELOAD` 时，`calloc` 的重载可能引发段错误（`dlsym` 内部会调用 `calloc`，导致无限递归）

## 方法三：使用 ld --wrap 链接时替换

利用链接器的 `--wrap` 选项，在链接时将符号引用重定向（完整实现见 [`mymalloc3.c`](mymalloc3.c)）：

{{< code-file file="mymalloc3.c" lang="c" >}}

- **优点**：比较通用，无递归问题
- **缺点**：只能替换静态文件中的符号，无法替换动态库中的符号调用

## 方法四：使用 __malloc_hook

利用 glibc 提供的 hook 接口（完整实现见 [`malloc_hook.c`](malloc_hook.c)）：

{{< code-file file="malloc_hook.c" lang="c" >}}

- **优点**：glibc 官方接口，使用方便
- **缺点**：`__malloc_hook` 已被标记为 deprecated，线程安全性差

## 构建与测试

配套测试主程序为 [`main.c`](main.c)，被测共享库为 [`test.c`](test.c)，完整构建规则见 [`Makefile`](Makefile)：

{{< code-file file="Makefile" lang="makefile" >}}

运行时使用 `LD_PRELOAD` 加载方法一或方法二：

```bash
$ LD_PRELOAD="./mymalloc1.so" ./malloc-glibc
$ LD_PRELOAD="./mymalloc2.so" ./malloc-dl
$ ./malloc-wrap
```

## 相关文件与实验附件

- 方法一源码（glibc 弱符号）：[`mymalloc1.c`](mymalloc1.c)
- 方法二源码（dlsym RTLD_NEXT）：[`mymalloc2.c`](mymalloc2.c)
- 方法三源码（ld --wrap）：[`mymalloc3.c`](mymalloc3.c)
- 方法四源码（__malloc_hook）：[`malloc_hook.c`](malloc_hook.c)
- 测试主程序：[`main.c`](main.c)
- 被测共享库：[`test.c`](test.c)
- 构建脚本：[`Makefile`](Makefile)
