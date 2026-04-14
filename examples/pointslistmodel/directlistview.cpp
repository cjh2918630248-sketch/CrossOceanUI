#include "directlistview.h"

#include <QLabel>
#include <QVBoxLayout>

DirectListView::DirectListView(PointsListModel *model, QWidget *parent)
    : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(new QLabel("直接绑定 PointsListModel 的列表视图:", this));

  mListView = new QListView(this);
  mListView->setModel(model);
  mListView->setAlternatingRowColors(true);
  layout->addWidget(mListView, 1);
}
