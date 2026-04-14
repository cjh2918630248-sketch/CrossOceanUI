#ifndef MQTTWINDOW_H
#define MQTTWINDOW_H

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>

#include "mqtt.h"

class MqttWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MqttWindow(QWidget *parent = nullptr);

 private slots:
  void onConnected();
  void onDisconnected();
  void onConnectionStateChanged(int state);
  void onMessageReceived(const QString &topic, const QByteArray &message);
  void onError(const QString &errorMessage);
  void onConnectClicked();
  void onDisconnectClicked();
  void onSubscribeClicked();
  void onUnsubscribeClicked();
  void onPublishClicked();

 private:
  void setupUi();
  void setupConnections();
  void updateConnectionButtons(int state);
  void appendLog(const QString &topic, const QString &message);

  Mqtt *mMqtt;

  // Connection controls
  QLineEdit *mHostnameEdit;
  QSpinBox *mPortSpin;
  QLineEdit *mUsernameEdit;
  QLineEdit *mPasswordEdit;
  QLabel *mStatusLabel;
  QPushButton *mConnectBtn;
  QPushButton *mDisconnectBtn;
  QLabel *mErrorLabel;

  // Subscribe controls
  QLineEdit *mSubTopicEdit;
  QPushButton *mSubscribeBtn;
  QPushButton *mUnsubscribeBtn;
  QListWidget *mTopicList;

  // Received messages log
  QTextEdit *mMessageLog;

  // Publish controls
  QLineEdit *mPubTopicEdit;
  QTextEdit *mPublishMessageText;
  QPushButton *mPublishBtn;
};

#endif  // MQTTWINDOW_H
