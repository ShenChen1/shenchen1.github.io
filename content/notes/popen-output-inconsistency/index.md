---
title: "popen 捕获的输出与 Shell 直接输出不一致"
date: 2019-10-30T16:51:34+08:00
lastmod: 2019-10-30T10:53:42+02:00
tags: ["linux", "c", "debugging"]
summary: "用 strace 分析 popen 执行 ls 时输出格式与直接在 shell 中执行 ls 不同的原因：terminal 属性检测导致的输出模式差异。"
---

## 现象

做 RPC 时想用 `popen` 执行远程命令并返回执行结果，发现输出格式不一致。

### 直接在 console 执行

```text
$ ls
a.out  dup.c  popen.c
```

文件名之间用空格分隔，横向排列。

### 通过 popen 执行

测试代码见 [`popen.c`](popen.c)：

{{< code-file file="popen.c" lang="c" >}}

```text
$ ./a.out
a.out
dup.c
popen.c
```

文件名变成了每行一个，竖向排列。

作为对照测试，[`dup.c`](dup.c) 通过 `dup2` 将 stdout 重定向到常规文件后调用 `system("ls")`，其输出结果同样呈现单列格式：

{{< code-file file="dup.c" lang="c" >}}

## 用 strace 分析原因

### strace 跟踪直接执行的 ls

```text
$ strace ls
......
execve("/usr/bin/sh", ["sh", "-c", "ls"], [/* 29 vars */]) = 0
......
ioctl(1, TCGETS, {B38400 opost isig icanon echo ...}) = 0
ioctl(1, TIOCGWINSZ, {ws_row=32, ws_col=77, ws_xpixel=0, ws_ypixel=0}) = 0
......
write(1, "a.out  dup.c  popen.c\n", 22) = 22
```

`ls` 通过 `ioctl(TCGETS)` 和 `ioctl(TIOCGWINSZ)` 成功获取了 terminal 属性（波特率、窗口尺寸等），判定 stdout 是一个 terminal，因此使用横向多列输出。

### strace 跟踪 popen 执行的 ls

```text
$ strace -f ./a.out
......
[pid  8204] execve("/bin/sh", ["sh", "-c", "ls"], [/* 29 vars */]) = 0
......
[pid  8204] ioctl(1, TCGETS, 0x7ffced4b8900) = -1 ENOTTY (Inappropriate ioctl for device)
[pid  8204] ioctl(1, TIOCGWINSZ, 0x7ffced4b89d0) = -1 ENOTTY (Inappropriate ioctl for device)
......
[pid  8204] write(1, "a.out\ndup.c\npopen.c\n", 20) = 20
[pid  8203] <... read resumed> "a.out\ndup.c\npopen.c\n", 4096) = 20
```

`popen` 创建的子进程中，stdout 被重定向到了 pipe，不再是 terminal。`ioctl(TCGETS)` 返回 `ENOTTY`，`ls` 判定 stdout 不是 terminal，退回到每行一个文件的输出格式。

## 结论

`ls` 会检查 stdout 是否为 terminal（通过 `isatty()` / `ioctl(TCGETS)`）来决定输出格式。`popen` 的 stdout 是 pipe 而非 terminal，所以输出格式不同。这不是 bug，是 `ls` 的预期行为。

## 相关文件与实验附件

- popen 测试程序：[`popen.c`](popen.c)
- dup2 文件重定向对照程序：[`dup.c`](dup.c)
