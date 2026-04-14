# Issue: 运行时崩溃 — Could not load Qt platform plugin "xcb"

## 现象

运行 `./deploy/debug/run.sh` 时程序崩溃：

```
qt.qpa.plugin: Could not load the Qt platform plugin "xcb" in "" even though it was found.
This application failed to start because no Qt platform plugin could be initialized.
Reinstalling the application may fix this problem.

Available platform plugins are: xcb.
```

## 原因

Qt 的 XCB 平台插件 `plugins/platforms/libqxcb.so` 在加载时需要 `libxcb-xinerama.so.0`，
但该库：

1. **系统未安装**（运行时找不到）
2. **deploy 包未打入**（打包时未收集平台插件的依赖库）

`ldd plugins/platforms/libqxcb.so` 输出：
```
libxcb-xinerama.so.0 => not found
```

## 影响范围

所有使用 XCB 平台插件的 deploy 包（Linux 桌面环境）。
