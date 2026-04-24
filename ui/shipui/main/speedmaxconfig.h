#ifndef SPEEDMAXCONFIG_H
#define SPEEDMAXCONFIG_H

#include <QString>
#include <QByteArray>
#include <QFile>
#include <QString>

/// 从文本文件读取最大船速（m/s）。文件约定：首行一个正浮点数，可含注释空行（仅读第一行非空）。
bool readMaxSpeedMpsFromFile(const QString &absolutePath, double *outMax);

/// 若文件不存在则写入默认最大值（演示 / 首次部署用）。
bool ensureDefaultMaxSpeedFile(const QString &absolutePath, double defaultMaxMps);

#endif // SPEEDMAXCONFIG_H
