#ifndef SPEEDGAUGE_H
#define SPEEDGAUGE_H

#include <QWidget>
#include "velocityinputinterface.h"

class SpeedGauge : public QWidget, public VelocityInputInterface
{
    Q_OBJECT

public:
    explicit SpeedGauge(QWidget *parent = nullptr);
    ~SpeedGauge() override;
    void setSpeed(double speed) override;
    double speed() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_speed = 0.0;
    double m_minSpeed = 0.0;
    double m_maxSpeed = 120.0;
};

#endif // SPEEDGAUGE_H
#ifndef SPEEDGAUGE_H

#endif // SPEEDGAUGE_H