#include "pointslistmodel.h"

#include <QDebug>
#include <cmath>

// 构造函数，初始化父对象
PointsListModel::PointsListModel(QObject *parent)
    : QAbstractListModel(parent)  // 调用父类构造函数
{}

/* ---------------------------------- 增 ---------------------------------- */
// 在指定索引位置插入数据
void PointsListModel::insert(int index, const ModelDataPoint &data) {
  if (index < 0 || index > mList.count()) {  // 检查索引是否有效
    emit errorOccurred(
        ErrorType::InvalidIndex,
        QString("Invalid index when inserting data: %1").arg(index));
    return;
  }
  beginInsertRows(QModelIndex(), index, index);  // 通知视图即将插入行
  mList.insert(index, data);                     // 在列表中插入数据
  endInsertRows();                               // 通知视图插入行完成
  emit pointUpdated();                           // 发出 countChanged 信号
  emit pointAdded(data.trajectoryID(), data.pointID());  // 发出 pointAdded 信号
}
// 追加数据到列表末尾
void PointsListModel::append(const ModelDataPoint &data) {
  insert(count(), data);  // 调用 insert 函数，在列表末尾插入数据
}
void PointsListModel::append(const QVariantMap map) {
  // {
  //     "trajectoryID": 0,
  //     "latitude": 31.8795,
  //     "longitude": 121.6238
  // }
  if (!map.contains("trajectoryID") || !map.contains("latitude") ||
      !map.contains("longitude")) {
    emit errorOccurred(ErrorType::InvalidData,
                       "Lack of necessary fields when appending data.");
    return;
  }

  // 获取变量并检查类型转换
  QVariant trajIDVar = map["trajectoryID"];
  QVariant latVar = map["latitude"];
  QVariant lonVar = map["longitude"];

  bool ok = false;

  // 尝试转换轨迹ID
  int trajectoryID = trajIDVar.toInt(&ok);
  if (!ok) {
    emit errorOccurred(
        ErrorType::InvalidData,
        QString("TrajectoryID cannot be converted to integer: %1")
            .arg(trajIDVar.toString()));
    return;
  }

  // 尝试转换纬度
  double latitude = latVar.toDouble(&ok);
  if (!ok || std::isnan(latitude) || std::isinf(latitude)) {
    emit errorOccurred(
        ErrorType::InvalidData,
        QString("Latitude cannot be converted to a valid double: %1")
            .arg(latVar.toString()));
    return;
  }

  // 尝试转换经度
  double longitude = lonVar.toDouble(&ok);
  if (!ok || std::isnan(longitude) || std::isinf(longitude)) {
    emit errorOccurred(
        ErrorType::InvalidData,
        QString("Longitude cannot be converted to a valid double: %1")
            .arg(lonVar.toString()));
    return;
  }

  int pointID = count();  // 获取轨迹点个数

  // 检查经纬度是否在合理范围内
  if (latitude < -90 || latitude > 90 || longitude < -180 || longitude > 180) {
    emit errorOccurred(ErrorType::OutOfRange,
                       QString("Latitude and longitude are out of range: "
                               "latitude=%1, longitude=%2")
                           .arg(latitude)
                           .arg(longitude));
    return;
  }

  ModelDataPoint data(trajectoryID, pointID, latitude,
                      longitude);  // 创建 ModelDataPoint 对象
  append(data);  // 调用 append 函数，追加数据到列表末尾
}

/* ---------------------------------- 删 ---------------------------------- */
// 移除指定索引位置的数据
void PointsListModel::remove(int index) {
  if (index < 0 || index >= mList.count()) {  // 检查索引是否有效
    emit errorOccurred(
        ErrorType::InvalidIndex,
        QString("Invalid index when deleting data.: %1").arg(index));
    return;
  }

  ModelDataPoint data =
      mList.at(index);  // 获取指定索引位置的 ModelDataTrajectory 对象
  beginRemoveRows(QModelIndex(), index, index);  // 通知视图即将移除行
  mList.removeAt(index);                         // 从列表中移除数据
  endRemoveRows();                               // 通知视图移除行完成
  emit pointUpdated();                           // 发出 countChanged 信号
  emit pointRemoved(data.trajectoryID(),
                    data.pointID());  // 发出 pointRemoved 信号
}
// 清空所有数据
void PointsListModel::clear() {
  beginResetModel();    // 通知视图即将重置模型
  mList.clear();        // 清空列表
  endResetModel();      // 通知视图重置模型完成
  emit pointUpdated();  // 发出 countChanged 信号
}

/* ---------------------------------- 查 ---------------------------------- */
// 返回模型中的数据数量
int PointsListModel::count() const {
  return rowCount(QModelIndex());  // 调用 rowCount 函数，返回行数
}
// 返回指定索引和角色的数据
QVariant PointsListModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0 || index.row() >= mList.count())  // 检查索引是否有效
    return QVariant();                                  // 返回空 QVariant

  const ModelDataPoint &data =
      mList[index.row()];  // 获取指定索引位置的 ModelDataTrajectory 对象

  if (role == TrajectoryIDRole)  // 如果角色是 TrajectoryIDRole
    return data.trajectoryID();  // 返回轨迹ID
  else if (role == PointIDRole)  // 如果角色是 PointsNumRole
    return data.pointID();       // 返回轨迹点个数
  else if (role == LatitudeRole)
    return data.latitude();
  else if (role == LongitudeRole)
    return data.longitude();

  return QVariant();  // 返回空 QVariant
}
// QML 获取对应点的数据
QVariantMap PointsListModel::get(int index) {
  if (index < 0 || index >= mList.count()) {  // 检查索引是否有效
    emit errorOccurred(
        ErrorType::InvalidIndex,
        QString("Invalid index when retrieving data.: %1").arg(index));
    return QVariantMap();
  }
  ModelDataPoint data =
      mList.at(index);  // 获取指定索引位置的 ModelDataTrajectory 对象
  QVariantMap map;
  map.insert("trajectoryID", data.trajectoryID());
  map.insert("pointID", data.pointID());
  map.insert("latitude", data.latitude());
  map.insert("longitude", data.longitude());
  return map;
}
// 获取指定索引位置的 ModelDataPoint
bool PointsListModel::modelData(int index, ModelDataPoint &ModelDataPoint) {
  if (index < 0 || index >= mList.count()) {  // 检查索引是否有效
    emit errorOccurred(
        ErrorType::InvalidIndex,
        QString("Invalid index when obtaining model data.: %1").arg(index));
    return false;
  }

  ModelDataPoint =
      mList.at(index);  // 获取指定索引位置的 ModelDataTrajectory 对象
  return true;
}
// 打印所有数据
void PointsListModel::printAllData() {
  if (mList.count() == 0) return;
  QDebug info = qInfo();
  info << "PointsListModel:\n";
  for (int i = 0; i < mList.count(); i++) {  // 遍历列表
    info << "[" << i << "]" << mList[i] << "\n";
  }
}
// friend QDebug operator<<(QDebug dbg, const ModelDataPoint &point)
QDebug operator<<(QDebug dbg, const ModelDataPoint &point) {
  dbg.nospace() << "ModelDataPoint(trajectoryID: " << point.trajectoryID()
                << ", pointID: " << point.pointID()
                << ", latitude: " << QString::number(point.latitude(), 'f', 6)
                << ", longitude: " << QString::number(point.longitude(), 'f', 6)
                << ")";
  return dbg.space();
}
// 返回角色名称的哈希表
QHash<int, QByteArray> PointsListModel::roleNames() const {
  QHash<int, QByteArray> roles;  // 创建角色名称的哈希表
  roles[TrajectoryIDRole] =
      "trajectoryID";  // 添加 TrajectoryIDRole 对应的角色名称
  roles[PointIDRole] = "pointID";  // 添加 PointsNumRole 对应的角色名称
  roles[LatitudeRole] = "latitude";
  roles[LongitudeRole] = "longitude";
  return roles;  // 返回角色名称的哈希表
}
// 返回模型中的行数
int PointsListModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);      // 忽略未使用的参数
  return mList.count();  // 返回列表中的项数
}

/* ---------------------------------- 改 ---------------------------------- */
// 修改指定索引位置的数据
void PointsListModel::set(int index, const double latitude,
                          const double longitude) {
  if (index < 0 || index >= mList.count()) {  // 检查索引是否有效
    emit errorOccurred(
        ErrorType::InvalidIndex,
        QString("Invalid index when modifying data. %1").arg(index));
    return;
  }

  // 检查是否是NaN或无穷大
  if (std::isnan(latitude) || std::isnan(longitude) || std::isinf(latitude) ||
      std::isinf(longitude)) {
    emit errorOccurred(ErrorType::InvalidData,
                       QString("Invalid latitude or longitude values (NaN or "
                               "infinity): lat=%1, lon=%2")
                           .arg(latitude)
                           .arg(longitude));
    return;
  }

  // 检查经纬度是否在合理范围内
  if (latitude < -90 || latitude > 90 || longitude < -180 || longitude > 180) {
    emit errorOccurred(ErrorType::OutOfRange,
                       QString("Latitude and longitude are out of range: "
                               "latitude=%1, longitude=%2")
                           .arg(latitude)
                           .arg(longitude));
    return;
  }

  ModelDataPoint data =
      mList.at(index);  // 获取指定索引位置的 ModelDataTrajectory 对象
  data.setLatitude(latitude);    // 设置纬度
  data.setLongitude(longitude);  // 设置经度

  mList.replace(index, data);  // 替换指定索引位置的数据
  emit dataChanged(this->index(index),
                   this->index(index));  // 发出 dataChanged 信号
  emit pointUpdated();                   // 发出 pointUpdated 信号
  emit pointChanged(data.trajectoryID(),
                    data.pointID());  // 发出 pointChanged 信号
}
