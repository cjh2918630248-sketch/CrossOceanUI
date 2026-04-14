#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QTabWidget>
#include <QTimer>

#include "datagenerator.h"
#include "pointslistmodel.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);

 private slots:
  void onTimerTriggered();
  void onPointUpdated();

 private:
  void setupUi();
  void setupConnections();

  PointsListModel *mModel;
  DataGenerator *mDataGenerator;
  QTimer *mTimer;

  QTabWidget *mTabWidget;
  QLabel *mCountLabel;
  bool mTimerRunning = true;
};

#endif  // MAINWINDOW_H
