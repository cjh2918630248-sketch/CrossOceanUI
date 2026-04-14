#include "mapview.h"

#include <QPainter>
#include <QVariantMap>
#include <limits>

static const QList<QColor> kTrajectoryColors = {
    QColor(255, 80, 80),    // 红
    QColor(80, 160, 255),   // 蓝
    QColor(80, 200, 120),   // 绿
    QColor(255, 200, 50),   // 黄
    QColor(200, 80, 255),   // 紫
};

MapView::MapView(PointsListModel *model, QWidget *parent)
    : QWidget(parent), mModel(model) {
  setMinimumSize(300, 200);
  QObject::connect(model, &PointsListModel::pointUpdated, this,
                   QOverload<>::of(&QWidget::update));
}

void MapView::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(rect(), QColor(30, 30, 40));

  int count = mModel->rowCount();
  if (count == 0) return;

  // Collect bounds
  double minLat = std::numeric_limits<double>::max();
  double maxLat = std::numeric_limits<double>::lowest();
  double minLon = std::numeric_limits<double>::max();
  double maxLon = std::numeric_limits<double>::lowest();

  for (int i = 0; i < count; ++i) {
    QVariantMap pt = mModel->get(i);
    double lat = pt["latitude"].toDouble();
    double lon = pt["longitude"].toDouble();
    if (lat < minLat) minLat = lat;
    if (lat > maxLat) maxLat = lat;
    if (lon < minLon) minLon = lon;
    if (lon > maxLon) maxLon = lon;
  }

  const double margin = 30.0;
  double drawW = width() - 2 * margin;
  double drawH = height() - 2 * margin;

  // Guard against zero range
  double latRange = (maxLat == minLat) ? 1.0 : (maxLat - minLat);
  double lonRange = (maxLon == minLon) ? 1.0 : (maxLon - minLon);

  // Group points by trajectoryID
  QMap<int, QList<QPointF>> trajectories;
  for (int i = 0; i < count; ++i) {
    QVariantMap pt = mModel->get(i);
    int tid = pt["trajectoryID"].toInt();
    double lat = pt["latitude"].toDouble();
    double lon = pt["longitude"].toDouble();

    double x = margin + (lon - minLon) / lonRange * drawW;
    double y = margin + (maxLat - lat) / latRange * drawH;
    trajectories[tid].append(QPointF(x, y));
  }

  // Draw each trajectory
  int colorIdx = 0;
  for (auto it = trajectories.begin(); it != trajectories.end(); ++it, ++colorIdx) {
    QColor color = kTrajectoryColors[colorIdx % kTrajectoryColors.size()];
    const QList<QPointF> &points = it.value();

    // Draw lines
    if (points.size() > 1) {
      QPen linePen(color, 2);
      painter.setPen(linePen);
      for (int i = 0; i < points.size() - 1; ++i) {
        painter.drawLine(points[i], points[i + 1]);
      }
    }

    // Draw points
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    for (const QPointF &p : points) {
      painter.drawEllipse(p, 4, 4);
    }

    // Draw trajectory ID label at first point
    if (!points.isEmpty()) {
      painter.setPen(color);
      painter.drawText(points[0] + QPointF(6, -4),
                       QString("T%1").arg(it.key()));
    }
  }
}

double MapView::lonToX(double lon, double minLon, double maxLon,
                        double w) const {
  return (maxLon == minLon) ? w / 2.0
                             : (lon - minLon) / (maxLon - minLon) * w;
}

double MapView::latToY(double lat, double minLat, double maxLat,
                        double h) const {
  return (maxLat == minLat) ? h / 2.0
                             : (maxLat - lat) / (maxLat - minLat) * h;
}
