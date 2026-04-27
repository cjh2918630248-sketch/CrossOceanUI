#include "main/icondye.h"

#include <QByteArray>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QXmlStreamReader>

QIcon iconDyedFromIcon(const QIcon &source, const QSize &logicalSize, const QColor &tint)
{
    const QPixmap pm = source.pixmap(logicalSize, QIcon::Normal, QIcon::Off);
    if (pm.isNull()) {
        return QIcon();
    }
    QImage image = pm.toImage().convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            const QColor c = image.pixelColor(x, y);
            if (c.alpha() == 0) {
                continue;
            }
            // 非预乘的深色描边/填充：以亮度为 alpha 权，将图标视为灰度蒙版
            const int luma   = 255 * 3 - c.red() - c.green() - c.blue();
            int       aOut   = c.alpha() * luma / (3 * 255);
            aOut = qBound(0, aOut, 255);
            if (aOut <= 0) {
                continue;
            }
            QColor o = tint;
            o.setAlpha(aOut);
            image.setPixelColor(x, y, o);
        }
    }
    return QIcon(QPixmap::fromImage(image));
}

QIcon iconDyedFromResource(const QString &qrcPath, const QSize &logicalSize, const QColor &tint)
{
    return iconDyedFromIcon(QIcon(qrcPath), logicalSize, tint);
}

QPixmap loadEmbeddedPixmap(const QString &qrcPath, const QSize &targetSize)
{
    QFile f(qrcPath);
    if (!f.open(QIODevice::ReadOnly)) {
        return QPixmap();
    }
    const QByteArray raw = f.readAll();
    f.close();

    // 尝试从 <image xlink:href="data:image/...;base64,..."> 提取内嵌位图
    QXmlStreamReader xml(raw);
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == QLatin1String("image")) {
            // xlink:href 或 href 均可能携带 data URI
            QString href = xml.attributes().value(QStringLiteral("xlink:href")).toString();
            if (href.isEmpty()) {
                href = xml.attributes().value(QStringLiteral("href")).toString();
            }
            if (href.startsWith(QStringLiteral("data:image/"))) {
                const int comma = href.indexOf(QLatin1Char(','));
                if (comma != -1) {
                    const QByteArray imgData =
                        QByteArray::fromBase64(href.mid(comma + 1).toLatin1());
                    QPixmap pm;
                    if (pm.loadFromData(imgData)) {
                        return pm.scaled(targetSize,
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
                    }
                }
            }
        }
    }

    // 无内嵌位图，当作普通 SVG 直接渲染
    QPixmap pm(targetSize);
    pm.fill(Qt::transparent);
    return QPixmap(qrcPath).scaled(targetSize,
                                   Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation);
}
