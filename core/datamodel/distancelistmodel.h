#ifndef DISTANCELISTMODEL_H
#define DISTANCELISTMODEL_H

#include <QAbstractListModel>

// 距离标签数据
class ModelDataDistance;

class DistanceListModel : public QAbstractListModel {
  Q_OBJECT
 public:
  // 构造函数，接受一个父对象指针，默认为空指针
  explicit DistanceListModel(QObject *parent = nullptr);
  // 构造函数，传入轨迹ID
  explicit DistanceListModel(int trajectoryID, QObject *parent = nullptr)
      : QAbstractListModel(parent), mTrajectoryID(trajectoryID) {}

  // 定义枚举类型 DisplayRoles，用于角色标识
  enum DisplayRoles {
    MiddlePointRole = Qt::UserRole + 1,  // 自定义角色，从 Qt::UserRole + 1 开始
    DistanceRole,
    TrajectoryIDRole
  };

  // QML注册方法 - 已移除

  /* 增 */
  // 插入数据到指定索引位置
  void insert(int index, const ModelDataDistance &data);
  // 追加数据到列表末尾
  void append(const ModelDataDistance &data);

  /* 删 */
  // 移除指定索引位置的数据
  void remove(int index);
  // 清空所有数据
  void clear();

  /* 查 */
  // 重载的虚函数，返回模型中的行数
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  // 重载的虚函数，返回指定索引和角色的数据
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  // 打印所有数据
  Q_INVOKABLE void printAllData();
  // 获取指定索引位置的 ModelData
  bool modelData(int index, ModelDataDistance &ModelDataTrajectory);

  /* 改 */
  // 重新加载数据
  void reload();
  // 更新对应轨迹点的距离标签
  void set(int index, const ModelDataDistance &data);

 signals:
  // 定义信号，当数据更新时发出
  void pointUpdated();

 private:
  // 返回模型中的数据数量
  int count() const;

 protected:
  // 重载的虚函数，返回角色名称的哈希表
  QHash<int, QByteArray> roleNames() const override;

 private:
  int mTrajectoryID;               // 轨迹ID
  QList<ModelDataDistance> mList;  // 存储 ModelDataTrajectory 对象的列表
};

// 定义 ModelDataPoint 类
class ModelDataDistance {
 public:
  // 默认构造函数
  ModelDataDistance() {}
  // 构造函数，接受
  ModelDataDistance(const QVariantMap &middlePoint, double distance)
      : mMiddlePoint(middlePoint), mDistance(distance) {}

  /* 查 */
  // 返回中间点
  QVariantMap middlePoint() const { return mMiddlePoint; }
  // 返回 距离
  double distance() const { return mDistance; }
  friend QDebug operator<<(QDebug dbg, const ModelDataDistance &point);

  /* 改 */
  // 设置中间点
  void setMiddlePoint(const QVariantMap &middlePoint) {
    mMiddlePoint = middlePoint;
  }
  // 设置距离
  void setDistance(double distance) { mDistance = distance; }

 private:
  QVariantMap mMiddlePoint;  // 中间点
  double mDistance;          // 距离
};

#endif  // DISTANCELISTMODEL_H
