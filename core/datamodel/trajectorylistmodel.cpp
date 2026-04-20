#include "trajectorylistmodel.h"

#include <cmath>

#define M_PI 3.14159265358979323846

/**
 *  \brief ModelDataTrajectory
 */
/* ---------------------------------- 配 ---------------------------------- */
double calculateDistance(QVariantMap currentPoint, QVariantMap nextPoint) {
  // 根据两个点计算距离
  double currentLatitude = currentPoint.value("latitude").toDouble();
  double currentLongitude = currentPoint.value("longitude").toDouble();
  double nextLatitude = nextPoint.value("latitude").toDouble();
  double nextLongitude = nextPoint.value("longitude").toDouble();

  // 地球半径（单位：米）
  const double R = 6371e3;

  // 将纬度和经度从度转换为弧度
  double φ1 = currentLatitude * M_PI / 180;
  double φ2 = nextLatitude * M_PI / 180;
  double Δφ = (nextLatitude - currentLatitude) * M_PI / 180;
  double Δλ = (nextLongitude - currentLongitude) * M_PI / 180;

  // Haversine 公式
  double a = std::sin(Δφ / 2) * std::sin(Δφ / 2) +
             std::cos(φ1) * std::cos(φ2) * std::sin(Δλ / 2) * std::sin(Δλ / 2);
  double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

  // 计算距离（单位：米）
  double distance = R * c;

  // 保留三位小数
  return std::round(distance * 1000) / 1000;
}

ModelDataDistance calculateMiddlePointAndDistance(
    const ModelDataPoint &point1, const ModelDataPoint &point2) {
  // 计算中间点和距离
  QVariantMap middlePoint = {
      {"latitude", (point1.latitude() + point2.latitude()) / 2},
      {"longitude", (point1.longitude() + point2.longitude()) / 2}};

  double distance = calculateDistance(
      {{"latitude", point1.latitude()}, {"longitude", point1.longitude()}},
      {{"latitude", point2.latitude()}, {"longitude", point2.longitude()}});

  return ModelDataDistance(middlePoint, distance);
}

/* ---------------------------------- 增 ---------------------------------- */
void ModelDataTrajectory::insertDistance(int index) {
  // 插入距离标签数据到模型中
  // 如果轨迹点的数量为1，不添加距离标签
  if (mPoints->rowCount() <= 1) {
    return;
  }
  if (mPoints->rowCount() == 2 && mIsLoop) {
    // 如果闭环，添加第一个点和最后一个点的距离标签
    appendLoopDistance();
  }

  // 当前点
  ModelDataPoint currentPoint;
  mPoints->modelData(index, currentPoint);
  // 前一个点
  ModelDataPoint prePoint;
  mPoints->modelData(index - 1, prePoint);

  ModelDataDistance data =
      calculateMiddlePointAndDistance(currentPoint, prePoint);

  // 中间点的index需要减一进行插入
  mDistanceLab->insert(index - 1, data);

  if (index == mPoints->rowCount() - 1 && mIsLoop) {
    // 如果闭环，删除之前的最后一个点和第一个点的距离标签
    mDistanceLab->remove(mPoints->rowCount() - 2);

    // 添加最后一个点和第一个点的距离标签
    appendLoopDistance();
  }
}

void ModelDataTrajectory::appendDistance() {
  // 添加距离标签数据到模型中
  insertDistance(mPoints->rowCount() - 1);
}

void ModelDataTrajectory::appendLoopDistance() {
  // 添加闭环距离标签数据到模型中
  if (mPoints->rowCount() <= 1) {
    return;
  }

  // 当前点
  ModelDataPoint currentPoint;
  mPoints->modelData(mPoints->rowCount() - 1, currentPoint);
  // 第一个点
  ModelDataPoint prePoint;
  mPoints->modelData(0, prePoint);

  ModelDataDistance data =
      calculateMiddlePointAndDistance(currentPoint, prePoint);

  mDistanceLab->append(data);
}

/* ---------------------------------- 删 ---------------------------------- */

void ModelDataTrajectory::removeDistance(int index) {
  // 移除指定索引位置的距离标签数据

  if (mPoints->rowCount() <= 0) {
    return;
  }
  if (mPoints->rowCount() == 1 && mIsLoop) {
    // 如果闭环，删除第一个点和最后一个点的距离标签
    mDistanceLab->remove(1);
  }

  mDistanceLab->remove(index - 1);
}

/* ---------------------------------- 查 ---------------------------------- */
QDebug operator<<(QDebug dbg, const ModelDataTrajectory &data) {
  // 打印所有数据
  dbg.nospace() << "ModelDataTrajectory(trajectoryID: " << data.trajectoryID()
                << ", isLoop: " << data.isLoop() << ", mPath: " << data.path();
  data.points()->printAllData();
  data.distanceLab()->printAllData();
  return dbg.space();
}

/* ---------------------------------- 改 ---------------------------------- */
void ModelDataTrajectory::updatePath() {
  // 更新Path的轨迹点
  mPath.clear();

  for (int i = 0; i < mPoints->rowCount(); i++) {
    ModelDataPoint data;
    // 获取对应索引位置的轨迹点数据
    mPoints->modelData(i, data);

    QVariantMap point = {{"latitude", data.latitude()},
                         {"longitude", data.longitude()}};

    mPath.append(point);
  }

  // 如果是闭环，添加第一个点到最后位置
  if (mIsLoop && mPoints->rowCount() > 0) mPath.append(mPath.at(0));
}

void ModelDataTrajectory::updateDistanceLab(int index) {
  // 设置对应的距离标签数据

  if (mPoints->rowCount() <= 1) {
    // 如果轨迹点的数量为1，不添加距离标签
    return;
  }

  // 更新左边的距离标签
  if (index > 0) {
    ModelDataPoint currentPoint;
    mPoints->modelData(index, currentPoint);
    ModelDataPoint prePoint;
    mPoints->modelData(index - 1, prePoint);

    ModelDataDistance data =
        calculateMiddlePointAndDistance(currentPoint, prePoint);

    mDistanceLab->set(index - 1, data);
  }

  // 更新右边的距离标签
  if (index < mPoints->rowCount() - 1) {
    ModelDataPoint currentPoint;
    mPoints->modelData(index, currentPoint);
    ModelDataPoint nextPoint;
    mPoints->modelData(index + 1, nextPoint);

    ModelDataDistance data =
        calculateMiddlePointAndDistance(currentPoint, nextPoint);

    mDistanceLab->set(index, data);
  }

  // 更新最后一个点和第一个点的距离标签
  if ((index == 0 || index == mPoints->rowCount() - 1) && mIsLoop) {
    ModelDataPoint currentPoint;
    mPoints->modelData(mPoints->rowCount() - 1, currentPoint);
    ModelDataPoint nextPoint;
    mPoints->modelData(0, nextPoint);

    ModelDataDistance data =
        calculateMiddlePointAndDistance(currentPoint, nextPoint);

    mDistanceLab->set(mPoints->rowCount() - 1, data);
  }
}

/**
 *  \brief TrajectoryListModel
 */
/* ---------------------------------- 配 ---------------------------------- */
// 构造函数，初始化父对象
TrajectoryListModel::TrajectoryListModel(QObject *parent)
    : QAbstractListModel(parent) {}

/* ---------------------------------- 增 ---------------------------------- */
void TrajectoryListModel::insert(int index, const ModelDataTrajectory &data) {
  // 在指定索引位置插入数据
  if (index < 0 || index > mList.count()) {
    return;
  }

  beginInsertRows(QModelIndex(), index, index);  // 通知视图即将插入行
  mList.insert(index, data);                     // 在列表中插入数据
  endInsertRows();                               // 通知视图插入行完成
  emit countChanged(mList.count());              // 发出 countChanged 信号
}

void TrajectoryListModel::insertDistance(int trajectoryID, int pointID) {
  // 追加距离标签数据到模型中，接受一个 index 参数
  if (trajectoryID < 0 || trajectoryID >= mList.count()) {
    return;
  }

  mList[trajectoryID].insertDistance(pointID);
}

void TrajectoryListModel::append(const ModelDataTrajectory &data) {
  // 追加数据到列表末尾
  insert(count(), data);
}

void TrajectoryListModel::append(const QVariantMap map) {
  // 通过 QVariantMap 追加数据
  // {
  //     "trajectoryID": 0,
  // }
  int trajectoryID = map["trajectoryID"].toInt();  // 从数据映射中获取轨迹ID

  // 创建 ModelDataTrajectory 对象
  ModelDataTrajectory data(trajectoryID);

  insert(count(), data);  // 调用 insert 函数，在列表末尾插入数据
}

void TrajectoryListModel::appendDistance(int trajectoryID) {
  // 添加距离标签数据到模型中，接受一个 ModelDataDistance 类型的数据映射
  if (trajectoryID < 0 || trajectoryID >= mList.count()) {
    return;
  }

  mList[trajectoryID].appendDistance();
}

/* ---------------------------------- 删 ---------------------------------- */
void TrajectoryListModel::remove(int index) {
  // 移除指定索引位置的数据
  if (index < 0 || index >= mList.count()) {
    return;
  }

  beginRemoveRows(QModelIndex(), index, index);  // 通知视图即将移除行
  mList.removeAt(index);                         // 从列表中移除数据
  endRemoveRows();                               // 通知视图移除行完成
  emit countChanged(mList.count());              // 发出 countChanged 信号
}

void TrajectoryListModel::removeDistance(int trajectoryID, int pointID) {
  // 移除指定索引位置的距离标签数据
  if (trajectoryID < 0 || trajectoryID >= mList.count()) {
    return;
  }

  mList[trajectoryID].removeDistance(pointID);
}

void TrajectoryListModel::clear(int index) {
  // 清空指定索引位置的数据
  if (index < 0 || index >= mList.count()) {
    return;
  }

  mList[index].points()->clear();
  mList[index].distanceLab()->clear();
}

/* ---------------------------------- 查 ---------------------------------- */

int TrajectoryListModel::rowCount(const QModelIndex &parent) const {
  // 返回模型中的行数
  Q_UNUSED(parent);      // 忽略未使用的参数
  return mList.count();  // 返回列表中的项数
}

QVariant TrajectoryListModel::data(const QModelIndex &index, int role) const {
  // 返回指定索引和角色的数据
  if (index.row() < 0 || index.row() >= mList.count())  // 检查索引是否有效
    return QVariant();                        // 返回空 QVariant

  const ModelDataTrajectory &data = mList[index.row()];

  if (role == TrajectoryIDRole)  // 如果角色是 TrajectoryIDRole
    return data.trajectoryID();  // 返回轨迹ID
  else if (role == IsLoopRole)
    return data.isLoop();
  else if (role == PathRole)
    return data.path();

  return QVariant();  // 返回空 QVariant
}

bool TrajectoryListModel::modelData(int index,
                                    ModelDataTrajectory &ModelDataTrajectory) {
  // 获取指定索引位置的 ModelDataTrajectory
  if (index < 0 || index >= mList.count()) {
    return false;
  }

  ModelDataTrajectory = mList.at(index);
  return true;
}

PointsListModel *TrajectoryListModel::getPointsModel(int index) {
  // 获取指定索引位置的 PointsListModel
  return mList[index].points();
}

DistanceListModel *TrajectoryListModel::getDistanceModel(int index) {
  // 获取指定索引位置的 DistanceListModel
  return mList[index].distanceLab();
}

int TrajectoryListModel::count() const {
  // 返回模型中的数据数量
  return rowCount(QModelIndex());
}

QHash<int, QByteArray> TrajectoryListModel::roleNames() const {
  // 返回角色名称的哈希表(角色名称与角色标识的映射 - 易忘)
  QHash<int, QByteArray> roles;  // 创建角色名称的哈希表
  roles[TrajectoryIDRole] = "trajectoryID";
  roles[IsLoopRole] = "isLoop";  // 添加 IsLoopRole 对应的角色名称
  roles[PathRole] = "path";      // 添加 PathRole 对应的角色名称
  return roles;                  // 返回角色名称的哈希表
}

void TrajectoryListModel::printAllData() {
  // 打印所有数据
  QDebug info = qInfo();
  info << "TrajectoryListModel:\n";
  for (int i = 0; i < mList.count(); i++) {  // 遍历列表
    info << "[" << i << "] TrajectoryListModel:"
         << "Trajectory ID:" << mList[i].trajectoryID()
         << ", IsLoop:" << mList[i].isLoop() << ", Path:\n";
    // 循环打印 Path 中的数据
    for (int j = 0; j < mList[i].path().count(); j++) {
      QVariantMap point = mList[i].path().at(j).toMap();
      info << "\t[" << j << "] PathPoint("
           << "latitude:"
           << QString::number(point["latitude"].toDouble(), 'f', 6)
           << ", longitude:"
           << QString::number(point["longitude"].toDouble(), 'f', 6) << ")\n";
    }
    mList[i].points()->printAllData();
  }
}

/* ---------------------------------- 改 ---------------------------------- */
void ModelDataTrajectory::setLoop(bool loop) {
  // 设置轨迹是否闭环
  mIsLoop = loop;

  // 如果是闭环，添加第一个点到路径的最后位置
  if (loop) {
    // 如果点的数量小于等于0，直接返回
    if (mPoints->rowCount() <= 0) {
      return;
    }
    // 如果m_points中点的数量等于m_path的数量 - 1（闭环），
    // 直接返回(防止重复设置)
    if (mPoints->rowCount() == mPath.count() - 1) {
      return;
    }
    updatePath();
    // 第一个点和最后一个点之间的距离标签
    appendLoopDistance();
  } else {
    // 如果m_points中点的数量等于m_path的数量（开环），直接返回
    if (mPoints->rowCount() == mPath.count()) {
      return;
    }
    updatePath();
    // 删除最后一个点和第一个点之间的中间点
    removeDistance(mPoints->rowCount());
  }
}

/* ---------------------------------- 改 ---------------------------------- */
void TrajectoryListModel::setLoop(int index, bool isLoop) {
  // 设置对应的轨迹点是否闭环
  if (index < 0 || index >= mList.count()) {
    return;
  }

  mList[index].setLoop(isLoop);
  emit dataChanged(this->index(index), this->index(index), {PathRole});
}

void TrajectoryListModel::updatePath(int index) {
  // 更新对应轨迹的路径Path
  if (index < 0 || index >= mList.count()) {
    return;
  }

  mList[index].updatePath();

  // 数据模型绑定到视图，当数据改变时，发出信号，通知视图更新
  emit dataChanged(this->index(index), this->index(index), {PathRole});
}

void TrajectoryListModel::updateDistance(int trajectoryID, int pointID) {
  // 更新对应轨迹的距离标签
  if (trajectoryID < 0 || trajectoryID >= mList.count()) {
    return;
  }

  mList[trajectoryID].updateDistanceLab(pointID);
}
