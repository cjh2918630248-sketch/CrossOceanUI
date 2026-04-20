#ifndef VELOCITY_H
#define VELOCITY_H

#include <QWidget>

class Velocity : public QWidget
{
    Q_OBJECT
public:
    explicit Velocity(QWidget *parent = nullptr);
    void SetVelocity(double speed);
    double GetVelocity() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_speed = 0.0;
    double m_minSpeed = 0.0;
    double m_maxSpeed = 120.0;
};

#endif // VELOCITY_H
