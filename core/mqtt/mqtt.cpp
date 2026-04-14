/***************************************************************************
  mqtt.cpp - Mqtt

 ---------------------
 begin                : 21.3.2025
 copyright            : (C) 2025 by Jinghui Liang
 email                : 3414467112@qq.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "mqtt.h"

Mqtt::Mqtt(QObject *parent) : QObject(parent) { setupMqttClient(); }

Mqtt::~Mqtt() {
  if (mMqttClient) {
    if (mMqttClient->state() == QMqttClient::Connected) {
      mMqttClient->disconnect();
    }
    delete mMqttClient;
  }

  // 直接迭代容器，避免创建临时副本
  for (auto it = mSubscriptions.begin(); it != mSubscriptions.end(); ++it) {
    delete it.value();
  }
  mSubscriptions.clear();
}

void Mqtt::setupMqttClient() {
  if (!mMqttClient) {
    mMqttClient = new QMqttClient(this);

    QObject::connect(mMqttClient, &QMqttClient::stateChanged, this,
                     &Mqtt::onStateChanged);
    QObject::connect(mMqttClient, &QMqttClient::messageReceived, this,
                     &Mqtt::onMessageReceived);
    QObject::connect(
        mMqttClient, &QMqttClient::errorChanged, this,
        [this](QMqttClient::ClientError error) { onError(error); });
  }
}

// MQTT Connection Properties
QString Mqtt::hostname() const {
  return mMqttClient ? mMqttClient->hostname() : QString();
}

void Mqtt::setHostname(const QString &hostname) {
  if (mMqttClient && mMqttClient->hostname() != hostname) {
    mMqttClient->setHostname(hostname);
    emit hostnameChanged(hostname);
  }
}

int Mqtt::port() const { return mMqttClient ? mMqttClient->port() : 0; }

void Mqtt::setPort(int port) {
  if (mMqttClient && mMqttClient->port() != port) {
    mMqttClient->setPort(port);
    emit portChanged(port);
  }
}

QString Mqtt::username() const {
  return mMqttClient ? mMqttClient->username() : QString();
}

void Mqtt::setUsername(const QString &username) {
  if (mMqttClient && mMqttClient->username() != username) {
    mMqttClient->setUsername(username);
    emit usernameChanged(username);
  }
}

QString Mqtt::password() const {
  return mMqttClient ? mMqttClient->password() : QString();
}

void Mqtt::setPassword(const QString &password) {
  if (mMqttClient && mMqttClient->password() != password) {
    mMqttClient->setPassword(password);
    emit passwordChanged(password);
  }
}

int Mqtt::connectionState() const {
  return mMqttClient ? static_cast<int>(mMqttClient->state()) : 0;
}

// Topic management
bool Mqtt::addTopic(const QString &topicName) {
  if (topicName.isEmpty()) return false;

  if (!mTopics.contains(topicName)) mTopics.append(topicName);

  if (mMqttClient && mMqttClient->state() == QMqttClient::Connected) {
    auto subscription = mMqttClient->subscribe(topicName);
    if (subscription) {
      mSubscriptions[topicName] = subscription;
      return true;
    } else {
      mTopics.removeOne(topicName);
      emit error(QString("Failed to subscribe to topic: %1").arg(topicName));
      return false;
    }
  }

  return true;
}

bool Mqtt::removeTopic(const QString &topicName) {
  if (!mTopics.contains(topicName)) return false;

  mTopics.removeOne(topicName);

  if (mMqttClient && mSubscriptions.contains(topicName)) {
    auto subscription = mSubscriptions.take(topicName);
    if (subscription) {
      subscription->unsubscribe();
      delete subscription;
    }
  }

  return true;
}

QStringList Mqtt::topics() const { return mTopics; }

// Connection management
void Mqtt::connect() {
  if (!mMqttClient) return;

  if (mMqttClient->state() == QMqttClient::Disconnected) {
    mMqttClient->connectToHost();
  }
}

void Mqtt::disconnect() {
  if (mMqttClient && mMqttClient->state() != QMqttClient::Disconnected) {
    mMqttClient->disconnectFromHost();
  }
}

// Data publishing
bool Mqtt::publish(const QString &topic, const QByteArray &message, int qos) {
  if (!mMqttClient || mMqttClient->state() != QMqttClient::Connected)
    return false;

  qint32 result = mMqttClient->publish(topic, message, qos);
  if (result == -1) {
    emit error(QString("Failed to publish message to topic: %1").arg(topic));
    return false;
  }
  return true;
}

bool Mqtt::publish(const QString &topic, const QVariantMap &message, int qos) {
  QJsonDocument doc = QJsonDocument::fromVariant(message);
  QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
  return publish(topic, jsonData, qos);
}

// Slots
void Mqtt::onStateChanged(QMqttClient::ClientState state) {
  emit connectionStateChanged(static_cast<int>(state));

  if (state == QMqttClient::Connected) {
    emit connected();
  } else if (state == QMqttClient::Disconnected) {
    emit disconnected();
  }
}

void Mqtt::onMessageReceived(const QByteArray &message,
                             const QMqttTopicName &topic) {
  QString topicName = topic.name();
  emit messageReceived(topicName, message);

  // Try to parse as JSON
  QVariantMap jsonData = byteArrayToVariantMap(message);
  if (!jsonData.isEmpty()) {
    emit messageReceivedJson(topicName, jsonData);
  }
}

void Mqtt::onError(QMqttClient::ClientError clientError) {
  QString errorMessage;

  switch (clientError) {
    case QMqttClient::NoError:
      return;
    case QMqttClient::InvalidProtocolVersion:
      errorMessage = "Invalid protocol version";
      break;
    case QMqttClient::IdRejected:
      errorMessage = "Client ID rejected";
      break;
    case QMqttClient::ServerUnavailable:
      errorMessage = "Server unavailable";
      break;
    case QMqttClient::BadUsernameOrPassword:
      errorMessage = "Bad username or password";
      break;
    case QMqttClient::NotAuthorized:
      errorMessage = "Not authorized";
      break;
    case QMqttClient::TransportInvalid:
      errorMessage = "Transport invalid";
      break;
    case QMqttClient::ProtocolViolation:
      errorMessage = "Protocol violation";
      break;
    case QMqttClient::UnknownError:
      errorMessage = "Unknown error";
      break;
    case QMqttClient::Mqtt5SpecificError:
      errorMessage = "MQTT 5 specific error";
      break;
    default:
      errorMessage = "Unspecified error";
  }

  emit error(errorMessage);
}

QVariantMap Mqtt::byteArrayToVariantMap(const QByteArray &data) {
  QJsonParseError parseError;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

  if (parseError.error != QJsonParseError::NoError) {
    return QVariantMap();
  }

  if (doc.isObject()) {
    return doc.object().toVariantMap();
  }

  return QVariantMap();
}
