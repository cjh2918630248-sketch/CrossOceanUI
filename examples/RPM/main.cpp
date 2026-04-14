#include "rpm.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto *window = new QWidget;
    window->setWindowTitle("RPM Example");

    auto *rpmWidget = new RPM(window);
    rpmWidget->setMinimumSize(600, 450);

    // 4) RPM 控制行：标题 + 滑块 + 当前值文本。
    auto *rpmTitle = new QLabel("RPM", window);
    auto *rpmSlider = new QSlider(Qt::Horizontal, window);
    rpmSlider->setRange(-100, 100);
    rpmSlider->setValue(0);
    auto *rpmValueLabel = new QLabel(QString::number(rpmSlider->value()), window);

    // 5) Angel 控制行：标题 + 滑块 + 当前值文本。
    auto *angleTitle = new QLabel("Angel", window);
    auto *angleSlider = new QSlider(Qt::Horizontal, window);
    angleSlider->setRange(-90, 90);
    angleSlider->setValue(0);
    auto *angleValueLabel = new QLabel(QString::number(angleSlider->value()), window);

    // 6) 信号槽连接：
    //    滑块数值变化 -> 调用 RPM 对外接口 setRpm/setAngle -> 刷新右侧数值标签。
    QObject::connect(rpmSlider, &QSlider::valueChanged, window, [rpmWidget, rpmValueLabel](int value) {
        rpmWidget->setRpm(static_cast<double>(value));
        rpmValueLabel->setText(QString::number(value));
    });

    QObject::connect(angleSlider, &QSlider::valueChanged, window, [rpmWidget, angleValueLabel](int value) {
        rpmWidget->setAngle(static_cast<double>(value));
        angleValueLabel->setText(QString::number(value));
    });

    // 7) 每行控件使用横向布局，第二列滑块设置 stretch=1 占据主要空间。
    auto *rpmControlLayout = new QHBoxLayout;
    rpmControlLayout->addWidget(rpmTitle);
    rpmControlLayout->addWidget(rpmSlider, 1);
    rpmControlLayout->addWidget(rpmValueLabel);

    auto *angleControlLayout = new QHBoxLayout;
    angleControlLayout->addWidget(angleTitle);
    angleControlLayout->addWidget(angleSlider, 1);
    angleControlLayout->addWidget(angleValueLabel);

    // 8) 整体采用纵向布局：上方仪表盘，下方两行控制条。
    auto *mainLayout = new QVBoxLayout(window);
    mainLayout->addWidget(rpmWidget, 1);
    mainLayout->addLayout(rpmControlLayout);
    mainLayout->addLayout(angleControlLayout);

    // 9) 启动时同步一次默认值，确保界面和内部状态一致。
    rpmWidget->setRpm(static_cast<double>(rpmSlider->value()));
    rpmWidget->setAngle(static_cast<double>(angleSlider->value()));

    // 10) 显示窗口并进入 Qt 事件循环。
    window->resize(840, 720);
    window->show();

    return QCoreApplication::exec();
}
