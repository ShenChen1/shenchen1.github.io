---
title: "四种斐波那契数列算法的 Python 实现"
date: 2020-09-17T17:07:51+08:00
lastmod: 2020-09-17T17:07:51+08:00
tags: ["algorithm", "python"]
summary: "四种不同复杂度的斐波那契算法实现：递归 O(2^n)、迭代 O(n)、快速倍增 O(log n)、黄金比例公式 O(1)，附精度问题分析。"
---

## 方法一：朴素递归 — O(2^n)

最直观的实现，但存在大量重复计算，n 稍大就会超时：

{{< code-file file="fibonacci.py" lines="7-11" lang="python" >}}

## 方法二：迭代 — O(n)

用两个变量滚动计算，空间 O(1)：

{{< code-file file="fibonacci.py" lines="13-23" lang="python" >}}

## 方法三：快速倍增 — O(log n)

利用矩阵快速幂的等价公式：

- F(2k) = F(k) × [2·F(k+1) − F(k)]
- F(2k+1) = F(k)² + F(k+1)²

将 n 转为二进制，从高位到低位扫描，每次做一个 "倍增" 步骤，遇到 1 时额外做一次加法：

{{< code-file file="fibonacci.py" lines="25-42" lang="python" >}}

Python 的大整数支持让这个方法可以精确计算任意大的斐波那契数。

## 方法四：黄金比例公式 — O(1)

利用 Binet 公式：

F(n) = (φ^n − ψ^n) / √5 ≈ round(φ^n / √5)

其中 φ = (1 + √5) / 2 ≈ 1.618...

{{< code-file file="fibonacci.py" lines="45-47" lang="python" >}}

**精度问题**：使用浮点数运算，当 n > 70 时会因精度不足产生错误结果。如果需要精确计算大数，应该用方法三。

## 运行与对比

运行测试与对比：

```bash
$ python fibonacci.py 10
55
55
55
```

```bash
$ python fibonacci.py 100
354224848179261915075
354224848179261915075
354224848179263111168   # 方法四在 n=100 时已经不准了
```

## 相关文件与实验附件

- 算法实现与基准测试脚本：[`fibonacci.py`](fibonacci.py)
