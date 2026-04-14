# CrossOceanUI

## 一、项目概述

CrossOceanUI 是一个基于 **Qt/C++** 的跨平台 UI 框架库，主要面向**海洋/地图类上位机应用**，核心功能包括：
- **MQTT 通信模块**：封装 Qt MQTT 客户端，支持 QML 暴露
- **地理轨迹数据模型**：管理多条轨迹（含路径点、距离标签、闭环等）
- **撤销/重做系统**：基于 `QUndoStack` 实现对轨迹点的操作历史管理
- **Protobuf 协议支持**：通过子模块引入 MCU 侧 proto 文件

---

## 二、项目整体结构

```
crossoceanui/
├── CrossOceanUI.pro          # 顶层 qmake 工程文件（subdirs 模式）
├── Makefile                  # 自动生成的构建文件
├── .gitignore
├── .gitmodules               # git 子模块声明
│
├── 3rdparty/                 # 第三方依赖
│   └── qtmqtt.pri            # qtmqtt 库的 include/lib 路径配置
│       └── qtmqtt/           # (子模块) Qt MQTT 源码或预编译产物
│
├── proto/                    # Protobuf 协议文件（子模块）
│   └── mcu/                  # 指向 YZH-JTU/protobuf_file 子模块
│
├── core/                     # 核心库代码
│   ├── mqtt/                 # MQTT 通信模块
│   │   ├── mqtt.h
│   │   ├── mqtt.cpp
│   │   └── mqtt.pri
│   └── datamodel/            # 数据模型模块
│       ├── datamodel.pri
│       ├── pointslistmodel.h / .cpp
│       ├── trajectorylistmodel.h / .cpp
│       ├── distancelistmodel.h / .cpp
│       ├── modelmanager.h / .cpp
│       └── commands.h / .cpp
│
├── examples/                 # 示例应用
│   ├── mqtt/                 # MQTT 示例程序
│   │   ├── mqtt.pro
│   │   ├── main.cpp
│   │   ├── mqttwindow.h
│   │   └── mqttwindow.cpp
│   └── pointslistmodel/      # 轨迹点数据模型示例程序
│       ├── pointslistmodel.pro
│       ├── main.cpp
│       ├── mainwindow.h / .cpp
│       ├── datagenerator.h
│       ├── datatableview.h / .cpp
│       ├── directlistview.h / .cpp
│       └── mapview.h / .cpp
│
└── docs/                     # 文档
    └── v0.0.1/
        ├── fix/              # Bug 修复记录
        ├── issues/           # 问题追踪文档
        └── plan/             # 版本规划文档
```

---

## 三、各模块详细说明

### 1. 顶层文件

| 文件               | 说明                                                         |
| ------------------ | ------------------------------------------------------------ |
| `CrossOceanUI.pro` | 顶层 qmake 工程，使用 `TEMPLATE = subdirs`，将 `examples/mqtt` 和 `examples/pointslistmodel` 作为子工程编译入口 |
| `Makefile`         | qmake 自动生成，用于 `make` 构建                             |
| `.gitmodules`      | 声明了 `3rdparty/qtmqtt`（Qt MQTT 库）和 `proto/mcu`（MCU Protobuf 文件）两个 git 子模块 |

---

### 2. `3rdparty/` — 第三方依赖

| 文件         | 说明                                                         |
| ------------ | ------------------------------------------------------------ |
| `qtmqtt.pri` | qmake include 片段，配置 `INCLUDEPATH` 和 `LIBS` 指向本地预编译的 `libQt5Mqtt.so`，被各子工程 `include()` 引入 |
| `qtmqtt/`    | git 子模块，存放 Qt MQTT 的编译产物（`install/include` + `install/lib`） |

---

### 3. `proto/` — Protobuf 协议

| 目录         | 说明                                                         |
| ------------ | ------------------------------------------------------------ |
| `proto/mcu/` | git 子模块，指向 [YZH-JTU/protobuf_file](https://github.com/YZH-JTU/protobuf_file)，包含 MCU（嵌入式控制器）与上位机通信的 `.proto` 协议定义文件 |

---

### 4. `core/mqtt/` — MQTT 通信模块

> 封装 `QMqttClient`，对外提供简洁的连接、订阅、发布接口，支持 QML 直接使用。

| 文件       | 说明                                                         |
| ---------- | ------------------------------------------------------------ |
| `mqtt.h`   | `Mqtt` 类声明，继承 `QObject`，暴露 `hostname`、`port`、`username`、`password`、`connectionState` 等 Q_PROPERTY |
| `mqtt.cpp` | 实现连接管理、主题管理、消息收发及 JSON 解析                 |
| `mqtt.pri` | qmake 片段，声明该模块的 SOURCES/HEADERS，供子工程 `include()` |

**核心接口：**

```
连接管理：  connect() / disconnect()
主题管理：  addTopic(topic) / removeTopic(topic) / topics()
消息发布：  publish(topic, QByteArray/QVariantMap, qos)
属性配置：  hostname / port / username / password
信号输出：  messageReceived(topic, bytes)
           messageReceivedJson(topic, QVariantMap)
           connected() / disconnected() / error()
```

---

### 5. `core/datamodel/` — 地理轨迹数据模型模块

这是项目的**核心业务层**，采用 Qt Model/View 架构，构建了三层嵌套的轨迹数据模型。

#### 5.1 数据层次结构

```
ModelManager（统一管理入口）
  └── TrajectoryListModel（轨迹列表，多条轨迹）
        └── ModelDataTrajectory（单条轨迹）
              ├── PointsListModel（路径点列表）
              │     └── ModelDataPoint（单个路径点：轨迹ID、点ID、经纬度）
              └── DistanceListModel（距离标签列表）
```

#### 5.2 文件说明

| 文件                        | 类                                            | 说明                                                         |
| --------------------------- | --------------------------------------------- | ------------------------------------------------------------ |
| `pointslistmodel.h/cpp`     | `PointsListModel` + `ModelDataPoint`          | 管理单条轨迹的路径点列表，继承 `QAbstractListModel`，暴露 `trajectoryID`、`pointID`、`latitude`、`longitude` 四个角色；支持增删改查及 QML 调用 |
| `trajectorylistmodel.h/cpp` | `TrajectoryListModel` + `ModelDataTrajectory` | 管理多条轨迹，每条轨迹持有一个 `PointsListModel` 和 `DistanceListModel`；支持闭环轨迹（`isLoop`）、路径更新（`updatePath`）、距离标签更新 |
| `distancelistmodel.h/cpp`   | `DistanceListModel`                           | 管理单条轨迹中相邻点之间的距离标签，供地图可视化使用         |
| `modelmanager.h/cpp`        | `ModelManager`                                | **统一对外入口**，聚合 `TrajectoryListModel`（本地数据）、`TrajectoryListModel`（PostgreSQL 数据）、`PointsListModel`（点集）、`QUndoStack`（历史栈）；提供 `add/remove/set/undo/redo` 接口 |
| `commands.h/cpp`            | `AddCommand` / `DeleteCommand` / `SetCommand` | 基于 `QUndoCommand` 的命令模式，支持对 `PointsListModel` 的增加、删除、修改操作的撤销/重做 |
| `datamodel.pri`             | —                                             | qmake 片段，声明所有 datamodel 模块的 SOURCES/HEADERS        |

---

### 6. `examples/` — 示例应用

#### 6.1 `examples/mqtt/` — MQTT 功能演示

| 文件               | 说明                                                         |
| ------------------ | ------------------------------------------------------------ |
| `mqtt.pro`         | 子工程配置，引入 `core/mqtt/mqtt.pri` 和 `3rdparty/qtmqtt.pri` |
| `main.cpp`         | 程序入口，创建 `QApplication` 并显示主窗口                   |
| `mqttwindow.h/cpp` | `MqttWindow` 窗口类，演示 MQTT 连接、订阅主题、消息收发的完整流程 |

#### 6.2 `examples/pointslistmodel/` — 轨迹数据模型演示

| 文件                   | 说明                                                |
| ---------------------- | --------------------------------------------------- |
| `pointslistmodel.pro`  | 子工程配置，引入 `core/datamodel/datamodel.pri`     |
| `main.cpp`             | 程序入口                                            |
| `mainwindow.h/cpp`     | 主窗口，整合地图视图、列表视图与数据表格            |
| `datagenerator.h`      | 模拟数据生成器，用于生成测试用轨迹点数据            |
| `mapview.h/cpp`        | 地图视图控件，将轨迹点渲染到地图上                  |
| `datatableview.h/cpp`  | 数据表格视图，以表格形式展示 `PointsListModel` 数据 |
| `directlistview.h/cpp` | 直接列表视图，简单列出所有轨迹点                    |

---

### 7. `docs/v0.0.1/` — 版本文档

| 目录      | 说明                    |
| --------- | ----------------------- |
| `plan/`   | v0.0.1 版本功能规划文档 |
| `issues/` | 已知问题追踪记录        |
| `fix/`    | Bug 修复记录            |

---

## 四、架构设计图

```
┌─────────────────────────────────────────────────────┐
│                    examples/                         │
│  ┌──────────────┐      ┌──────────────────────────┐  │
│  │  mqtt/       │      │  pointslistmodel/        │  │
│  │  MqttWindow  │      │  MainWindow              │  │
│  │  main.cpp    │      │  MapView / TableView     │  │
│  └──────┬───────┘      └────────────┬─────────────┘  │
└─────────┼──────────────────────────┼─────────────────┘
          │ include                  │ include
┌─────────▼──────────────────────────▼─────────────────┐
│                      core/                            │
│  ┌─────────────────┐  ┌──────────────────────────┐   │
│  │  mqtt/           │  │  datamodel/              │   │
│  │  Mqtt            │  │  ModelManager            │   │
│  │  (QMqttClient)   │  │  ├─ TrajectoryListModel  │   │
│  └────────┬─────────┘  │  │   └─ ModelDataTraj.  │   │
│           │            │  │       ├─ PointsModel  │   │
│           │            │  │       └─ DistModel    │   │
│           │            │  └─ QUndoStack            │   │
│           │            │     (Commands)            │   │
│           │            └──────────────────────────┘   │
└───────────┼───────────────────────────────────────────┘
            │ depends
┌───────────▼───────────────────────────────────────────┐
│                    3rdparty/                           │
│   qtmqtt (libQt5Mqtt.so)   |   proto/mcu (protobuf)   │
└───────────────────────────────────────────────────────┘
```

---

## 五、关键设计决策总结

| 设计点         | 方案                                                         |
| -------------- | ------------------------------------------------------------ |
| **构建系统**   | qmake `subdirs` 模式，各模块以 `.pri` 片段被引用，解耦性好   |
| **数据模型**   | 严格遵循 Qt Model/View，`QAbstractListModel` 子类，天然支持 QML ListView 绑定 |
| **撤销/重做**  | 命令模式（`QUndoCommand`）+ `QUndoStack`，操作可追溯         |
| **通信协议**   | MQTT（Qt MQTT 模块）+ Protobuf（MCU 通信），双协议支持       |
| **三方库管理** | git submodule + `.pri` 配置文件，避免直接拷贝源码            |
| **QML 友好**   | 核心类均以 `Q_INVOKABLE` / `Q_PROPERTY` / `Q_ENUM` 标注，随时可注册到 QML 引擎 |
# CrossOceanUI
