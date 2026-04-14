# Plan: Build & Install qtmqtt Third-Party Library

## Problem
需要在 Makefile 中添加编译和安装 `3rdparty/qtmqtt` 的命令，安装路径需与 `3rdparty/qtmqtt.pri` 中引用的路径一致。

## Current State
- Qt 5.15.2 安装于 `/opt/Qt/5.15.2/gcc_64`，qmake 路径：`/opt/Qt/5.15.2/gcc_64/bin/qmake`
- `3rdparty/qtmqtt/` 是完整的 Qt addon module 源码（使用 `load(qt_parts)` 构建系统）
- `3rdparty/qtmqtt.pri` 期望安装路径（注意 typo "intall"）：
  - 头文件：`3rdparty/qtmqtt/intall/include`
  - 动态库：`3rdparty/qtmqtt/intall/lib`
- Makefile 现有结构：包含 init、submodule、doc、git 等分区

## Approach
使用 Qt 的 shadow build 方式：
1. 在 `3rdparty/qtmqtt/build/` 下进行 shadow build
2. 用 `make install INSTALL_ROOT=.../.staging` 安装到临时目录
3. 从临时目录中提取 lib（`libQt5Mqtt*`）和 headers（`QtMqtt/`）到 `intall/lib` 和 `intall/include`
4. 清理临时目录

## Todos
1. 在 Makefile 顶部变量区添加 qtmqtt 相关变量（`QT_QMAKE`, `QTMQTT_SRC`, `QTMQTT_BUILD`, `QTMQTT_INSTALL`）
2. 在 `.PHONY` 中注册新 target：`qtmqtt-build`, `qtmqtt-install`, `qtmqtt-clean`
3. 在 Makefile 中添加 `# ====== 3rdparty ======` 区块，包含三个 target：
   - `qtmqtt-build`：shadow build（mkdir + qmake + make）
   - `qtmqtt-install`：build 后用 INSTALL_ROOT 安装并整理到 `intall/`
   - `qtmqtt-clean`：删除 build 和 intall 目录
4. 在 `init` target 依赖中加入 `qtmqtt-install`（可选，视需求）
5. 在 `help` target 中补充 3rdparty 相关说明

## Notes
- `qtmqtt.pri` 中的 "intall" 是已有的 typo，保持一致不修正
- `QT_QMAKE` 变量使用 `/opt/Qt/5.15.2/gcc_64/bin/qmake`（与 `qmake -version` 一致）
- `make install INSTALL_ROOT=` 会按 Qt 安装树结构存放，需用 `find` 提取到扁平目录
