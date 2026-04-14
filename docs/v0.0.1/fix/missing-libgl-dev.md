# Fix: 安装 libgl1-mesa-dev 解决 -lGL 链接失败

## 关联 Issue

[missing-libgl-dev](../issues/missing-libgl-dev.md)

## 解决方案

安装 OpenGL 开发包，该包会在系统中创建链接器所需的 `libGL.so` 无版本符号链接：

```bash
sudo apt-get install -y libgl1-mesa-dev
```

安装后验证：

```bash
ls -la /usr/lib/x86_64-linux-gnu/libGL.so
# 预期输出：
# lrwxrwxrwx ... libGL.so -> libGL.so.1
```

## 验证

安装完成后重新执行编译，应成功：

```bash
make build-debug
# 或
make build-release
```

## 备注

此包为**编译期依赖**，运行时不需要（`libGL.so.1` 已随系统安装）。  
在新开发环境初始化时应将此步骤加入环境搭建文档。
