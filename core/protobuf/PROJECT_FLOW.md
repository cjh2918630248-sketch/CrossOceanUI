# protobuf 模块项目流程图与说明

## 1. 这个项目在做什么

当前工作区的核心代码位于 `core/protobuf/gins_data_model`，是一个 **Qt Widget 演示工程**，用于展示：

- 如何用 `QAbstractListModel` 管理 `protobuf` 的 `GINSData` 列表；
- 如何在 UI 层以 `QVariantMap` 方式读写数据；
- 如何进行单条数据的序列化/反序列化（protobuf 二进制）。

核心协议定义来自 `proto/mcu/msg_gins_data.proto`，生成文件为 `proto/mcu/proto/msg_gins_data.pb.h/.cc`。

---

## 2. 核心文件与职责

- `gins_data_model/example/main.cpp`  
  Qt 应用入口，创建并显示 `Widget`，退出时调用 `ShutdownProtobufLibrary()`。
- `gins_data_model/example/widget.h/.cpp`  
  演示界面与交互控制层：按钮事件、列表选中、状态提示、hex 展示。
- `gins_data_model/ginsdatalistmodel.h/.cpp`  
  数据模型核心：增删改查、Qt Model 通知、`map <-> proto` 转换、序列化反序列化。
- `proto/mcu/msg_gins_data.proto`  
  协议字段定义（含单位缩放规则，如 `roll_deg10`、`vn_mps100`、`hdop10`）。

---

## 3. 总体流程图（主链路）

```mermaid
flowchart TD
    A[main.cpp 启动 QApplication] --> B[创建并显示 Widget]
    B --> C[Widget 构造: 创建 GinsDataListModel 并绑定 QListView]
    C --> D[用户触发 UI 操作]

    D --> E1[添加/修改: 生成或编辑 QVariantMap]
    D --> E2[删除/清空]
    D --> E3[序列化: 选中行 -> serializeItem]
    D --> E4[反序列化: hex -> bytes -> deserializeAndAppend]
    D --> E5[选中变化: get(index) 刷新详情]

    E1 --> F1[mapToProto: UI友好值 -> 协议值(含缩放)]
    F1 --> G1[写入 m_list 并发出 Qt 模型信号]

    E2 --> G2[更新 m_list 并发出 count/data/item 信号]
    E3 --> H1[protobuf SerializeToString]
    H1 --> I1[bytes 转 hex 显示到 QTextEdit]
    E4 --> H2[protobuf ParseFromArray]
    H2 --> G3[appendProto/updateProto 并刷新视图]
    E5 --> J1[protoToMap: 协议值 -> UI展示值(反缩放)]
    J1 --> K1[右侧详情 QLabel 展示]

    G1 --> L[QListView 自动刷新]
    G2 --> L
    G3 --> L
```

---

## 4. 分流程拆解（更细）

### 4.1 添加数据（Append）

1. 点击“添加”按钮，`Widget::onAppend()` 调用 `sampleData()` 生成 `QVariantMap`。  
2. 调用 `GinsDataListModel::append()`，内部转到 `insert(count, map)`。  
3. `mapToProto()` 执行单位缩放并写入 `GINSData`。  
4. `beginInsertRows/endInsertRows` 修改 `m_list`。  
5. 发出 `countChanged/dataUpdated/itemAdded`，列表刷新。

### 4.2 修改数据（Set）

1. 点击“修改”，读取当前选中行。  
2. 生成新样本并调用 `set(index, map)`。  
3. `mapToProto()` 覆盖目标项。  
4. 发出 `dataChanged + itemChanged + dataUpdated`，仅刷新该行。

### 4.3 删除/清空

- 删除：`remove(index)` 做索引校验 -> `beginRemoveRows/endRemoveRows` -> 发送信号。  
- 清空：`clear()` -> `beginResetModel/endResetModel` -> 发送 `countChanged/dataUpdated`。

### 4.4 序列化（Proto -> Bytes -> Hex）

1. 点击“序列化”，`serializeItem(index)` 校验索引。  
2. 调用 protobuf `SerializeToString` 得到二进制。  
3. UI 将 `QByteArray` 转为 hex 文本显示，便于复制/检查。

### 4.5 反序列化（Hex -> Bytes -> Proto）

1. 点击“反序列化”，将文本框 hex 去空格后 `fromHex`。  
2. `deserializeAndAppend(bytes)` 内部 `ParseFromArray`。  
3. 解析成功后 `appendProto()` 入模型并刷新列表。

### 4.6 选中详情展示

1. 列表选中变化触发 `onSelectionChanged()`。  
2. `get(index)` 内部 `protoToMap()` 将协议值恢复为 UI 友好单位。  
3. 详情区格式化输出（经纬度、姿态、速度、状态等）。

---

## 5. 关键数据转换规则（非常重要）

该项目把“协议字段缩放”统一封装在模型中，避免 UI 到处做换算：

- `roll_deg10 / pitch_deg10`：协议单位是 `0.1 度`，展示时除以 `10`，写回时乘以 `10`。  
- `vn_mps100 / ve_mps100`：协议单位是 `0.01 m/s`，展示时除以 `100`，写回时乘以 `100`。  
- `hdop10`：展示时除以 `10`，写回时乘以 `10`。  

这样可以保证：

- UI 看到的是直观数值；
- 协议里保持稳定、紧凑、跨语言一致的数据表达。

---

## 6. 异常与反馈机制

- 索引越界、编解码失败等问题统一通过 `errorOccurred(errorType, message)` 上抛。  
- `Widget` 里监听该信号并更新状态栏文本。  
- 模型变更后通过 Qt Model 标准通知（`begin/end...` + `dataChanged`）驱动视图更新。

---

## 7. 一句话理解项目

这是一个“**Qt 列表模型 + protobuf 协议对象**”的教学/验证样例：  
UI 只关心友好数据，模型负责协议转换与通知，protobuf 负责跨进程/跨语言的紧凑传输。

