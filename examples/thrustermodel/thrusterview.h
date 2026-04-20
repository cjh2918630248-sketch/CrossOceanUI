#ifndef THRUSTERVIEW_H
#define THRUSTERVIEW_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include <QFormLayout>
#include <QVariantMap>

class ThrusterDataModel;

// ─────────────────────────────────────────────────────────────────────────────
// ThrusterCardWidget
//   单台推进器的数据展示卡片，显示所有 thruster::Data 字段。
//   通过 updateData(QVariantMap) / reset() 刷新界面。
// ─────────────────────────────────────────────────────────────────────────────
class ThrusterCardWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ThrusterCardWidget(const QString &title, QWidget *parent = nullptr);

    void updateData(const QVariantMap &data);
    void reset();

private:
    static QLabel *makeValueLabel(const QString &init, QWidget *parent);
    static QProgressBar *makeBar(int max, QWidget *parent);
    void applyTempColor(QLabel *lbl, int tempC);
    void applyStatusBadge(QLabel *lbl, const QString &statusText, uint status);

    // 状态区
    QLabel *m_lblStatusBadge;
    QLabel *m_lblErrorCode;

    // 运动参数
    QLabel *m_lblRpm;
    QLabel *m_lblAngle;
    QProgressBar *m_barRpm;

    // 功率
    QLabel *m_lblPower;
    QProgressBar *m_barPower;

    // 电气参数
    QLabel *m_lblBusVoltage;
    QLabel *m_lblBusCurrent;
    QLabel *m_lblPhaseCurrent;

    // 温度
    QLabel *m_lblMotorTemp;
    QLabel *m_lblMosTemp;
    QLabel *m_lblTemp;

    // 运行时间
    QLabel *m_lblRunTime;
    QLabel *m_lblSumRunTime;
};

// ─────────────────────────────────────────────────────────────────────────────
// ThrusterView
//   单推进器数据模型的演示窗口：
//   - 上方标题栏显示端口号与推进器编号；
//   - 中部显示单张 ThrusterCardWidget 卡片；
//   - 下方提供序列化输出区域与操作按钮；
//   - 底部状态栏实时反映模型状态与错误信息。
// ─────────────────────────────────────────────────────────────────────────────
class ThrusterView : public QWidget
{
    Q_OBJECT
public:
    explicit ThrusterView(QWidget *parent = nullptr);

    void setSampleIndex(const int& sample_index);

private slots:
    void onLoadSampleData();
    void onSerialize();
    void onDeserialize();
    void onClear();
    void onModelDataUpdated();
    void onModelError(int type, const QString &msg);

private:
    void setupUI();
    void refreshCard();
    void updateHeaderInfo();

    ThrusterDataModel *m_model;
    int m_sampleIndex = 0;

    // 标题栏
    QLabel *m_lblPort;
    QLabel *m_lblThrusterNum;

    // 卡片
    ThrusterCardWidget *m_card;

    // 序列化区
    QTextEdit *m_txtHex;
    QLabel    *m_lblByteInfo;

    // 状态栏
    QLabel *m_statusBar;
};

#endif // THRUSTERVIEW_H
