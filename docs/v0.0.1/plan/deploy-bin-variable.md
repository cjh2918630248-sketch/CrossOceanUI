# Plan: Makefile 支持灵活 deploy 目标 + 移除 QML 部署步骤

## 背景

原 `_deploy_app` 函数硬编码了 mqtt 示例的二进制路径，且包含 QML 模块复制步骤  
（`QtQuick.2/`、`QtQuick/`、`QML2_IMPORT_PATH`）。  
重构为纯 QWidget 后，QML 步骤需要移除；同时应支持通过变量指定任意二进制路径。

## 变更内容

### 1. 添加 `DEPLOY_BIN` 变量

```makefile
DEPLOY_BIN ?= bin/examples/mqtt/example_mqtt
```

- 默认指向 mqtt 示例（无 debug/release 后缀，因 qtmqtt.pri 已去掉 `_d` 后缀）
- 用户可在命令行覆盖：`make deploy-debug DEPLOY_BIN=bin/examples/pointslistmodel/example_pointslistmodel`

### 2. 移除 QML 部署步骤

从 `_deploy_app` 中删除：
- `cp -r $(QT_QML)/QtQuick.2  $(DEPLOY)/qml/`
- `cp -r $(QT_QML)/QtQuick    $(DEPLOY)/qml/`
- `find $(DEPLOY)/qml -name "*.so" | xargs ...` QML 插件依赖扫描
- `run.sh` 中的 `QML2_IMPORT_PATH` 环境变量

删除无用变量：`QT_QML`、`DEPLOY_*/qml/` 目录创建。

### 3. 补充 libQt5Mqtt 间接依赖扫描

参见 [fix/deploy-missing-mqtt-transitive-dep](../fix/deploy-missing-mqtt-transitive-dep.md)。

## 验证

```bash
# 默认目标（mqtt）
make deploy-debug
./deploy/debug/run.sh

# 覆盖为 pointslistmodel
make deploy-debug DEPLOY_BIN=bin/examples/pointslistmodel/example_pointslistmodel
./deploy/debug/run.sh
```
