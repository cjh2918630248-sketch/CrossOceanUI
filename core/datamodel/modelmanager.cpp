#include "modelmanager.h"

#include <QUndoStack>

#include "commands.h"
#include "pointslistmodel.h"
#include "trajectorylistmodel.h"

// 构造函数，初始化成员变量
ModelManager::ModelManager(QObject *parent)
    : QObject(parent)  // 调用父类构造函数
{
  mPointsModel = new PointsListModel(this);
  mPostgreModel = new TrajectoryListModel(this);
  mTrajectoryModel = new TrajectoryListModel(this);
  mUndoStack = new QUndoStack(this);
}

// 信号绑定函数
void ModelManager::bindSignals() {
  // 将 pointsModel 的信号连接到对应的 Trajectorymodel 的函数
  for (int i = 0; i < mTrajectoryModel->rowCount(); ++i) {
    PointsListModel *pointsModel = mTrajectoryModel->getPointsModel(i);
    if (pointsModel) {
      connect(pointsModel, &PointsListModel::pointUpdated, mTrajectoryModel,
              [this, i]() { mTrajectoryModel->updatePath(i); });
      connect(pointsModel, &PointsListModel::pointAdded, mTrajectoryModel,
              [this, i](int trajectoryID, int pointID) {
                mTrajectoryModel->insertDistance(trajectoryID, pointID);
                mTrajectoryModel->updateDistance(trajectoryID, pointID);
              });
      connect(pointsModel, &PointsListModel::pointRemoved, mTrajectoryModel,
              [this, i](int trajectoryID, int pointID) {
                mTrajectoryModel->removeDistance(trajectoryID, pointID);
                // 更新删除点的上一个点的距离标签
                mTrajectoryModel->updateDistance(trajectoryID, pointID - 1);
              });
      connect(pointsModel, &PointsListModel::pointChanged, this,
              [this, i](int trajectoryID, int pointID) {
                mTrajectoryModel->updateDistance(trajectoryID, pointID);
              });
    }
  }

  // 将 pointsModel 的信号连接到对应的 PostgreModel 的函数
  for (int i = 0; i < mPostgreModel->rowCount(); ++i) {
    PointsListModel *pointsModel = mPostgreModel->getPointsModel(i);
    if (pointsModel) {
      connect(pointsModel, &PointsListModel::pointUpdated, mPostgreModel,
              [this, i]() { mPostgreModel->updatePath(i); });
      connect(pointsModel, &PointsListModel::pointAdded, mPostgreModel,
              [this, i](int trajectoryID, int pointID) {
                mPostgreModel->insertDistance(trajectoryID, pointID);
                mPostgreModel->updateDistance(trajectoryID, pointID);
              });
      connect(pointsModel, &PointsListModel::pointRemoved, mPostgreModel,
              [this, i](int trajectoryID, int pointID) {
                mPostgreModel->removeDistance(trajectoryID, pointID);
                // 更新删除点的上一个点的距离标签
                mPostgreModel->updateDistance(trajectoryID, pointID - 1);
              });
      connect(pointsModel, &PointsListModel::pointChanged, this,
              [this, i](int trajectoryID, int pointID) {
                mPostgreModel->updateDistance(trajectoryID, pointID);
              });
    }
  }
}

/* ---------------------------------- 增 ---------------------------------- */
// 添加数据到模型中，接受一个 QVariantMap 类型的数据映射
void ModelManager::add(const QVariantMap &dataMap) {
  // {
  //     "trajectoryID": 7,
  //     "latitude": 31.8795,
  //     "longitude": 121.6238
  // }

  int trajectoryID = dataMap["trajectoryID"].toInt();  // 从数据映射中获取轨迹ID
  int pointsID = mTrajectoryModel->getPointsModel(trajectoryID)
                     ->rowCount();                   // 获取轨迹点个数
  double latitude = dataMap["latitude"].toDouble();  // 从数据映射中获取纬度
  double longitude = dataMap["longitude"].toDouble();  // 从数据映射中获取经度

  ModelDataPoint data(trajectoryID, pointsID, latitude,
                      longitude);  // 创建 ModelDataPoint 对象
  // 获取指定索引位置的 PointsListModel 对象
  mPointsModel = mTrajectoryModel->getPointsModel(trajectoryID);
  QUndoCommand *addCommand =
      new AddCommand(data, mPointsModel);  // 创建 AddCommand 对象
  mUndoStack->push(addCommand);  // 将 AddCommand 对象推入撤销栈
}

/* ---------------------------------- 删 ---------------------------------- */
// 从模型中移除指定索引位置的数据
void ModelManager::remove(int trajectoryID, int pointID) {
  mPointsModel = mTrajectoryModel->getPointsModel(trajectoryID);
  QUndoCommand *deleteCommand =
      new DeleteCommand(pointID, mPointsModel);  // 创建 DeleteCommand 对象
  mUndoStack->push(deleteCommand);  // 将 DeleteCommand 对象推入撤销栈
}
// 清空模型中的所有数据
void ModelManager::clearUndoStack() {
  mUndoStack->clear();  // 调用撤销栈的 clear 函数
}

/* ---------------------------------- 查 ---------------------------------- */
DistanceListModel *ModelManager::getDistanceModel(int index) const {
  return mTrajectoryModel->getDistanceModel(index);
}
PointsListModel *ModelManager::getPointsModel(int index) const {
  return mTrajectoryModel->getPointsModel(index);
}
PointsListModel *ModelManager::getPostgrePointsModel(int index) const {
  return mPostgreModel->getPointsModel(index);
}
TrajectoryListModel *ModelManager::getPostgreModel() const {
  return mPostgreModel;
}
TrajectoryListModel *ModelManager::getTrajectoryModel() const {
  return mTrajectoryModel;
}

/* ---------------------------------- 改 ---------------------------------- */
// 设置对应轨迹点的经纬度
void ModelManager::set(int trajectoryID, int pointID, QVariantMap newPoint,
                       QVariantMap oldPoint) {
  mPointsModel = mTrajectoryModel->getPointsModel(trajectoryID);
  QUndoCommand *setCommand = new SetCommand(
      pointID, newPoint, oldPoint, mPointsModel);  // 创建 SetCommand 对象
  mUndoStack->push(setCommand);  // 将 SetCommand 对象推入撤销栈
}
// 撤销上一次操作
void ModelManager::undo() {
  mUndoStack->undo();  // 调用撤销栈的 undo 函数
}
// 重做上一次撤销的操作
void ModelManager::redo() {
  mUndoStack->redo();  // 调用撤销栈的 redo 函数
}
