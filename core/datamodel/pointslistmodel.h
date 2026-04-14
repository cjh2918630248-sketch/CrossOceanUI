#ifndef POINTSLISTMODEL_H
#define POINTSLISTMODEL_H

#include <QAbstractListModel>

// 轨迹点数据模型
class ModelDataPoint;

// 定义 PointsListModel 类，继承自 QAbstractListModel
class PointsListModel : public QAbstractListModel {
  Q_OBJECT
 public:
  // 构造函数，接受一个父对象指针，默认为空指针
  explicit PointsListModel(QObject *parent = nullptr);

  // 定义枚举类型 DisplayRoles，用于角色标识
  enum DisplayRoles {
    TrajectoryIDRole =
        Qt::UserRole + 1,  // 自定义角色，从 Qt::UserRole + 1 开始
    PointIDRole,
    LatitudeRole,
    LongitudeRole
  };

  // 定义错误类型枚举
  enum ErrorType {
    NoError = 0,     // 无错误
    InvalidIndex,    // 无效索引
    InvalidData,     // 无效数据
    OutOfRange,      // 超出范围
    DuplicatePoint,  // 重复点
    UnknownError     // 未知错误
  };
  Q_ENUM(ErrorType)  // 注册枚举以便在QML中使用

  // QML注册方法 - 已移除

  /* 增 */
  // 插入数据到指定索引位置
  Q_INVOKABLE void insert(int index, const ModelDataPoint &data);
  // 追加数据到列表末尾
  Q_INVOKABLE void append(const ModelDataPoint &data);
  Q_INVOKABLE void append(const QVariantMap map);

  /* 删 */
  // 移除指定索引位置的数据
  Q_INVOKABLE void remove(int index);
  // 清空所有数据
  Q_INVOKABLE void clear();

  /* 查 */
  // 重载的虚函数，返回指定索引和角色的数据
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  // QML 获取对应点的数据
  Q_INVOKABLE QVariantMap get(int index);
  // 获取指定索引位置的 ModelData
  bool modelData(int index, ModelDataPoint &ModelDataPoint);
  // 打印所有数据
  Q_INVOKABLE void printAllData();
  // 重载的虚函数，返回模型中的行数
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  /* 改 */
  // 修改指定索引位置的数据
  Q_INVOKABLE void set(int index, const double latitude,
                       const double longitude);

 signals:
  // 定义信号，当数据更新时发出
  void pointUpdated();
  // 定义信号，当轨迹点添加时发出
  void pointAdded(int trajectoryID, int pointID);
  // 定义信号，当轨迹点删除时发出
  void pointRemoved(int trajectoryID, int pointID);
  // 定义信号，当轨迹点修改时发出
  void pointChanged(int trajectoryID, int pointID);
  // 定义信号，当发生错误时发出
  void errorOccurred(int errorType, const QString &errorMessage);

 private:
  // 返回模型中的数据数量
  int count() const;

 protected:
  // 重载的虚函数，返回角色名称的哈希表
  QHash<int, QByteArray> roleNames() const override;

 private:
  QList<ModelDataPoint> mList;  // 存储 ModelDataTrajectory 对象的列表
};

// 定义 ModelDataPoint 类
class ModelDataPoint {
 public:
  // 默认构造函数
  ModelDataPoint() {}
  // 构造函数，接受
  ModelDataPoint(int trajectoryID, int pointID, double latitude,
                 double longitude)
      : mTrajectoryID(trajectoryID),
        mPointID(pointID),
        mLatitude(latitude),
        mLongitude(longitude) {}

  /* 查 */
  // 返回 trajectoryID
  int trajectoryID() const { return mTrajectoryID; }
  // 返回 pointID
  int pointID() const { return mPointID; }
  // 返回 纬度
  double latitude() const { return mLatitude; }
  // 返回 经度
  double longitude() const { return mLongitude; }
  friend QDebug operator<<(QDebug dbg, const ModelDataPoint &point);

  /* 改 */
  // 设置点的ID
  void setPointID(int pointID) { mPointID = pointID; }
  // 设置纬度
  void setLatitude(double latitude) { mLatitude = latitude; }
  // 设置经度
  void setLongitude(double longitude) { mLongitude = longitude; }

 private:
  int mTrajectoryID;  // 轨迹ID
  int mPointID;       // 轨迹点ID
  double mLatitude;   // 纬度
  double mLongitude;  // 经度
};

#endif  // PointsListModel_H
