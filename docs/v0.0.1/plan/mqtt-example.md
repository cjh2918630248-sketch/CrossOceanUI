# Plan: Build Commands for CrossOceanUI + qtmqtt.pri Fix

## Problem
1. Makefile 中缺少编译 `CrossOceanUI.pro` 的 debug/release 命令
2. `qtmqtt.pri` 中 debug 链接 `-lQt5Mqttd` 在 Linux 上不存在（实际只有 `libQt5Mqtt.so`，debug 信息存在 `.debug` 文件中）

## Analysis
- `intall/lib/` 实际文件：`libQt5Mqtt.so`, `libQt5Mqtt.so.5`, `libQt5Mqtt.so.5.15`, `libQt5Mqtt.so.5.15.2`, `libQt5Mqtt.so.5.15.2.debug`
- Linux 上 Qt 无 `_d` 后缀的独立 debug 库，debug 和 release 都链接同一个 `libQt5Mqtt.so`

## Changes
1. **`3rdparty/qtmqtt.pri`**：debug/release 均使用 `-lQt5Mqtt`（去掉 `d` 后缀）
2. **`Makefile`**：
   - 添加变量 `BUILD_DEBUG`, `BUILD_RELEASE`
   - 添加 target：`build-debug`, `build-release`, `build-clean`
   - 更新 `.PHONY` 和 `help`
