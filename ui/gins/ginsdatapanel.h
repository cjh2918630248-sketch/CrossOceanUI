#ifndef GINSDATAPANEL_H
#define GINSDATAPANEL_H

#include <QWidget>
#include <QLabel>
#include <QVariantMap>

class QGroupBox;
class GinsDataModel;

// 单条 GINS 数据展示面板（只读）。
// 通过 setData() 更新所有字段显示，clear() 重置为占位符 "--"。
// 数据格式与 GinsDataListModel::get() 的返回值保持一致。
class GinsDataPanel : public QWidget
{
    Q_OBJECT
public:
    explicit GinsDataPanel(QWidget *parent = nullptr);

    // 接收 GinsDataListModel::get(index) 的返回值，刷新所有字段。
    void setData(const QVariantMap &map);
    void setData(const GinsDataModel& model);

    // 将所有字段重置为 "--"。
    void clear();

private:
    void buildUi();
    QGroupBox *makeGroup(const QString &title,
                         const QStringList &rowNames,
                         QList<QLabel *> &outLabels);
    void setLabel(QLabel *lbl, const QString &text, bool highlight = false);

    // 位置组
    QLabel *m_lblPort       = nullptr;
    QLabel *m_lblUtcLocal   = nullptr;
    QLabel *m_lblLatitude   = nullptr;
    QLabel *m_lblLongitude  = nullptr;
    QLabel *m_lblAltitude   = nullptr;

    // 姿态组
    QLabel *m_lblRoll       = nullptr;
    QLabel *m_lblPitch      = nullptr;
    QLabel *m_lblHeading    = nullptr;

    // 速度与加速度组
    QLabel *m_lblVn         = nullptr;
    QLabel *m_lblVe         = nullptr;
    QLabel *m_lblRoti       = nullptr;
    QLabel *m_lblAx         = nullptr;
    QLabel *m_lblAy         = nullptr;
    QLabel *m_lblAaz        = nullptr;

    // 状态组
    QLabel *m_lblGinsStatus = nullptr;
    QLabel *m_lblHdop       = nullptr;
    QLabel *m_lblHeadStd    = nullptr;
};

#endif // GINSDATAPANEL_H
