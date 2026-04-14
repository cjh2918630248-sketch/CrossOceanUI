#ifndef TRAJECTORYLISTMODEL_H
#define TRAJECTORYLISTMODEL_H

#include <QAbstractListModel>
#include <QMetaType>

#include "distancelistmodel.h"
#include "pointslistmodel.h"

// 定义 ModelTrajectoryData 类
class ModelDataTrajectory {
 public:
  // 默认构造函数
  ModelDataTrajectory() {}
  // 构造函数，接受一个轨迹 ID 和一个轨迹点列表
  ModelDataTrajectory(int trajectoryID) : mTrajectoryID(trajectoryID) {
    mPoints = new PointsListModel();
    mDistanceLab = new DistanceListModel(trajectoryID);
  }

  /* 增 */
  // 插入距离标签数据到模型中
  void insertDistance(int index);
  // 添加距离标签数据到模型中
  void appendDistance();
  // 添加闭环距离标签数据到模型中
  void appendLoopDistance();

  /* 删 */
  // 移除指定索引位置的距离标签数据
  void removeDistance(int index);

  /* 查 */
  // 返回轨迹 ID
  int trajectoryID() const { return mTrajectoryID; }
  // 返回是否闭环
  bool isLoop() const { return mIsLoop; }
  // 返回轨迹路径
  QVariantList path() const { return mPath; }
  // 返回轨迹距离
  DistanceListModel *distanceLab() const { return mDistanceLab; }
  // 返回轨迹点列表
  PointsListModel *points() const { return mPoints; }
  // 打印所有数据
  friend QDebug operator<<(QDebug dbg, const ModelDataTrajectory &data);

  /* 改 */
  // 更新Path的轨迹点
  void updatePath();
  // 设置轨迹是否闭环
  void setLoop(bool loop);
  // 更新对应的距离标签数据
  void updateDistanceLab(int index);

 private:
  int mTrajectoryID;                // 轨迹ID
  bool mIsLoop;                     // 是否闭环
  QVariantList mPath;               // 轨迹路径
  DistanceListModel *mDistanceLab;  // 轨迹距离
  PointsListModel *mPoints;         // 轨迹点列表
};

// 定义 TrajectoryListModel 类，继承自 QAbstractListModel
class TrajectoryListModel : public QAbstractListModel {
  Q_OBJECT
 public:
  // 构造函数，接受一个父对象指针，默认为空指针
  explicit TrajectoryListModel(QObject *parent = nullptr);

  // 定义枚举类型 DisplayRoles，用于角色标识
  enum DisplayRoles {
    TrajectoryIDRole =
        Qt::UserRole + 1,  // 自定义角色，从 Qt::UserRole + 1 开始
    IsLoopRole,
    PathRole
  };

  // QML注册方法 - 已移除

  /* 增 */
  // 插入数据到指定索引位置
  Q_INVOKABLE void insert(int index, const ModelDataTrajectory &data);
  // 插入距离标签数据到模型中，接受一个 index 参数
  Q_INVOKABLE void insertDistance(int trajectoryID, int pointID);
  // 追加数据到列表末尾
  Q_INVOKABLE void append(const ModelDataTrajectory &data);
  // 添加数据到模型中，接受一个 QVariantMap 类型的数据映射
  Q_INVOKABLE void append(const QVariantMap map);
  // 追加距离标签数据到模型中
  Q_INVOKABLE void appendDistance(int trajectoryID);

  /* 删 */
  // 移除指定索引位置的数据
  Q_INVOKABLE void remove(int index);
  // 移除指定索引位置的距离标签数据
  Q_INVOKABLE void removeDistance(int trajectoryID, int pointID);
  // 清空指定索引位置的数据
  Q_INVOKABLE void clear(int index);

  /* 查 */
  // 重载的虚函数，返回模型中的行数
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  // 重载的虚函数，返回指定索引和角色的数据
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  // 打印所有数据
  Q_INVOKABLE void printAllData();
  // 获取指定索引位置的 ModelData (撤销操作需要)
  bool modelData(int index, ModelDataTrajectory &ModelDataTrajectory);
  // 获取指定索引位置的 PointsListModel
  Q_INVOKABLE PointsListModel *getPointsModel(int index);
  // 获取指定索引位置的 DistanceListModel
  Q_INVOKABLE DistanceListModel *getDistanceModel(int index);

  /* 改 */
  // 设置对应的轨迹点是否闭环
  Q_INVOKABLE void setLoop(int index, bool isLoop);
  // 更新对应轨迹的路径Path
  Q_INVOKABLE void updatePath(int index);
  // 更新对应的距离标签
  Q_INVOKABLE void updateDistance(int trajectoryID, int pointID);

 signals:
  // 定义信号，当计数改变时发出
  void countChanged(int arg);

 private:
  // 返回模型中的数据数量
  int count() const;

 protected:
  // 重载的虚函数，返回角色名称的哈希表
  QHash<int, QByteArray> roleNames() const override;

 private:
  QList<ModelDataTrajectory> mList;  // 存储 ModelDataTrajectory 对象的列表
};

#endif  // TRAJECTORYLISTMODEL_H
