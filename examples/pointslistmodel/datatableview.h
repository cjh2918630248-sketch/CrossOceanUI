#ifndef DATATABLEVIEW_H
#define DATATABLEVIEW_H

#include <QLabel>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QWidget>

#include "pointslistmodel.h"

// Proxy model that filters by trajectoryID column
class TrajectoryFilterProxy : public QSortFilterProxyModel {
  Q_OBJECT
 public:
  explicit TrajectoryFilterProxy(QObject *parent = nullptr);
  void setTrajectoryFilter(const QString &filter);

 protected:
  bool filterAcceptsRow(int sourceRow,
                        const QModelIndex &sourceParent) const override;

 private:
  QString mFilter;
};

class DataTableView : public QWidget {
  Q_OBJECT

 public:
  explicit DataTableView(PointsListModel *model, QWidget *parent = nullptr);

 private:
  QLineEdit *mFilterEdit;
  QTableView *mTableView;
  TrajectoryFilterProxy *mProxy;
  QLabel *mCountLabel;

  PointsListModel *mModel;
};

#endif  // DATATABLEVIEW_H
