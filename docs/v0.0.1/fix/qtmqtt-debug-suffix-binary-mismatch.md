# Fix: 移除 qtmqtt.pri 中的 _d debug 后缀逻辑

## 关联 Issue

[qtmqtt-debug-suffix-binary-mismatch](../issues/qtmqtt-debug-suffix-binary-mismatch.md)

## 修改文件

`3rdparty/qtmqtt.pri`

## 修复内容

将原先分 debug/release 的两个 `CONFIG()` 分支合并为单条无条件 `LIBS`，并删除 `TARGET` 重命名语句：

**修改前：**

```qmake
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_d)
    LIBS += -L$$PWD/qtmqtt/install/lib -lQt5Mqtt
}
else: {
    CONFIG(release, debug|release): LIBS += -L$$PWD/qtmqtt/install/lib -lQt5Mqtt
}
```

**修改后：**

```qmake
# Linux 无独立 debug 库，debug/release 均链接同一 libQt5Mqtt.so
LIBS += -L$$PWD/qtmqtt/install/lib -lQt5Mqtt
```

## 验证

```bash
make build-debug
ls bin/examples/mqtt/
# 预期：example_mqtt（无 _d 后缀）

make deploy-debug
# 应成功找到二进制并完成打包
```
