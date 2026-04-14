#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>  // 包含 Qt 框架中的 QObject 类，用于对象模型
#include <QVariantMap>

class TrajectoryListModel;  // 前向声明 TrajectoryListModel 类
class QUndoStack;           // 前向声明 QUndoStack 类
class PointsListModel;
class DistanceListModel;

// 定义 ModelManager 类，继承自 QObject
class ModelManager : public QObject {
  Q_OBJECT
 public:
  // 构造函数，接受一个父对象指针，默认为空指针
  explicit ModelManager(QObject *parent = nullptr);
  // 信号绑定函数
  Q_INVOKABLE void bindSignals();

  // QML注册方法 - 已移除

  /* 增 */
  // 添加数据到模型中，接受一个 QVariantMap 类型的数据映射
  Q_INVOKABLE void add(const QVariantMap &dataMap);

  /* 删 */
  // 从模型中移除指定索引位置的数据
  Q_INVOKABLE void remove(int trajectoryID, int pointID);
  // 清空操作栈
  Q_INVOKABLE void clearUndoStack();

  /* 查 */
  Q_INVOKABLE PointsListModel *getPointsModel(int index) const;
  Q_INVOKABLE PointsListModel *getPostgrePointsModel(int index) const;
  Q_INVOKABLE TrajectoryListModel *getPostgreModel() const;
  Q_INVOKABLE TrajectoryListModel *getTrajectoryModel() const;
  Q_INVOKABLE DistanceListModel *getDistanceModel(int index) const;

  /* 改 */
  // 改变对应轨迹点的经纬度
  Q_INVOKABLE void set(int trajectoryID, int pointID, QVariantMap newPoint,
                       QVariantMap oldPoint);
  // 撤销上一次操作
  Q_INVOKABLE void undo();
  // 重做上一次撤销的操作
  Q_INVOKABLE void redo();

 private:
  PointsListModel *mPointsModel = nullptr;
  TrajectoryListModel *mPostgreModel = nullptr;
  TrajectoryListModel *mTrajectoryModel = nullptr;
  QUndoStack *mUndoStack = nullptr;
};

#endif  // MODELMANAGER_H
