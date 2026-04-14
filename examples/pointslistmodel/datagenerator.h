#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QObject>
#include <QRandomGenerator>
#include <limits>

#include "pointslistmodel.h"

class DataGenerator : public QObject {
  Q_OBJECT

 public:
  explicit DataGenerator(QObject *parent = nullptr) : QObject(parent) {}

  // 初始化模型数据
  Q_INVOKABLE void initializeModel(PointsListModel *model) {
    if (!model) return;

    // 清空现有数据
    model->clear();

    // 添加初始测试轨迹
    addTrajectory(model, 0, 31.2, 121.4, 10);  // 上海附近轨迹
    addTrajectory(model, 1, 39.9, 116.4, 8);   // 北京附近轨迹
    addTrajectory(model, 2, 23.1, 113.3, 12);  // 广州附近轨迹
  }

  // 添加新的轨迹点
  Q_INVOKABLE void appendRandomPoint(PointsListModel *model, int trajectoryID) {
    if (!model) return;

    // 查找该轨迹ID的最后一个点
    int lastPointIndex = -1;
    double lastLat = 0.0;
    double lastLon = 0.0;

    for (int i = 0; i < model->rowCount(); i++) {
      QVariantMap point = model->get(i);
      if (point["trajectoryID"].toInt() == trajectoryID) {
        lastPointIndex = i;
        lastLat = point["latitude"].toDouble();
        lastLon = point["longitude"].toDouble();
      }
    }

    if (lastPointIndex == -1) {
      // 如果没有找到该轨迹，创建一个新的起始点
      QRandomGenerator *rng = QRandomGenerator::global();
      double newLat = 30.0 + rng->bounded(10.0);
      double newLon = 120.0 + rng->bounded(10.0);

      QVariantMap newPoint;
      newPoint["trajectoryID"] = trajectoryID;
      newPoint["latitude"] = newLat;
      newPoint["longitude"] = newLon;
      model->append(newPoint);
    } else {
      // 在最后一个点的基础上添加一个随机偏移的新点
      QRandomGenerator *rng = QRandomGenerator::global();
      // 生成-5到5之间的随机偏移
      double latOffset = (rng->bounded(100) - 50) / 10.0;
      double lonOffset = (rng->bounded(100) - 50) / 10.0;

      QVariantMap newPoint;
      newPoint["trajectoryID"] = trajectoryID;
      newPoint["latitude"] = lastLat + latOffset;
      newPoint["longitude"] = lastLon + lonOffset;
      model->append(newPoint);
    }
  }

  // 生成并添加测试数据
  Q_INVOKABLE void updateRandomPoint(PointsListModel *model) {
    if (!model) return;

    if (model->rowCount() > 0) {
      QRandomGenerator *rng = QRandomGenerator::global();
      int randomIndex = rng->bounded(model->rowCount());

      QVariantMap point = model->get(randomIndex);
      double lat = point["latitude"].toDouble();
      double lon = point["longitude"].toDouble();

      // 生成-2到2之间的随机偏移
      double latOffset = (rng->bounded(40) - 20) / 10.0;
      double lonOffset = (rng->bounded(40) - 20) / 10.0;

      // 更新点位置
      model->set(randomIndex, lat + latOffset, lon + lonOffset);
    }
  }

  // 生成错误测试数据
  Q_INVOKABLE void generateErrorCase(PointsListModel *model, int errorCase) {
    if (!model) return;

    switch (errorCase) {
      case 0:  // 无效索引错误
        model->get(-1);
        break;
      case 1:  // 无效数据错误 - 缺少字段
      {
        QVariantMap invalidMap;
        // 缺少必要字段
        invalidMap["trajectoryID"] = 1;
        // 缺少latitude和longitude
        model->append(invalidMap);
      } break;
      case 2:  // 超出范围错误
      {
        QVariantMap outOfRangeMap;
        outOfRangeMap["trajectoryID"] = 1;
        outOfRangeMap["latitude"] = 91.5;  // 超出纬度范围
        outOfRangeMap["longitude"] = 120.0;
        model->append(outOfRangeMap);
      } break;
      case 3:  // 类型转换错误 - 轨迹ID
      {
        QVariantMap invalidTypeMap;
        invalidTypeMap["trajectoryID"] = "not_a_number";  // 字符串而非数字
        invalidTypeMap["latitude"] = 30.0;
        invalidTypeMap["longitude"] = 120.0;
        model->append(invalidTypeMap);
      } break;
      case 4:  // 类型转换错误 - 纬度
      {
        QVariantMap invalidTypeMap;
        invalidTypeMap["trajectoryID"] = 1;
        invalidTypeMap["latitude"] = "invalid_latitude";  // 字符串而非数字
        invalidTypeMap["longitude"] = 120.0;
        model->append(invalidTypeMap);
      } break;
      case 5:  // NaN值测试
      {
        if (model->rowCount() > 0) {
          model->set(0, std::numeric_limits<double>::quiet_NaN(), 120.0);
        } else {
          QVariantMap nanMap;
          nanMap["trajectoryID"] = 1;
          nanMap["latitude"] = std::numeric_limits<double>::quiet_NaN();
          nanMap["longitude"] = 120.0;
          model->append(nanMap);
        }
      } break;
      default:  // 无效索引
        model->remove(model->rowCount() + 100);
        break;
    }
  }

 private:
  // 添加一条轨迹，生成多个连续点
  void addTrajectory(PointsListModel *model, int trajectoryID, double startLat,
                     double startLon, int pointCount) {
    QRandomGenerator *rng = QRandomGenerator::global();

    double lat = startLat;
    double lon = startLon;

    for (int i = 0; i < pointCount; i++) {
      QVariantMap point;
      point["trajectoryID"] = trajectoryID;
      point["latitude"] = lat;
      point["longitude"] = lon;
      model->append(point);

      // 生成下一个点的随机偏移
      double latOffset = (rng->bounded(100) - 50) / 10.0;
      double lonOffset = (rng->bounded(100) - 50) / 10.0;

      lat += latOffset;
      lon += lonOffset;
    }
  }
};

#endif  // DATAGENERATOR_H
