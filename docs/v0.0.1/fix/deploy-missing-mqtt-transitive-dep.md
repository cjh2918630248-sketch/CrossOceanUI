# Fix: 扫描 libQt5Mqtt.so.5 的间接依赖补全 deploy 包

## 关联 Issue

[deploy-missing-mqtt-transitive-dep](../issues/deploy-missing-mqtt-transitive-dep.md)

## 修改文件

`Makefile` — `_deploy_app` 函数

## 修复内容

在复制 `libQt5Mqtt.so.*` 之后，对该库单独执行 `ldd` 扫描，将其 Qt 依赖也收入 `lib/`：

```makefile
cp -P $(QTMQTT_INSTALL)/lib/libQt5Mqtt.so.* $(DEPLOY)/lib/
ldd $(QTMQTT_INSTALL)/lib/libQt5Mqtt.so.5 \
    | awk '/\/opt\/Qt\// {print $$3}' \
    | xargs -I{} cp -Lf {} $(DEPLOY)/lib/
```

## 验证

```bash
make deploy-debug
ls deploy/debug/lib/ | grep Network
# 预期：libQt5Network.so.5

./deploy/debug/run.sh
# 不再报 libQt5Network 缺失
```
