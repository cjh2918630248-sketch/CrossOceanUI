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
#ifndef MQTT_H
#define MQTT_H

#include <QtMqtt/qmqttclient.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>

class Mqtt : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
  Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
  Q_PROPERTY(
      QString username READ username WRITE setUsername NOTIFY usernameChanged)
  Q_PROPERTY(
      QString password READ password WRITE setPassword NOTIFY passwordChanged)
  Q_PROPERTY(
      int connectionState READ connectionState NOTIFY connectionStateChanged)

 public:
  explicit Mqtt(QObject *parent = nullptr);
  ~Mqtt();

  // MQTT connection properties
  QString hostname() const;
  void setHostname(const QString &hostname);

  int port() const;
  void setPort(int port);

  QString username() const;
  void setUsername(const QString &username);

  QString password() const;
  void setPassword(const QString &password);

  int connectionState() const;

  // Topic management
  Q_INVOKABLE bool addTopic(const QString &topicName);
  Q_INVOKABLE bool removeTopic(const QString &topicName);
  Q_INVOKABLE QStringList topics() const;

  // Connection management
  Q_INVOKABLE void connect();
  Q_INVOKABLE void disconnect();

  // Data publishing
  Q_INVOKABLE bool publish(const QString &topic, const QByteArray &message,
                           int qos = 0);
  Q_INVOKABLE bool publish(const QString &topic, const QVariantMap &message,
                           int qos = 0);

 signals:
  void hostnameChanged(const QString &hostname);
  void portChanged(int port);
  void usernameChanged(const QString &username);
  void passwordChanged(const QString &password);
  void connectionStateChanged(int state);
  void connected();
  void disconnected();
  void messageReceived(const QString &topic, const QByteArray &message);
  void messageReceivedJson(const QString &topic, const QVariantMap &message);
  void error(const QString &errorMessage);

 private slots:
  void onStateChanged(QMqttClient::ClientState state);
  void onMessageReceived(const QByteArray &message,
                         const QMqttTopicName &topic);
  void onError(QMqttClient::ClientError error);

 private:
  void setupMqttClient();
  QVariantMap byteArrayToVariantMap(const QByteArray &data);

 private:
  QMqttClient *mMqttClient = nullptr;
  QMap<QString, QMqttSubscription *> mSubscriptions;
  QStringList mTopics;
};

#endif  // MQTT_H
