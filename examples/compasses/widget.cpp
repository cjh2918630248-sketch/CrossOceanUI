#include "widget.h"

#include "compasseswidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_compassesWidget = new CompassesWidget(this);

    auto *pointerTitle = new QLabel(QStringLiteral("指针角度"), this);
    m_pointerSlider = new QSlider(Qt::Horizontal, this);
    m_pointerSlider->setRange(-180, 180);
    m_pointerSlider->setValue(0);
    m_pointerValueLabel = new QLabel(QString::number(m_pointerSlider->value()), this);

    auto *pointerLayout = new QHBoxLayout;
    pointerLayout->addWidget(pointerTitle);
    pointerLayout->addWidget(m_pointerSlider, 1);
    pointerLayout->addWidget(m_pointerValueLabel);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_compassesWidget, 1);
    mainLayout->addLayout(pointerLayout);

    connect(m_pointerSlider, &QSlider::valueChanged, this, [this](int value) {
        m_compassesWidget->setPointerAngle(static_cast<double>(value));
        m_pointerValueLabel->setText(QString::number(value));
    });
}

Widget::~Widget()
{
}
