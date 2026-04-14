# Issue: 编译失败 — cannot find -lGL

## 现象

执行 `make build-debug` 或 `make build-release` 时，链接阶段报错：

```
/usr/bin/ld: cannot find -lGL: No such file or directory
collect2: error: ld returned 1 exit status
make[1]: *** [Makefile:250: ../../../../bin/examples/mqtt/example_mqtt] Error 1
```

## 原因

Qt Quick (`QT += quick`) 在链接时依赖 OpenGL，需要 `-lGL`。  
链接器在查找 `-lGL` 时只认无版本号的 `libGL.so`，而非运行时的 `libGL.so.1`。

系统中已存在运行时库：
```
/usr/lib/x86_64-linux-gnu/libGL.so.1      # 运行时库（已有）
/usr/lib/x86_64-linux-gnu/libGL.so        # 链接器符号链接（缺失）
```

`libGL.so` 无版本符号链接由 `libgl-dev` / `libgl1-mesa-dev` 开发包提供，该包未安装时链接失败。

## 影响范围

所有使用 `QT += quick` 或 `QT += opengl` 的子项目，包括：
- `examples/mqtt`
