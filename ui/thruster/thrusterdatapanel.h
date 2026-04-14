#ifndef THRUSTERDATAPANEL_H
#define THRUSTERDATAPANEL_H

#include <QWidget>
#include <QLabel>
#include <QVariantMap>

class QGroupBox;

// 单条推进器数据展示面板（只读）。
// 通过 setData() 更新所有字段显示，clear() 重置为占位符 "--"。
// 数据格式与 ThrusterDataModel::toMap() 的返回值保持一致。
class ThrusterDataPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ThrusterDataPanel(QWidget *parent = nullptr);

    // 接收 ThrusterDataModel::toMap() 的返回值，刷新所有字段。
    void setData(const QVariantMap &map);

    // 将所有字段重置为 "--"。
    void clear();

private:
    void buildUi();
    QGroupBox *makeGroup(const QString &title,
                         const QStringList &rowNames,
                         QList<QLabel *> &outLabels);
    void setLabel(QLabel *lbl, const QString &text, bool highlight = false);

    QLabel *m_lblTitle        = nullptr;

    // 运行状态组
    QLabel *m_lblStatus       = nullptr;
    QLabel *m_lblStatusText   = nullptr;
    QLabel *m_lblErrorCode    = nullptr;

    // 电气参数组
    QLabel *m_lblRpm          = nullptr;
    QLabel *m_lblAngle        = nullptr;
    QLabel *m_lblPower        = nullptr;
    QLabel *m_lblBusVoltage   = nullptr;
    QLabel *m_lblBusCurrent   = nullptr;
    QLabel *m_lblPhaseCurrent = nullptr;

    // 温度组
    QLabel *m_lblMotorTemp    = nullptr;
    QLabel *m_lblMosTemp      = nullptr;
    QLabel *m_lblTemp         = nullptr;

    // 时间组
    QLabel *m_lblRunTime      = nullptr;
    QLabel *m_lblSumRunTime   = nullptr;
};

#endif // THRUSTERDATAPANEL_H
