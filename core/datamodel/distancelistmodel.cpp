#include "distancelistmodel.h"

#include <QDebug>

DistanceListModel::DistanceListModel(QObject *parent)
    : QAbstractListModel{parent} {}

/* ---------------------------------- 增 ---------------------------------- */
// 在指定索引位置插入数据
void DistanceListModel::insert(int index, const ModelDataDistance &data) {
  beginInsertRows(QModelIndex(), index, index);  // 通知视图即将插入行
  mList.insert(index, data);                     // 在列表中插入数据
  endInsertRows();                               // 通知视图插入行完成
}
// 追加数据到列表末尾
void DistanceListModel::append(const ModelDataDistance &data) {
  insert(count(), data);  // 调用 insert 函数，在列表末尾插入数据
}

/* ---------------------------------- 删 ---------------------------------- */
// 移除指定索引位置的数据
void DistanceListModel::remove(int index) {
  beginRemoveRows(QModelIndex(), index, index);  // 通知视图即将移除行
  mList.removeAt(index);                         // 从列表中移除数据
  endRemoveRows();                               // 通知视图移除行完成
}
// 清空所有数据
void DistanceListModel::clear() {
  beginResetModel();  // 通知视图即将重置模型
  mList.clear();      // 清空列表
  endResetModel();    // 通知视图重置模型完成
}

/* ---------------------------------- 查 ---------------------------------- */
// 返回模型中的行数
int DistanceListModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);      // 忽略未使用的参数
  return mList.count();  // 返回列表中的项数
}
// 返回指定索引和角色的数据
QVariant DistanceListModel::data(const QModelIndex &index, int role) const {
  const ModelDataDistance &data =
      mList[index.row()];  // 获取指定索引位置的 ModelDataTrajectory 对象

  if (role == MiddlePointRole)        // 如果角色是 MiddlePointRole
    return data.middlePoint();        // 返回中间点
  else if (role == DistanceRole)      // 如果角色是 DistanceRole
    return data.distance();           // 返回距离
  else if (role == TrajectoryIDRole)  // 如果角色是 TrajectoryIDRole
    return mTrajectoryID;             // 返回轨迹ID

  return QVariant();  // 返回空 QVariant
}
// 获取指定索引位置的 ModelDataTrajectory
bool DistanceListModel::modelData(int index,
                                  ModelDataDistance &ModelDataTrajectory) {
  ModelDataTrajectory =
      mList.at(index);  // 获取指定索引位置的 ModelDataTrajectory 对象
  return true;
}
// 返回模型中的数据数量
int DistanceListModel::count() const {
  return rowCount(QModelIndex());  // 调用 rowCount 函数，返回行数
}
// 返回角色名称的哈希表
QHash<int, QByteArray> DistanceListModel::roleNames() const {
  QHash<int, QByteArray> roles;              // 创建角色名称的哈希表
  roles[MiddlePointRole] = "middlePoint";    // 添加 MiddlePointRole
  roles[DistanceRole] = "distance";          // 添加 DistanceRole
  roles[TrajectoryIDRole] = "trajectoryID";  // 添加 TrajectoryIDRole
  return roles;                              // 返回角色名称的哈希表
}
// 打印所有数据
void DistanceListModel::printAllData() {
  for (int i = 0; i < mList.count(); i++) {  // 遍历列表
    qDebug() << mList[i];
  }
}
// friend QDebug operator<<(QDebug dbg, const ModelDataDistance &point)
QDebug operator<<(QDebug dbg, const ModelDataDistance &point) {
  dbg.nospace() << "ModelDataDistance("
                << "middlePoint: " << point.middlePoint()
                << ", distance: " << point.distance() << ")";
  return dbg.space();
}

/* ---------------------------------- 改 ---------------------------------- */
// 更新对应轨迹点的距离标签
void DistanceListModel::set(int index, const ModelDataDistance &data) {
  mList[index] = data;  // 更新指定索引位置的数据
  emit dataChanged(this->index(index), this->index(index));  // 发出数据更新信号
}
