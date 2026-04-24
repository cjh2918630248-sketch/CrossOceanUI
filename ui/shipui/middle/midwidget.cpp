#include "middle/midwidget.h"

namespace {
constexpr int kMidWidgetWidth  = 1440;
constexpr int kMidWidgetHeight = 1390;
} // namespace

MidWidget::MidWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("midWidget"));
    setFixedSize(kMidWidgetWidth, kMidWidgetHeight);

    m_mapSurface = new QLabel(this);
    m_mapSurface->setObjectName(QStringLiteral("mapSurface"));
    m_mapSurface->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_mapSurface->setAlignment(Qt::AlignCenter);
    m_mapSurface->setGeometry(0, 0, kMidWidgetWidth, kMidWidgetHeight);
}

void MidWidget::setMapBackground(const QPixmap &pixmap)
{
    if (!m_mapSurface) {
        return;
    }
    if (pixmap.isNull()) {
        m_mapSurface->setPixmap(QPixmap());
        return;
    }
    m_mapSurface->setPixmap(
        pixmap.scaled(m_mapSurface->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}
