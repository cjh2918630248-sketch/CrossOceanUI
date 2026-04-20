#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "speedgauge.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

    void Init();

private slots:
    void on_speed_lineedit_returnPressed();

private:
    Ui::Widget *ui;

    // SpeedGauge* m_speed_guage = nullptr;
};
#endif // WIDGET_H
