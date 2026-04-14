#include "mainwindow.h"

#include <QAction>
#include <QRandomGenerator>
#include <QStatusBar>
#include <QToolBar>

#include "datatableview.h"
#include "directlistview.h"
#include "mapview.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  mModel = new PointsListModel(this);
  mDataGenerator = new DataGenerator(this);
  mTimer = new QTimer(this);
  mTimer->setInterval(1000);

  setupUi();
  setupConnections();

  mDataGenerator->initializeModel(mModel);
  mTimer->start();
}

void MainWindow::setupUi() {
  // Toolbar
  QToolBar *toolbar = addToolBar("Controls");
  toolbar->setMovable(false);

  QAction *addT0 = toolbar->addAction("添加轨迹0点");
  QAction *addT1 = toolbar->addAction("添加轨迹1点");
  QAction *addT2 = toolbar->addAction("添加轨迹2点");
  toolbar->addSeparator();
  QAction *toggleTimer = toolbar->addAction("暂停更新");
  QAction *resetAction = toolbar->addAction("重置数据");
  toolbar->addSeparator();
  mCountLabel = new QLabel("点数量: 0", this);
  toolbar->addWidget(mCountLabel);

  QObject::connect(addT0, &QAction::triggered, this,
                   [this]() { mDataGenerator->appendRandomPoint(mModel, 0); });
  QObject::connect(addT1, &QAction::triggered, this,
                   [this]() { mDataGenerator->appendRandomPoint(mModel, 1); });
  QObject::connect(addT2, &QAction::triggered, this,
                   [this]() { mDataGenerator->appendRandomPoint(mModel, 2); });
  QObject::connect(toggleTimer, &QAction::triggered, this,
                   [this, toggleTimer]() {
                     mTimerRunning = !mTimerRunning;
                     if (mTimerRunning) {
                       mTimer->start();
                       toggleTimer->setText("暂停更新");
                     } else {
                       mTimer->stop();
                       toggleTimer->setText("继续更新");
                     }
                   });
  QObject::connect(resetAction, &QAction::triggered, this, [this]() {
    mDataGenerator->initializeModel(mModel);
  });

  // Tab widget
  mTabWidget = new QTabWidget(this);
  mTabWidget->addTab(new MapView(mModel, this), "地图视图");
  mTabWidget->addTab(new DataTableView(mModel, this), "数据表格");
  mTabWidget->addTab(new DirectListView(mModel, this), "直接列表");
  setCentralWidget(mTabWidget);

  statusBar()->showMessage("就绪");
}

void MainWindow::setupConnections() {
  QObject::connect(mTimer, &QTimer::timeout, this,
                   &MainWindow::onTimerTriggered);
  QObject::connect(mModel, &PointsListModel::pointUpdated, this,
                   &MainWindow::onPointUpdated);
  QObject::connect(mModel, &PointsListModel::errorOccurred, this,
                   [this](int, const QString &msg) {
                     statusBar()->showMessage("错误: " + msg, 3000);
                   });
}

void MainWindow::onTimerTriggered() {
  mDataGenerator->updateRandomPoint(mModel);

  if (QRandomGenerator::global()->bounded(10) < 3) {
    int tid = QRandomGenerator::global()->bounded(3);
    mDataGenerator->appendRandomPoint(mModel, tid);
  }
}

void MainWindow::onPointUpdated() {
  mCountLabel->setText(QString("点数量: %1").arg(mModel->rowCount()));
}
