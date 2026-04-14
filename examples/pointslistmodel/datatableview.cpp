#include "datatableview.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QVBoxLayout>

// TrajectoryFilterProxy
TrajectoryFilterProxy::TrajectoryFilterProxy(QObject *parent)
    : QSortFilterProxyModel(parent) {}

void TrajectoryFilterProxy::setTrajectoryFilter(const QString &filter) {
  mFilter = filter.trimmed();
  invalidateFilter();
}

bool TrajectoryFilterProxy::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const {
  if (mFilter.isEmpty()) return true;

  bool ok = false;
  int filterID = mFilter.toInt(&ok);
  if (!ok) return true;

  QModelIndex idx = sourceModel()->index(
      sourceRow, 0, sourceParent);
  int tid = sourceModel()->data(idx, PointsListModel::TrajectoryIDRole).toInt();
  return tid == filterID;
}

// DataTableView
DataTableView::DataTableView(PointsListModel *model, QWidget *parent)
    : QWidget(parent), mModel(model) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  // Filter row
  QHBoxLayout *filterLayout = new QHBoxLayout();
  filterLayout->addWidget(new QLabel("轨迹ID过滤:"));
  mFilterEdit = new QLineEdit(this);
  mFilterEdit->setPlaceholderText("输入轨迹ID（留空显示全部）");
  mFilterEdit->setMaximumWidth(250);
  filterLayout->addWidget(mFilterEdit);
  filterLayout->addStretch();
  mCountLabel = new QLabel(this);
  filterLayout->addWidget(mCountLabel);
  layout->addLayout(filterLayout);

  // Table view
  mProxy = new TrajectoryFilterProxy(this);
  mProxy->setSourceModel(model);

  mTableView = new QTableView(this);
  mTableView->setModel(mProxy);
  mTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  mTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  mTableView->setAlternatingRowColors(true);
  layout->addWidget(mTableView, 1);

  // Update count label
  auto updateCount = [this]() {
    mCountLabel->setText(
        QString("显示 %1 行").arg(mProxy->rowCount()));
  };
  QObject::connect(model, &PointsListModel::pointUpdated, this, updateCount);
  QObject::connect(mProxy, &QSortFilterProxyModel::rowsInserted, this,
                   updateCount);
  QObject::connect(mProxy, &QSortFilterProxyModel::rowsRemoved, this,
                   updateCount);
  updateCount();

  QObject::connect(mFilterEdit, &QLineEdit::textChanged, mProxy,
                   &TrajectoryFilterProxy::setTrajectoryFilter);
}
