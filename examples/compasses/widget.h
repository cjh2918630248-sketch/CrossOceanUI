#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class CompassesWidget;
class QSlider;
class QLabel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private:
    CompassesWidget *m_compassesWidget = nullptr;
    QSlider *m_pointerSlider = nullptr;
    QLabel *m_pointerValueLabel = nullptr;
};

#endif // WIDGET_H
#ifndef WIDGET_H
#endif // WIDGET_H
