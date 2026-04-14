# Fix: 修复 QML import 版本号（Qt5 语法）

## 关联 Issue

[qml-import-missing-version](../issues/qml-import-missing-version.md)

## 修改文件

`examples/mqtt/main.qml`

## 修复内容

为所有 QML import 添加 Qt 5 所需的版本号：

```qml
// 修复前（Qt 6 风格）
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import CrossOcean.Core

// 修复后（Qt 5 风格）
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CrossOcean.Core 1.0
```

`CrossOcean.Core` 版本号 `1.0` 来自 `core/mqtt/mqtt.cpp` 中的注册：
```cpp
qmlRegisterType<Mqtt>("CrossOcean.Core", 1, 0, "Mqtt");
```

## 验证

```bash
make deploy-clean && make deploy-debug
./deploy/debug/run.sh  # 应正常启动，无 QML 加载错误
```
