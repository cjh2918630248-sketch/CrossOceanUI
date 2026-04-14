# Issue: deploy 打包缺失 QML 插件的运行时依赖

## 现象

运行 deploy 包时报错：

```
Cannot load library .../deploy/debug/qml/QtQuick.2/libqtquick2plugin.so:
  (libQt5QmlWorkerScript.so.5: cannot open shared object file: No such file or directory)
QQmlApplicationEngine failed to load component
qrc:/main.qml:1:1: plugin cannot be loaded for module "QtQuick": ...
```

## 原因

`_deploy_app` 打包逻辑只收集了主二进制 (`ldd`) 的 Qt 依赖，但 QML 模块（`QtQuick.2/`、`QtQuick/`）中的插件（`.so`）在运行时**动态加载**，其依赖不会出现在主二进制的 `ldd` 输出中。

例如 `libqtquick2plugin.so` 依赖 `libQt5QmlWorkerScript.so.5`，该库不在主二进制的依赖链中，因此未被打入 `deploy/lib/`。

## 影响范围

所有使用 `QML2_IMPORT_PATH` 加载 QML 模块的 deploy 包。
