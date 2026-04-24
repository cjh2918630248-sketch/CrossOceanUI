#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QWidget>
#include <QHBoxLayout>

#include "top/clocktoolbar.h"
#include "top/vesselstatus.h"
/**
 * 顶部工具栏容器。
 * 固定尺寸 2168 × 106；水平 justify-content: space-between，中间间距 1065px。
 * 左半部为 VesselStatus（643×42），右半部为 ClockToolbar（460×70）。
 */
class TopWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TopWidget(QWidget *parent = nullptr);

    VesselStatus *vesselStatus()  const { return m_vesselStatus; }
    ClockToolbar *clockToolbar()  const { return m_clockToolbar; }

private:
    void setupUi();

    VesselStatus *m_vesselStatus = nullptr;
    ClockToolbar *m_clockToolbar = nullptr;
};

#endif // TOPWIDGET_H