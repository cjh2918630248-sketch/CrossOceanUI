#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QLabel;
class QSlider;
class ThrusterRpmWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private:
    ThrusterRpmWidget *m_thrusterWidget = nullptr;
    QSlider *m_leftSlider = nullptr;
    QSlider *m_rightSlider = nullptr;
    QLabel *m_leftValueLabel = nullptr;
    QLabel *m_rightValueLabel = nullptr;
};
#endif // WIDGET_H
