# Plan: Deploy/Package Command for CrossOceanUI

## Problem 3（新）
1. 运行时报错：`libQt5Mqtt.so.5: cannot open shared object file`——第三方库未随二进制一起发布
2. 需要 Makefile 支持打包发布（含 Qt 库 + 第三方库）

## Analysis
- `linuxdeployqt` 未安装，需手动 deploy
- `ldd` 输出：`libQt5Mqtt.so.5 => not found`，其余 Qt 库均来自 `/opt/Qt/5.15.2/gcc_64/lib`
- QML 模块：`QtQuick`, `QtQuick.Window`, `QtQuick.Controls`, `QtQuick.Layouts`
- 对应 Qt 目录：
  - libs: `/opt/Qt/5.15.2/gcc_64/lib`
  - plugins: `/opt/Qt/5.15.2/gcc_64/plugins`
  - qml: `/opt/Qt/5.15.2/gcc_64/qml`

## Deploy Directory Structure
```
deploy/{debug,release}/
  example_mqtt          # binary
  lib/                  # all .so (Qt + qtmqtt)
  plugins/platforms/    # libqxcb.so
  qml/                  # QtQuick.2/, QtQuick/ (Controls.2, Layouts)
  run.sh                # launcher (sets LD_LIBRARY_PATH, QML2_IMPORT_PATH)
```

## Todos
1. 添加变量：`QT_DIR`, `QT_PLUGINS`, `QT_QML`, `DEPLOY_DEBUG`, `DEPLOY_RELEASE`
2. 添加 `.PHONY`：`deploy-debug`, `deploy-release`, `deploy-clean`
3. 添加 `deploy` 区块（三个 target）
4. 更新 `help`