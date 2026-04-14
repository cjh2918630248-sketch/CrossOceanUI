#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>

#include "pointslistmodel.h"

class MapView : public QWidget {
  Q_OBJECT

 public:
  explicit MapView(PointsListModel *model, QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;

 private:
  PointsListModel *mModel;

  // Map transform helpers
  double lonToX(double lon, double minLon, double maxLon, double w) const;
  double latToY(double lat, double minLat, double maxLat, double h) const;
};

#endif  // MAPVIEW_H
