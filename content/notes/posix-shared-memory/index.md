---
title: "POSIX 共享内存 shm_open 实验"
date: 2020-09-24T15:41:34+08:00
lastmod: 2020-09-24T15:41:34+08:00
tags: ["linux", "memory", "c"]
summary: "通过实验验证 POSIX 共享内存的 shm_unlink 行为：删除共享内存对象后，已有的 mmap 映射是否仍然有效。"
---

## 问题

调用 `shm_unlink` 删除共享内存对象后，之前通过 `mmap` 建立的映射是否还能访问？再次 `shm_open` 同名对象后，拿到的是同一块内存吗？

## 实验代码

完整测试代码见源文件 [`posix_shm.c`](posix_shm.c)：

{{< code-file file="posix_shm.c" lang="c" >}}

## 编译和运行

```bash
$ gcc -o posix_shm posix_shm.c -lrt
```

### 不传参数（不删除）

```bash
$ ./posix_shm
[0x7f...]:abcdef
[0x7f...]:abcdef
```

两个指针指向同一个底层对象，写入 `"abcdef"` 后两者内容一致。

### 传参数（先删除再重建）

```bash
$ ./posix_shm 1
[0x7f...]:123456
[0x7f...]:abcdef
```

`shm_unlink` 之后，已有的 `mmap` 映射仍然有效（引用计数未归零），但新创建的同名对象是一个全新的对象，两块内存内容不同。

## 结论

- `shm_unlink` 类似文件的 `unlink`：删除名字，但已有的 fd 和 mmap 映射仍然有效，直到所有引用关闭
- 再次 `shm_open` 同名对象会创建一个全新的共享内存对象
- 这个行为与 POSIX 文件系统语义一致

## 相关文件与实验附件

- POSIX 共享内存验证源码：[`posix_shm.c`](posix_shm.c)
