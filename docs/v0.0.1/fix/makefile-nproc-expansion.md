# Fix: 用 $(shell nproc) 替换 $(nproc) 获取 CPU 核心数

## 关联 Issue

[makefile-nproc-expansion](../issues/makefile-nproc-expansion.md)

## 修改文件

`Makefile` — 变量声明区与 `build-debug` / `build-release` target

## 修复内容

在变量区添加：

```makefile
NPROC := $(shell nproc)
```

并将所有 `-j$(nproc)` 替换为 `-j$(NPROC)`：

```makefile
make -C $(BUILD_DEBUG) -j$(NPROC)
make -C $(BUILD_RELEASE) -j$(NPROC)
```

## 验证

```bash
make build-debug
# 输出中应出现 -j8（或与实际核心数一致的值）
```
