# Issue: Makefile 中 $(nproc) 展开为空导致无限并行

## 现象

执行 `make build-debug` 时并行度不受控制，等同于 `make -j`（无上限），  
在资源受限环境下可能导致 OOM 或链接阶段崩溃。

## 原因

Makefile 中写了：

```makefile
make -j$(nproc) ...
```

`$(nproc)` 是 **Make 变量展开**语法，而 `nproc` 未在 Makefile 中定义，展开结果为空字符串，  
最终命令为 `make -j`，即不限制并行任务数。

`nproc` 是 shell 命令，在 Makefile 中调用 shell 命令需使用 `$(shell ...)` 函数：

```makefile
NPROC := $(shell nproc)   # 正确：调用 shell 获取 CPU 核心数
```

## 影响范围

`build-debug` / `build-release` target 的并行编译控制。
