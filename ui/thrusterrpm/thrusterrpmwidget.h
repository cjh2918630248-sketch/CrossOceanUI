#ifndef THRUSTERRPMWIDGET_H
#define THRUSTERRPMWIDGET_H

#include <QWidget>

class ThrusterRpmWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThrusterRpmWidget(QWidget *parent = nullptr);
    ~ThrusterRpmWidget() override;

    void setLeftRpm(double rpmValue);
    void setRightRpm(double rpmValue);
    void setRpms(double leftRpm, double rightRpm);
    void setMaxAbsRpm(double maxAbsRpm);

    double leftRpm() const;
    double rightRpm() const;
    double maxAbsRpm() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double clampRpm(double value) const;

private:
    double m_leftRpm = 0.0;
    double m_rightRpm = 0.0;
    double m_maxAbsRpm = 100.0;
};

#endif // THRUSTERRPMWIDGET_H
