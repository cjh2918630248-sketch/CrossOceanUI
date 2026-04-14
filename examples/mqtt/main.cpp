#include <QApplication>

#include "mqttwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MqttWindow window;
  window.setWindowTitle("MQTT Client Example");
  window.resize(800, 600);
  window.show();

  return app.exec();
}
