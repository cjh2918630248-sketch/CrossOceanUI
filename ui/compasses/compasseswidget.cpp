#include "compasseswidget.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QStackedLayout>
#include <QSvgRenderer>
#include <QSvgWidget>

class RotatingSvgWidget : public QWidget
{
public:
    explicit RotatingSvgWidget(const QString &svgPath, QWidget *parent = nullptr)
        : QWidget(parent)
        , m_renderer(svgPath, this)
    {
    }

    void setAngle(double angleDeg)
    {
        m_angleDeg = angleDeg;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event)
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

        const QPointF c = rect().center();
        painter.translate(c); //将坐标原点由原来的窗口左上角(0,0)移动到点c的位置作为原点(0,0)
        painter.rotate(m_angleDeg);
        painter.translate(-c);
        m_renderer.render(&painter, rect());
    }

private:
    QSvgRenderer m_renderer;
    double m_angleDeg = 0.0;
};

CompassesWidget::CompassesWidget(QWidget *parent)
    : QWidget(parent)
{
    const int instrumentSize = 300;
    const int headingInset = 37;

    m_headingBg = new QSvgWidget(":/asset/headingbg.svg", this);
    m_headingPointer = new RotatingSvgWidget(":/asset/headingpointer.svg", this);
    m_rollPitchAll = new QSvgWidget(":/asset/rollpitchall.svg", this);
    m_rollPitchAll->setFixedSize(instrumentSize, instrumentSize);

    auto *headingContainer = new QWidget(this);
    headingContainer->setFixedSize(instrumentSize, instrumentSize);
    auto *headingOverlay = new QStackedLayout(headingContainer);
    headingOverlay->setStackingMode(QStackedLayout::StackAll);
    headingOverlay->setContentsMargins(headingInset, headingInset, headingInset, headingInset);
    headingOverlay->addWidget(m_headingBg);
    headingOverlay->addWidget(m_headingPointer);
    headingOverlay->setCurrentWidget(m_headingPointer); //e确保m_headingPointer在逻辑上处于激活状态
    m_headingPointer->raise();

    auto *rowLayout = new QHBoxLayout;
    rowLayout->setContentsMargins(20, 20, 20, 20);
    rowLayout->setSpacing(0);
    rowLayout->addStretch(1);
    rowLayout->addWidget(m_rollPitchAll);
    rowLayout->addStretch(1);
    rowLayout->addWidget(headingContainer);
    rowLayout->addStretch(1);
    setLayout(rowLayout);
}

CompassesWidget::~CompassesWidget()
{
}

void CompassesWidget::setPointerAngle(double angleDeg)
{
    auto *pointer = static_cast<RotatingSvgWidget *>(m_headingPointer);
    pointer->setAngle(angleDeg);
}
