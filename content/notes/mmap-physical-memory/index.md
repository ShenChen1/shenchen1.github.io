---
title: "通过 /dev/mem 实现物理地址映射"
date: 2020-11-22T15:09:37+08:00
lastmod: 2020-11-22T15:09:37+08:00
tags: ["linux", "memory", "c"]
summary: "一个用户态工具，通过 mmap /dev/mem 将物理地址映射到用户空间进行读写，附页对齐处理细节。"
---

## 用途

在嵌入式开发中，经常需要从用户态直接访问硬件寄存器或物理内存。Linux 提供了 `/dev/mem` 设备文件，配合 `mmap` 可以将物理地址映射到用户空间。

## 实现

完整工具实现见 [`phymap.c`](phymap.c)：

{{< code-file file="phymap.c" lang="c" >}}

## 使用

```bash
# 编译
$ gcc -o phymap phymap.c

# 读取物理地址 0x80000000 开始的 256 字节（需要 root）
$ sudo ./phymap 0x80000000 256
```

## 注意事项

- 需要 root 权限才能打开 `/dev/mem`
- 内核配置了 `CONFIG_STRICT_DEVMEM` 时，只允许映射非 RAM 区域（如 MMIO 寄存器），需要关闭此选项或使用 `CONFIG_IO_STRICT_DEVMEM`
- `O_SYNC` 标志确保对映射区域的访问不经过 CPU cache，适合访问硬件寄存器
- `MAP_PRIVATE` 使修改不会写回 `/dev/mem`；如果需要写入物理内存，改用 `MAP_SHARED`

## 相关文件与实验附件

- 物理内存映射工具源码：[`phymap.c`](phymap.c)
