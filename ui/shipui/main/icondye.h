#ifndef ICONDYE_H
#define ICONDYE_H

#include <QColor>
#include <QIcon>
#include <QPixmap>
#include <QSize>
#include <QString>

/// 将单色/深色线稿图标的像素着色为 {@param tint}（保边缘透明度）。
QIcon iconDyedFromResource(const QString &qrcPath, const QSize &logicalSize, const QColor &tint);
QIcon iconDyedFromIcon(const QIcon &source, const QSize &logicalSize, const QColor &tint);

/**
 * 专为 Figma 导出的 SVG 设计：文件内含 <pattern><image xlink:href="data:image/png;base64,..."/>
 * Qt SVG 渲染器不支持 <pattern>，此函数直接解析 XML 提取内嵌 base64 图像并返回 QPixmap。
 * 若文件是普通 SVG（无内嵌位图），退回使用 QPixmap(qrcPath) 正常渲染。
 */
QPixmap loadEmbeddedPixmap(const QString &qrcPath, const QSize &targetSize);

#endif
