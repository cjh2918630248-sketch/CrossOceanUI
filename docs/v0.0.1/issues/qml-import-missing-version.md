# Issue: QML 导入失败 — Library import requires a version

## 现象

应用运行时报错，窗口无法显示：

```
QQmlApplicationEngine failed to load component
qrc:/main.qml: Library import requires a version
qrc:/main.qml: Library import requires a version
qrc:/main.qml: Library import requires a version
qrc:/main.qml: Library import requires a version
qrc:/main.qml: Library import requires a version
```

## 原因

`examples/mqtt/main.qml` 使用了 Qt 6 风格的无版本号 import 语法：

```qml
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import CrossOcean.Core
```

但项目使用的是 **Qt 5.15.2**，Qt 5 的 QML 引擎要求 import 语句必须带版本号。

## 影响范围

`examples/mqtt` 示例程序，使用 Qt 5 编译时无法启动。
