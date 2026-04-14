#ifndef COMPASSESWIDGET_H
#define COMPASSESWIDGET_H

#include <QWidget>

class QSvgWidget;

class CompassesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompassesWidget(QWidget *parent = nullptr);
    ~CompassesWidget() override;
    void setPointerAngle(double angleDeg);

private:
    QSvgWidget *m_headingBg = nullptr;
    QWidget *m_headingPointer = nullptr;
    QSvgWidget *m_rollPitchAll = nullptr;
};

#endif // COMPASSESWIDGET_H
