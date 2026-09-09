---
title: "使用 git send-email 发送 Patchset"
date: 2019-02-14T17:56:15+08:00
lastmod: 2019-02-14T18:03:40+08:00
tags: ["git", "workflow"]
summary: "git format-patch 和 git send-email 发送 patchset 的两种方式：--compose 和 --cover-letter，以及邮件列表中 reply-to 的组织结构。"
---

## 方式一：--compose

先生成要提交的几个 patch（这里生成 3 个，`-n` 生成序列信息如 1/3，最早的提交放在最前面）：

```bash
$ git format-patch -s -3 -n
```

发送 patchset：

```bash
$ git send-email --compose ./*.patch
```

`--compose` 会增加一个 `[PATCH 0/m]` 作为 summary。输入命令后编辑器打开，此时需要填写 subject 和 body 作为 `[PATCH 0/m]` 的内容（手写的 summary）。

后面发送的每个 patch 都是对第一条的回复，每个 patch 的 `In-Reply-To` 都是第一条的 `Message-Id`。

这是因为 `git send-email` 默认使用 `--no-chain-reply-to`：

```text
--[no-]chain-reply-to
    If this is set, each email will be sent as a reply to the previous email sent.
    If disabled with "--no-chain-reply-to", all emails after the first will be sent
    as replies to the first email sent. When using this, it is recommended that the
    first file given be an overview of the entire patch series. Disabled by default,
    but the sendemail.chainreplyto configuration variable can be used to enable it.
```

## 方式二：--cover-letter

生成 patch 时使用 `--cover-letter` 参数：

```bash
$ git format-patch -3 -n --cover-letter
```

这会生成一个 `0000-cover-letter.patch` 文件：

```text
--[no-]cover-letter
    In addition to the patches, generate a cover letter file containing the shortlog
    and the overall diffstat. You can fill in a description in the file before sending
    it out.
```

需要编辑 `0000-cover-letter.patch` 的 subject 和 body，作为对这个 patchset 的介绍。**一定要先编辑再发送**，否则 `git send-email` 会拒绝发送。

`--cover-letter` 会自动生成 diffstat 信息，所以不需要额外添加 `--stat` 参数。

然后发送 patch，此时可以不用 `--compose`：

```bash
$ git send-email ./*.patch --to=xxx@gmail.com
```

## 邮件列表中的回复结构

先发送一份 cover letter（无论用什么客户端，只要是 plaintext），然后后续补丁使用 `--chain-reply-to` 或 `--no-chain-reply-to` 来决定回复结构。

使用 `--chain-reply-to`，结果是有深度的链式结构：

```text
XXX CoverLetter
└─> [PATCH 1/2] XXX1
    └─> [PATCH 2/2] XXX2
```

邮件列表建议使用 `--no-chain-reply-to`，结果更加平坦，便于审阅：

```text
XXX CoverLetter
    ├─> [PATCH 1/2] XXX1
    └─> [PATCH 2/2] XXX2
```
