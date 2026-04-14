#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow window;
  window.setWindowTitle("轨迹点模型示例");
  window.showMaximized();

  return app.exec();
}
