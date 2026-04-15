#ifndef RPM_H
#define RPM_H

#include <QWidget>

// RPM 仪表控件：
// - 通过 setRpm() 设置转速值
// - 通过 setAngle() 设置角度值
// - 在 paintEvent() 中根据当前值进行绘制
class RPM : public QWidget
{
    Q_OBJECT

public:
    explicit RPM(QWidget *parent = nullptr);
    ~RPM() override;

    void setRpm(double rpmValue);
    // 设置角度值：绘制时以中线为基准，正值向右、负值向左。
    void setAngle(double angleValue);

protected:
    // Qt 重绘入口：每次 update() 后由框架回调。
    void paintEvent(QPaintEvent *event) override;

private:
    double m_rpm = 0.0;
    double m_maxAbsRpm = 100.0;
    double m_angle = 0.0;
    double m_maxAbsAngle = 90.0;
};

#endif // RPM_H
