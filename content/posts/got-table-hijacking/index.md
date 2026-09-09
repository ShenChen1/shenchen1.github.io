---
title: "x86 GOT 表替换实现函数劫持"
date: 2019-02-10T09:39:59+08:00
lastmod: 2019-02-10T09:41:48+08:00
tags: ["linux", "linker", "c", "security"]
summary: "通过修改 x86_64 ELF 的 GOT (Global Offset Table) 表项，将 printf 替换为自定义的 my_printf，演示动态链接的 PLT/GOT 机制。"
---

## 原理

动态链接的 ELF 程序在调用外部函数时，经过 PLT (Procedure Linkage Table) 跳转到 GOT (Global Offset Table) 中存储的地址。首次调用时 GOT 中存的是动态链接器的 resolve 入口，解析完成后 GOT 被覆写为函数的真实地址。

如果在解析前手动修改 GOT 表项，就可以劫持函数调用。

## 测试程序

[`test.c`](test.c) — 正常调用 `printf`：

{{< code-file file="test.c" lang="c" >}}

[`stub.c`](stub.c) — 替换用的桩函数，内部调用 `puts` 而非 `printf`：

{{< code-file file="stub.c" lang="c" >}}

配套构建脚本见 [`Makefile`](Makefile)：

{{< code-file file="Makefile" lang="makefile" >}}

## 分析 PLT/GOT

### 找到 my_printf 的地址

```bash
$ objdump -d -s -j .text test.out
```

```text
0000000000400560 <my_printf>:
  400560:   48 83 ec 08             sub    $0x8,%rsp
  400564:   e8 97 fe ff ff          callq  400400 <puts@plt>
  400569:   48 83 c4 08             add    $0x8,%rsp
  40056d:   c3                      retq
```

### 找到 printf@plt

```bash
$ objdump -d -s -j .plt test.out
```

```text
00000000004003f0 <printf@plt>:
  4003f0:   ff 25 22 05 20 00       jmpq   *0x200522(%rip)   # 600918 <_GLOBAL_OFFSET_TABLE_+0x18>
  4003f6:   68 00 00 00 00          pushq  $0x0
  4003fb:   e9 e0 ff ff ff          jmpq   4003e0 <_init+0x18>
```

### 找到 printf 的 GOT 位置

```bash
$ objdump -d -s -j .got.plt test.out
```

```text
Contents of section .got.plt:
 600900 68076000 00000000 00000000 00000000  h.`.............
 600910 00000000 00000000 f6034000 00000000  ..........@.....
 600920 06044000 00000000 16044000 00000000  ..@.......@.....
```

`0x600918` 地址上的 `004003f6` 就是 `printf@plt` 中 `pushq $0x0` 的地址。

`0x4003e0` 是动态链接器的处理入口。正常运行后，`0x600918` 处的值会被替换为 `printf` 在动态库内存映像中的真实地址。

因此，直接将 `0x600918` 处的 `f6034000` 替换为 `my_printf` 的地址 `60054000`，就完成了函数替换。

## 执行结果

正常执行 [`test.out`](test.out)：

```text
hello 1
hello 2
hello 3
hello ./test.out
```

GOT 替换后执行 [`a.out`](a.out)：

```text
hello 1
hello 2
hello 3
hello %s

hello %s %s

```

`printf` 被替换为 `puts` 后，格式化字符串不再被解析，`%s` 被原样输出。

## 相关文件与实验附件

- 主程序源码：[`test.c`](test.c)
- 桩函数源码：[`stub.c`](stub.c)
- 构建配置：[`Makefile`](Makefile)
- 原始构建二进制：[`test.out`](test.out)
- GOT 表篡改后的二进制：[`a.out`](a.out)
