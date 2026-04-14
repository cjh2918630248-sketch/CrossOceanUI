# Issue: deploy 运行时缺失 libQt5Network.so.5

## 现象

执行 `./deploy/debug/run.sh` 时报错：

```
./example_mqtt: error while loading shared libraries:
  libQt5Network.so.5: cannot open shared object file: No such file or directory
```

## 原因

`_deploy_app` 只对主二进制执行 `ldd`，收集其直接依赖的 Qt 库。  
但 `libQt5Mqtt.so.5` 本身依赖 `libQt5Network.so.5`，而主二进制并不直接链接 Network 模块，因此该依赖**不出现在主二进制的 `ldd` 输出中**，打包时被遗漏。

```
# ldd example_mqtt（片段）
libQt5Mqtt.so.5 => 3rdparty/qtmqtt/install/lib/libQt5Mqtt.so.5
# libQt5Network 不在此列表中

# ldd libQt5Mqtt.so.5（片段）
libQt5Network.so.5 => /opt/Qt/5.15.2/gcc_64/lib/libQt5Network.so.5  ← 被遗漏
```

## 影响范围

`deploy-debug` / `deploy-release` target；所有依赖 `libQt5Mqtt` 的 deploy 包。
