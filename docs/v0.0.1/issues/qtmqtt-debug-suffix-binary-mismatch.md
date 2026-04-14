# Issue: qtmqtt.pri debug 构建给二进制附加 _d 后缀

## 现象

执行 `make build-debug` 后，mqtt 示例产物为：

```
bin/examples/mqtt/example_mqtt_d    ← 多出 _d 后缀
```

而 pointslistmodel 示例产物为：

```
bin/examples/pointslistmodel/example_pointslistmodel   ← 无后缀
```

导致 `DEPLOY_BIN` 默认值（`bin/examples/mqtt/example_mqtt`）在 deploy 步骤中找不到文件。

## 原因

`3rdparty/qtmqtt.pri` 的 debug 分支中有如下语句：

```qmake
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_d)   ← 强制追加 _d
    ...
}
```

此写法来自 Windows 约定（Windows 的 Qt debug 库均带 `_d` 后缀，如 `Qt5Cored.dll`）。  
Linux 上 Qt 没有独立的 debug 库，`libQt5Mqtt.so` 对 debug/release 完全相同，  
`_d` 后缀仅增加混乱，与项目其他子模块的命名不一致。

## 影响范围

所有引入 `qtmqtt.pri` 的子项目；`make deploy-debug` 因找不到无后缀二进制而失败。
