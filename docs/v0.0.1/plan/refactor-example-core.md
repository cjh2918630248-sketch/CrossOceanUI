# 重构计划：移除 QML，改用 QWidget + 修改 Mqtt 父类

## 目标
1. `core/mqtt/`: 将 `Mqtt` 父类从 `QQuickItem` 改为 `QObject`
2. `examples/mqtt/`: 移除 QML，改用 QWidget 重新实现 UI
3. `examples/pointslistmodel/`: 移除所有 QML 文件，改用 QWidget 重新实现三个视图

---

## 任务列表

### Task 1: 修改 core/mqtt/ 父类 + 移除 registerQml
- **mqtt.h**: `QQuickItem` → `QObject`，移除 `#include <QQuickItem>` 和 `#include <QQmlEngine>`，构造参数改为 `QObject *parent`，删除 `static void registerQml()` 声明
- **mqtt.cpp**: 构造函数签名与初始化列表同步更新，删除 `registerQml()` 方法实现
- **mqtt.pri**: 无需改动（Qt 模块由各 .pro 文件控制）

### Task 2: 重构 examples/mqtt/
删除文件：
- `main.qml`
- `qml.qrc`

修改/新增文件：
- **`mqtt.pro`**: `QT += quick` → `QT += widgets`，移除 QML 资源和 QMLCACHE_DIR，添加新源文件
- **`main.cpp`**: `QGuiApplication + QQmlApplicationEngine` → `QApplication + MqttWindow`
- **`mqttwindow.h/.cpp`** (新增): QMainWindow，包含：
  - 连接配置区 (hostname/port/username/password QLineEdit/QSpinBox)
  - 状态标签 + Connect/Disconnect 按钮
  - 错误标签
  - QTabWidget：Ship Data / Propulsion Data / Order Data (只读 QTextEdit) + Publish 页

### Task 3: 重构 examples/pointslistmodel/
删除文件：
- `main.qml`
- `MapView.qml`
- `DataTableView.qml`
- `DirectListView.qml`
- `qml.qrc`

修改/新增文件：
- **`pointslistmodel.pro`**: `QT += quick` → `QT += widgets`，移除 QML 资源，添加新源文件
- **`main.cpp`**: `QGuiApplication + QQmlApplicationEngine` → `QApplication + MainWindow`
- **`mainwindow.h/.cpp`** (新增): QMainWindow，包含：
  - QToolBar：添加轨迹点按钮、暂停/继续更新、重置数据、点数量标签
  - QTabWidget：地图视图 / 数据表格 / 直接列表
  - QTimer 替代 QML Timer
- **`mapview.h/.cpp`** (新增): 继承 QWidget，用 QPainter 绘制轨迹，响应 model 的 `pointUpdated` 信号
- **`datatableview.h/.cpp`** (新增): QWidget 包含 QLineEdit(轨迹ID过滤) + QTableView，使用 QSortFilterProxyModel
- **`directlistview.h/.cpp`** (新增): QWidget 包含 QListView，直接绑定 PointsListModel

### Task 4: 移除 core/datamodel/ 下所有 registerQml
涉及文件（声明 + 实现同步删除，并移除相关 `#include <QQmlEngine>`）：
- **pointslistmodel.h/.cpp**
- **trajectorylistmodel.h/.cpp**
- **distancelistmodel.h/.cpp**
- **modelmanager.h/.cpp**（包含对子模型 registerQml 的调用链）

---

## 注意事项
- `DataGenerator` 无需改动，仍作为 QObject 使用
- `datagenerator.h` 是 header-only，继续保留
- 保持与现有 QMqttClient 的信号连接逻辑不变
