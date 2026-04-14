#ifndef DIRECTLISTVIEW_H
#define DIRECTLISTVIEW_H

#include <QListView>
#include <QWidget>

#include "pointslistmodel.h"

class DirectListView : public QWidget {
  Q_OBJECT

 public:
  explicit DirectListView(PointsListModel *model, QWidget *parent = nullptr);

 private:
  QListView *mListView;
};

#endif  // DIRECTLISTVIEW_H
