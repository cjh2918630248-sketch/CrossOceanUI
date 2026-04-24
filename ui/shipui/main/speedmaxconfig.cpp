#include "main/speedmaxconfig.h"

bool readMaxSpeedMpsFromFile(const QString &absolutePath, double *outMax)
{
    if (!outMax) {
        return false;
    }
    QFile f(absolutePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    while (!f.atEnd()) {
        const QByteArray raw = f.readLine().trimmed();
        if (raw.isEmpty()) {
            continue;
        }
        bool ok = false;
        const double v = raw.toDouble(&ok);
        if (!ok || v <= 0.0) {
            return false;
        }
        *outMax = v;
        return true;
    }
    return false;
}

bool ensureDefaultMaxSpeedFile(const QString &absolutePath, double defaultMaxMps)
{
    if (QFile::exists(absolutePath)) {
        return true;
    }
    QFile f(absolutePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    f.write(QByteArray::number(defaultMaxMps, 'f', 1));
    f.write("\n", 1);
    f.flush();
    return true;
}
