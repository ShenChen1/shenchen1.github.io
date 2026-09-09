---
title: "Linux 共享库 soname 版本管理"
date: 2019-10-31T20:04:04+08:00
lastmod: 2026-02-26T10:59:10+00:00
tags: ["linux", "linker", "c"]
summary: "通过实验演示 Linux 共享库的三级命名规则（realname / soname / linkname）以及 soname 如何实现库的平滑升级。"
---

## Linux 共享库命名规则

共享库使用 `libname.so.x.y.z` 的三段版本号：

- **主版本号 (x)**：重大升级，不同主版本号的库之间不兼容
- **次版本号 (y)**：增量升级，增加新接口但保持原有符号不变，高次版本号向后兼容低次版本号
- **发布版本号 (z)**：错误修正和性能改进，不添加或修改接口，完全兼容

由此产生三个名字：

| 名称 | 格式 | 用途 |
|------|------|------|
| **realname** | `libname.so.x.y.z` | 包含实际库代码的文件 |
| **soname** | `libname.so.x` | runtime linker 用来查找库的名字 |
| **linkname** | `libname.so` | 编译器链接时使用的名字 |

## 实验

### 实验源码与 Makefile

配套实验构建脚本见 [`Makefile`](Makefile)，包含接口定义头文件 [`foo.h`](foo.h)、版本 1 实现 [`foo_1.c`](foo_1.c)、版本 2 实现 [`foo_2.c`](foo_2.c) 以及测试程序 [`main.c`](main.c)：

{{< code-file file="Makefile" lang="makefile" >}}

### 编译 v1 并运行

```text
$ make v1 test
gcc -shared -fPIC -Wl,-soname,libfoo.so.1 -o libfoo.so.1.1 foo_1.c
ln -sf libfoo.so.1.1 libfoo.so
gcc main.c -I. -L. -lfoo -o a.out
ldconfig -n .
LD_LIBRARY_PATH=. ./a.out
call foo 1
```

### 升级到 v2 并运行

```text
$ make v2 test
gcc -shared -fPIC -Wl,-soname,libfoo.so.1 -o libfoo.so.1.2 foo_2.c
ldconfig -n .
LD_LIBRARY_PATH=. ./a.out
call foo 2
```

可以看到库已经被升级了——**无需重新编译 `a.out`**，因为 runtime linker 使用的是 soname (`libfoo.so.1`)，而 `ldconfig` 自动更新了 soname 的符号链接。

### 最终的符号链接关系

```text
$ ll
lrwxrwxrwx 1 root root   13 Oct 31 20:06 libfoo.so -> libfoo.so.1.1    # linkname → 编译时版本
lrwxrwxrwx 1 root root   13 Oct 31 20:06 libfoo.so.1 -> libfoo.so.1.2  # soname → 最新 realname
-rwxr-xr-x 1 root root 8104 Oct 31 20:06 libfoo.so.1.1                 # v1 realname
-rwxr-xr-x 1 root root 8104 Oct 31 20:06 libfoo.so.1.2                 # v2 realname
```

- `libfoo.so`（linkname）仍指向 v1，这是编译时确定的
- `libfoo.so.1`（soname）已指向 v2，这是 `ldconfig` 更新的
- 程序运行时查找的是 soname，所以自动使用了新版本

## 相关文件与实验附件

- 公共头文件：[`foo.h`](foo.h)
- 库版本 1 源码：[`foo_1.c`](foo_1.c)
- 库版本 2 源码：[`foo_2.c`](foo_2.c)
- 测试主程序：[`main.c`](main.c)
- 构建脚本：[`Makefile`](Makefile)
