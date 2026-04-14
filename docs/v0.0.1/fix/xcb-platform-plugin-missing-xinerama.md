# Fix: 安装 libxcb-xinerama0 并更新 deploy 打包逻辑

## 关联 Issue

[xcb-platform-plugin-missing-xinerama](../issues/xcb-platform-plugin-missing-xinerama.md)

## 修复内容

### 1. 系统安装缺失库（开发机）

```bash
sudo apt-get install -y libxcb-xinerama0
```

### 2. 更新 deploy 打包逻辑（Makefile）

在 `_deploy_app` 中增加一步：复制 `libqxcb.so` 的系统库依赖到 `lib/`，
使 deploy 包在未安装 `libxcb-xinerama0` 的目标机器上也能运行：

```makefile
ldd $(DEPLOY)/plugins/platforms/libqxcb.so \
    | awk '/\/usr\/lib\// {print $3}' \
    | xargs -I{} cp -Lf {} $(DEPLOY)/lib/ 2>/dev/null; true
```

此步骤在复制 `libqxcb.so` 之后执行，将其所有 `/usr/lib` 依赖一并打入 `lib/`。

## 验证

安装后重新打包并检查：

```bash
make deploy-clean && make deploy-debug

# 验证无缺失依赖
ldd deploy/debug/plugins/platforms/libqxcb.so | grep "not found"
# 应无输出

# 运行
./deploy/debug/run.sh
```

## 备注

- `libxcb-xinerama0` 为**运行时依赖**，开发机需安装后 deploy 才能收集到该库
- 目标机器如已安装 `libxcb-xinerama0`，则 deploy 包中的副本也不影响正常运行
