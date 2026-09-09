---
title: "Linux 内核 list.h 和 hashtable.h 的用户态移植与使用"
date: 2019-09-30T14:54:20+08:00
lastmod: 2019-10-01T19:13:49+08:00
tags: ["linux-kernel", "data-structures", "c"]
summary: "将 Linux 内核的 list.h（双向链表）和 hashtable.h（哈希表）移植到用户态，通过示例代码演示增删查遍历操作。"
---

## 背景

Linux 内核中的 [`list.h`](list.h) 和 [`hashtable.h`](hashtable.h) 是非常经典的侵入式数据结构实现。它们把链表/哈希节点嵌入到数据结构中，通过 `container_of` 宏获取外层结构体指针，避免了额外的内存分配。

这里把内核的相关头文件剥离并移植到用户态环境，去掉了 `rcu`、`spinlock` 等内核依赖。

## 数据结构定义

```c
struct object {
    int id;
    char name[16];

    struct list_head node;      /* 用于链表 */
    /* 或 struct hlist_node node; 用于哈希表 */
};
```

## 双向链表示例

完整双向链表演示见 [`list_example.c`](list_example.c)：

### list_add — 头插法

{{< code-file file="list_example.c" lines="11-38" lang="c" >}}

输出（后插入的在前面）：

```text
obj3
obj2
obj1
```

### list_add_tail — 尾插法

{{< code-file file="list_example.c" lines="40-67" lang="c" >}}

输出（保持插入顺序）：

```text
obj1
obj2
obj3
```

### list_del — 删除节点

{{< code-file file="list_example.c" lines="69-99" lang="c" >}}

输出：

```text
obj1
obj3
```

## 哈希表示例

完整哈希表演示代码见 [`hashtable_example.c`](hashtable_example.c)：

### 增删查与遍历基础操作

{{< code-file file="hashtable_example.c" lines="21-65" lang="c" >}}

注意 `hash_for_each_possible` 遍历的是同一个 bucket 中的所有元素（因为不同 key 可能哈希到同一个 bucket），所以还需要额外比较 key。

### 查看 bucket 分布

{{< code-file file="hashtable_example.c" lines="113-156" lang="c" >}}

由于哈希函数的映射，id=1 和 id=9 可能落在同一个 bucket 中，形成冲突链。

### 相同 key 的元素

同一个 bucket 中可以存放多个相同 key 的元素，它们形成链表：

{{< code-file file="hashtable_example.c" lines="160-189" lang="c" >}}

输出：

```text
bucket[0]=> obj3
bucket[0]=> obj2
bucket[0]=> obj1
```

后插入的在链表头部（和 `list_add` 类似）。

## 构建

工程构建脚本见 [`Makefile`](Makefile)：

{{< code-file file="Makefile" lang="makefile" >}}

## 相关文件与实验附件

- 链表演示源码：[`list_example.c`](list_example.c)
- 哈希表演示源码：[`hashtable_example.c`](hashtable_example.c)
- 构建脚本：[`Makefile`](Makefile)
- 移植头文件库：
  - 双向循环链表：[`list.h`](list.h)
  - 哈希表宏实现：[`hashtable.h`](hashtable.h)
  - 内核哈希函数：[`hash.h`](hash.h)
  - 核心宏定义（`container_of` 等）：[`kernel.h`](kernel.h)
  - 类型定义：[`types.h`](types.h)
  - 偏移与对齐定义：[`stddef.h`](stddef.h)
  - 非法指针毒化：[`poison.h`](poison.h)
  - 二进制对数运算：[`log2.h`](log2.h)
  - 编译器内建属性封装：[`compiler.h`](compiler.h)
