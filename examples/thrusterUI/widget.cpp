#include "widget.h"
#include "thrusterrpmwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);

    m_thrusterWidget = new ThrusterRpmWidget(this);
    m_thrusterWidget->setMinimumSize(600, 420);
    m_thrusterWidget->setMaxAbsRpm(100.0);
    m_thrusterWidget->setRpms(0.0, 0.0);

    auto *leftTitle = new QLabel(QStringLiteral("左推进器"), this);
    m_leftSlider = new QSlider(Qt::Horizontal, this);
    m_leftSlider->setRange(-100, 100);
    m_leftSlider->setValue(0);
    m_leftValueLabel = new QLabel(QString::number(m_leftSlider->value()), this);

    auto *rightTitle = new QLabel(QStringLiteral("右推进器"), this);
    m_rightSlider = new QSlider(Qt::Horizontal, this);
    m_rightSlider->setRange(-100, 100);
    m_rightSlider->setValue(0);
    m_rightValueLabel = new QLabel(QString::number(m_rightSlider->value()), this);

    auto *leftLayout = new QHBoxLayout;
    leftLayout->addWidget(leftTitle);
    leftLayout->addWidget(m_leftSlider, 1);
    leftLayout->addWidget(m_leftValueLabel);

    auto *rightLayout = new QHBoxLayout;
    rightLayout->addWidget(rightTitle);
    rightLayout->addWidget(m_rightSlider, 1);
    rightLayout->addWidget(m_rightValueLabel);

    mainLayout->addWidget(m_thrusterWidget, 1);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    connect(m_leftSlider, &QSlider::valueChanged, this, [this](int value) {
        m_thrusterWidget->setLeftRpm(static_cast<double>(value));
        m_leftValueLabel->setText(QString::number(value));
    });

    connect(m_rightSlider, &QSlider::valueChanged, this, [this](int value) {
        m_thrusterWidget->setRightRpm(static_cast<double>(value));
        m_rightValueLabel->setText(QString::number(value));
    });
}

Widget::~Widget()
{
}
