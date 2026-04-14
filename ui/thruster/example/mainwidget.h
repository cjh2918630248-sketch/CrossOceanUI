#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QSpinBox>

class ThrusterDataModel;
class ThrusterDataView;
class ThrusterDataPanel;

// ─────────────────────────────────────────────────────────────────────────────
// MainWidget
//   thrusterUI 接口示例主窗口。
//
//   布局：
//     左侧  — ThrusterDataView（单数据摘要）
//     右上  — ThrusterDataPanel（详情面板）
//     右下  — 操作按钮区（演示写入 / 序列化 / 元信息等接口）
//     底部  — 日志栏（显示信号回调与序列化结果）
//
//   覆盖的接口：
//     ThrusterDataModel  : fromMap / toMap / loadFromProto / exportToProto
//                          setPort / setThrusterNum / serialize / deserialize
//                          printData / signals: dataUpdated / errorOccurred
//     ThrusterDataView : setModel / currentIndex
//     ThrusterDataPanel    : setData / clear
// ─────────────────────────────────────────────────────────────────────────────
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

private slots:
    // ── 数据操作（单条覆盖） ──
    void onLoadSample();
    void onOverwriteSample();
    void onClear();

    // ── 元信息 ──
    void onSetPort();
    void onSetThrusterNum();

    // ── 序列化 ──
    void onSerialize();
    void onDeserialize();

    // ── Proto 导入导出 ──
    void onLoadProto();
    void onExportProto();

    // ── 调试 ──
    void onPrintData();

    // ── 模型信号 ──
    void onDataUpdated();
    void onErrorOccurred(int type, const QString &msg);

private:
    void buildUi();
    void connectModel();
    void log(const QString &text);

    ThrusterDataModel    *m_model    = nullptr;
    ThrusterDataView *m_listView = nullptr;
    ThrusterDataPanel    *m_panel    = nullptr;

    QSpinBox  *m_spinPort        = nullptr;
    QSpinBox  *m_spinThrusterNum = nullptr;
    QTextEdit *m_logEdit         = nullptr;
    QLabel    *m_statusBar       = nullptr;
    QTextEdit *m_hexEdit         = nullptr;

    int m_sampleSeq = 0;
};

#endif // MAINWIDGET_H
