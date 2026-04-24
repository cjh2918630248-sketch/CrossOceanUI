# proto_file
### 学习建议（先看这里）

- `msg_*.proto` 是**源定义**，建议阅读和修改这里；
- `proto/msg_*.pb.h` 与 `proto/msg_*.pb.cc` 是 `protoc` 自动生成文件，不建议手工改；
- 学习顺序可按：`enum`（状态位） -> `message`（结构） -> 字段单位缩放（0.1/0.01 等）；
- 每次修改 `.proto` 后，都应重新生成代码并在业务中验证单位换算。

### 生成c语言代码文件

```bash
# 以下命令在Linux(PC)下执行
# 安装protoc-c
sudo apt install -y protobuf-compiler libprotobuf-c-dev

# 假设当前在proto_file目录下
mkdir build && cd build
# 将.proto文件转为c头文件和源文件，执行后会在当前文件夹下生成.c 和 .h文件
protoc -I../ --c_out=. ../*.proto
```

### 生成 C++ 代码文件（当前仓库常见）

```bash
# 在 proto/mcu 目录执行
mkdir -p proto
protoc -I../ --cpp_out=. ../*.proto
```