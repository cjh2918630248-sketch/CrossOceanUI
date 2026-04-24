#include "top/topwidget.h"

namespace {
constexpr int kTopWidgetWidth  = 2168;
constexpr int kTopWidgetHeight = 106;
} // namespace

TopWidget::TopWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("topWidget"));
    setFixedSize(kTopWidgetWidth, kTopWidgetHeight);
    setupUi();
}

void TopWidget::setupUi()
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_vesselStatus = new VesselStatus(this);
    m_clockToolbar = new ClockToolbar(this);

    layout->addWidget(m_vesselStatus, 0, Qt::AlignVCenter | Qt::AlignLeft); //垂直居中并且左对齐
    layout->addStretch(1);
    layout->addWidget(m_clockToolbar, 0, Qt::AlignVCenter | Qt::AlignRight); //垂直居中并且右对齐
}