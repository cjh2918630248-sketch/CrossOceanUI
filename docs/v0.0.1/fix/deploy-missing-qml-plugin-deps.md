# Fix: deploy 打包时收集 QML 插件的运行时依赖

## 关联 Issue

[deploy-missing-qml-plugin-deps](../issues/deploy-missing-qml-plugin-deps.md)

## 修改文件

`Makefile` — `_deploy_app` 函数

## 修复内容

在复制 QML 模块之后，扫描所有 QML 插件（`.so`）的 Qt 依赖并补全到 `lib/`：

```makefile
# 复制 QML 模块
cp -r $(QT_QML)/QtQuick.2  $(DEPLOY)/qml/
cp -r $(QT_QML)/QtQuick    $(DEPLOY)/qml/
# 收集 QML 插件的 Qt 依赖（运行时加载，主二进制 ldd 不包含）
find $(DEPLOY)/qml -name "*.so" | xargs -I{} ldd {} 2>/dev/null \
    | awk '/\/opt\/Qt\// {print $3}' | sort -u \
    | xargs -I{} cp -Lf {} $(DEPLOY)/lib/ 2>/dev/null; true
```

## 验证

```bash
make deploy-clean && make deploy-debug

# 检查关键库已打入
ls deploy/debug/lib/ | grep QmlWorkerScript

# 运行
./deploy/debug/run.sh  # 应正常启动，无 Cannot load library 错误
```

## 备注

此方案只收集来自 `/opt/Qt/` 的依赖，不会将系统库打入包中，
避免与目标机器的系统库产生冲突。
