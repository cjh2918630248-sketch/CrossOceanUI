#include "main/scaledview.h"
#include "timerviewdemo.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ScaledView view;
    view.resize(QApplication::primaryScreen()->availableGeometry().size());
    view.show();

    TimerViewDemo timerDemo(view.shipUiWidget(), &app);
    timerDemo.start(50);

    return QCoreApplication::exec();
}
