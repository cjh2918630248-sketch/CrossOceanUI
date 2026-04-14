#include "mqttwindow.h"

#include <QDateTime>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

MqttWindow::MqttWindow(QWidget *parent) : QMainWindow(parent) {
  mMqtt = new Mqtt(this);
  setupUi();
  setupConnections();

  mMqtt->setHostname(mHostnameEdit->text());
  mMqtt->setPort(mPortSpin->value());
  mMqtt->setUsername(mUsernameEdit->text());
  mMqtt->setPassword(mPasswordEdit->text());
  mMqtt->connect();
}

void MqttWindow::setupUi() {
  QWidget *central = new QWidget(this);
  setCentralWidget(central);

  QVBoxLayout *mainLayout = new QVBoxLayout(central);
  mainLayout->setContentsMargins(10, 10, 10, 10);
  mainLayout->setSpacing(8);

  // Connection group
  QGroupBox *connGroup = new QGroupBox("连接配置", central);
  QFormLayout *formLayout = new QFormLayout(connGroup);

  mHostnameEdit = new QLineEdit("139.196.203.208", connGroup);
  mPortSpin = new QSpinBox(connGroup);
  mPortSpin->setRange(1, 65535);
  mPortSpin->setValue(1883);
  mUsernameEdit = new QLineEdit("hzy", connGroup);
  mPasswordEdit = new QLineEdit("ipac_1234", connGroup);
  mPasswordEdit->setEchoMode(QLineEdit::Password);

  formLayout->addRow("Hostname:", mHostnameEdit);
  formLayout->addRow("Port:", mPortSpin);
  formLayout->addRow("Username:", mUsernameEdit);
  formLayout->addRow("Password:", mPasswordEdit);

  QHBoxLayout *statusLayout = new QHBoxLayout();
  mStatusLabel = new QLabel("Disconnected", connGroup);
  mStatusLabel->setStyleSheet("color: red; font-weight: bold;");
  mConnectBtn = new QPushButton("Connect", connGroup);
  mDisconnectBtn = new QPushButton("Disconnect", connGroup);
  mDisconnectBtn->setEnabled(false);
  statusLayout->addWidget(new QLabel("Status:", connGroup));
  statusLayout->addWidget(mStatusLabel);
  statusLayout->addStretch();
  statusLayout->addWidget(mConnectBtn);
  statusLayout->addWidget(mDisconnectBtn);
  formLayout->addRow(statusLayout);
  mainLayout->addWidget(connGroup);

  // Error label
  mErrorLabel = new QLabel(central);
  mErrorLabel->setStyleSheet("color: red;");
  mErrorLabel->setVisible(false);
  mErrorLabel->setWordWrap(true);
  mainLayout->addWidget(mErrorLabel);

  // Subscribe group
  QGroupBox *subGroup = new QGroupBox("订阅", central);
  QHBoxLayout *subLayout = new QHBoxLayout(subGroup);

  mSubTopicEdit = new QLineEdit(subGroup);
  mSubTopicEdit->setPlaceholderText("输入话题，例如 crossocean/ship/1/data");
  mSubscribeBtn = new QPushButton("订阅", subGroup);
  mSubscribeBtn->setEnabled(false);
  mUnsubscribeBtn = new QPushButton("取消订阅", subGroup);
  mUnsubscribeBtn->setEnabled(false);
  mTopicList = new QListWidget(subGroup);
  mTopicList->setMaximumHeight(80);
  mTopicList->setSelectionMode(QAbstractItemView::SingleSelection);

  QVBoxLayout *subInputLayout = new QVBoxLayout();
  QHBoxLayout *subBtnRow = new QHBoxLayout();
  subBtnRow->addWidget(mSubTopicEdit, 1);
  subBtnRow->addWidget(mSubscribeBtn);
  subBtnRow->addWidget(mUnsubscribeBtn);
  subInputLayout->addLayout(subBtnRow);
  subInputLayout->addWidget(mTopicList);
  subLayout->addLayout(subInputLayout);
  mainLayout->addWidget(subGroup);

  // Splitter: received log | publish
  QSplitter *splitter = new QSplitter(Qt::Vertical, central);

  QWidget *logWidget = new QWidget(splitter);
  QVBoxLayout *logLayout = new QVBoxLayout(logWidget);
  logLayout->setContentsMargins(0, 0, 0, 0);
  logLayout->addWidget(new QLabel("接收消息:"));
  mMessageLog = new QTextEdit(logWidget);
  mMessageLog->setReadOnly(true);
  mMessageLog->setPlaceholderText("等待消息...");
  logLayout->addWidget(mMessageLog);

  QWidget *pubWidget = new QWidget(splitter);
  QVBoxLayout *pubLayout = new QVBoxLayout(pubWidget);
  pubLayout->setContentsMargins(0, 0, 0, 0);
  pubLayout->addWidget(new QLabel("发布消息:"));
  QHBoxLayout *pubTopicRow = new QHBoxLayout();
  pubTopicRow->addWidget(new QLabel("Topic:"));
  mPubTopicEdit = new QLineEdit(pubWidget);
  mPubTopicEdit->setPlaceholderText("输入发布话题");
  pubTopicRow->addWidget(mPubTopicEdit, 1);
  pubLayout->addLayout(pubTopicRow);
  mPublishMessageText = new QTextEdit(pubWidget);
  mPublishMessageText->setPlaceholderText("输入消息内容");
  mPublishMessageText->setText("{\n  \"example\": \"data\",\n  \"value\": 123\n}");
  pubLayout->addWidget(mPublishMessageText, 1);
  QHBoxLayout *pubBtnRow = new QHBoxLayout();
  pubBtnRow->addStretch();
  mPublishBtn = new QPushButton("发布", pubWidget);
  mPublishBtn->setEnabled(false);
  pubBtnRow->addWidget(mPublishBtn);
  pubLayout->addLayout(pubBtnRow);

  splitter->addWidget(logWidget);
  splitter->addWidget(pubWidget);
  splitter->setStretchFactor(0, 2);
  splitter->setStretchFactor(1, 1);
  mainLayout->addWidget(splitter, 1);
}

void MqttWindow::setupConnections() {
  QObject::connect(mConnectBtn, &QPushButton::clicked, this,
                   &MqttWindow::onConnectClicked);
  QObject::connect(mDisconnectBtn, &QPushButton::clicked, this,
                   &MqttWindow::onDisconnectClicked);
  QObject::connect(mSubscribeBtn, &QPushButton::clicked, this,
                   &MqttWindow::onSubscribeClicked);
  QObject::connect(mUnsubscribeBtn, &QPushButton::clicked, this,
                   &MqttWindow::onUnsubscribeClicked);
  QObject::connect(mPublishBtn, &QPushButton::clicked, this,
                   &MqttWindow::onPublishClicked);

  QObject::connect(mMqtt, &Mqtt::connected, this, &MqttWindow::onConnected);
  QObject::connect(mMqtt, &Mqtt::disconnected, this,
                   &MqttWindow::onDisconnected);
  QObject::connect(mMqtt, &Mqtt::connectionStateChanged, this,
                   &MqttWindow::onConnectionStateChanged);
  QObject::connect(mMqtt, &Mqtt::messageReceived, this,
                   &MqttWindow::onMessageReceived);
  QObject::connect(mMqtt, &Mqtt::error, this, &MqttWindow::onError);
}

void MqttWindow::onConnectClicked() {
  mMqtt->setHostname(mHostnameEdit->text());
  mMqtt->setPort(mPortSpin->value());
  mMqtt->setUsername(mUsernameEdit->text());
  mMqtt->setPassword(mPasswordEdit->text());
  mMqtt->connect();
}

void MqttWindow::onDisconnectClicked() { mMqtt->disconnect(); }

void MqttWindow::onConnected() {
  mStatusLabel->setText("Connected");
  mStatusLabel->setStyleSheet("color: green; font-weight: bold;");
  mErrorLabel->setVisible(false);
  updateConnectionButtons(2);
}

void MqttWindow::onDisconnected() {
  mStatusLabel->setText("Disconnected");
  mStatusLabel->setStyleSheet("color: red; font-weight: bold;");
  updateConnectionButtons(0);
}

void MqttWindow::onConnectionStateChanged(int state) {
  updateConnectionButtons(state);
}

void MqttWindow::onSubscribeClicked() {
  QString topic = mSubTopicEdit->text().trimmed();
  if (topic.isEmpty()) return;
  if (mMqtt->addTopic(topic)) {
    if (mTopicList->findItems(topic, Qt::MatchExactly).isEmpty()) {
      mTopicList->addItem(topic);
    }
    mSubTopicEdit->clear();
  }
}

void MqttWindow::onUnsubscribeClicked() {
  QListWidgetItem *item = mTopicList->currentItem();
  if (!item) return;
  mMqtt->removeTopic(item->text());
  delete mTopicList->takeItem(mTopicList->row(item));
}

void MqttWindow::onMessageReceived(const QString &topic,
                                    const QByteArray &message) {
  appendLog(topic, QString::fromUtf8(message));
}

void MqttWindow::onError(const QString &errorMessage) {
  mErrorLabel->setText("Error: " + errorMessage);
  mErrorLabel->setVisible(true);
}

void MqttWindow::onPublishClicked() {
  QString topic = mPubTopicEdit->text().trimmed();
  if (topic.isEmpty()) return;
  mMqtt->publish(topic, mPublishMessageText->toPlainText().toUtf8());
}

void MqttWindow::updateConnectionButtons(int state) {
  mConnectBtn->setEnabled(state == 0);
  mDisconnectBtn->setEnabled(state == 2);
  mSubscribeBtn->setEnabled(state == 2);
  mUnsubscribeBtn->setEnabled(state == 2);
  mPublishBtn->setEnabled(state == 2);
}

void MqttWindow::appendLog(const QString &topic, const QString &message) {
  QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
  mMessageLog->append(QString("[%1] %2\n%3\n").arg(time, topic, message));
}
