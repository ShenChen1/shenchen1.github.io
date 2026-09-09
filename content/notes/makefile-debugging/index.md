---
title: "Makefile 调试方法汇总"
date: 2019-07-08T13:35:49+08:00
lastmod: 2026-02-26T10:59:10+00:00
tags: ["build-system", "makefile", "debugging"]
summary: "三类 Makefile 调试手段：内置函数 info/warning/error 输出调试信息、命令行选项 -n/-p/--warn-undefined-variables、以及 --debug 选项。"
---

## 1. 增加调试信息

使用 `info`/`warning`/`error` 内置函数输出调试信息。这些调用可以放在 Makefile 中的任何位置，执行到时会将信息输出到 stderr。

### info

```makefile
$(info "here add the debug info")
```

缺点：不能打印出 `.mk` 的行号。

### warning

```makefile
$(warning "here add the debug info")
```

`warning` 会输出行号，便于定位执行位置。示例：

```makefile
$(warning A top-level warning)

FOO := $(warning Right-hand side of a simple variable)bar
BAZ = $(warning Right-hand side of a recursive variable)boo

$(warning A target)target: $(warning In a prerequisite list)makefile
	$(BAZ)
	$(warning In a command script)
	ls
$(BAZ):
```

产生如下输出：

```text
$ make
makefile:1: A top-level warning
makefile:2: Right-hand side of a simple variable
makefile:5: A target
makefile:5: In a prerequisite list
makefile:5: Right-hand side of a recursive variable
makefile:8: Right-hand side of a recursive variable
makefile:6: In a command script
ls
makefile
```

注意几个点：
- 简单变量 (`FOO :=`) 在赋值时求值，所以第 2 行的 warning 在读取阶段就输出了
- 递归变量 (`BAZ =`) 在使用时才求值，所以它的 warning 出现在 target 执行阶段

### error

```makefile
$(error "error: this will stop the compile")
```

会停止当前 Makefile 的编译，适合在特定条件下强制中断构建。

### 打印变量的值

```makefile
$(info $(TARGET_DEVICE))
```

### echo

`echo` 只能在 target 的 recipe 中使用，且前面必须是 TAB：

```makefile
target:
	@echo "start compile..."
	@echo $(files)
```

## 2. 命令行选项

### --just-print (-n)

查看 Makefile 展开后的命令，但不真正执行：

```bash
$ make -n
```

在新的 Makefile 上，第一个测试通常就是 `make -n`。GNU make 允许你为 `-n` 模式下的输出保留安静模式修饰符（`@`）。

**注意**：`-n` 并不能抑制所有执行——在立即求值（immediate context）中的 `$(shell ...)` 调用仍然会被执行。比如用 `:=` 赋值的 shell 调用在 make 读取 Makefile 阶段就会执行，不受 `-n` 影响。

常用技巧：

```bash
$ make -n > build.sh
$ sh build.sh
```

导出所有命令后逐行执行，便于排查问题。

### --print-data-base (-p)

输出 make 的内部数据库，包括：variables、directories、implicit rules、pattern-specific variables、files（explicit rules）、vpath search path。

```bash
$ make -p
```

### --warn-undefined-variables

未定义的变量被展开时显示警告。因为未定义变量会被展开为空字符串，变量名拼写错误很容易被忽略。

```bash
$ make --warn-undefined-variables -n
```

**缺点**：很多内置规则包含未定义变量作为用户自定义扩展点，所以会产生大量不相关的警告。实际使用中要注意甄别。

## 3. --debug 选项 (-d)

`make -d` 输出非常详细的调试信息，包括每个目标的依赖检查过程、为什么需要重新构建等。信息量大，建议配合 `grep` 过滤使用。
