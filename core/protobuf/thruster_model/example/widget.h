#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QLabel;
class QTextEdit;
class ThrusterDataModel;

// 演示窗口：
// - 展示单个 ThrusterDataModel 的字段与元信息；
// - 按钮演示 fromMap/loadFromProto/serialize/deserialize 等接口；
// - 不再依赖旧版列表模型接口。
class Widget : public QWidget {
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);

private slots:
    void onLoadSample();
    void onOverwriteSample();
    void onClear();
    void onLoadProto();
    void onExportProto();
    void onSerialize();
    void onDeserialize();
    void onPrintData();
    void refreshView();
    void onErrorOccurred(int type, const QString &msg);

private:
    ThrusterDataModel *m_model      = nullptr;
    QLabel            *m_detailLabel = nullptr;
    QLabel            *m_statusLabel = nullptr;
    QTextEdit         *m_hexEdit     = nullptr;

    int m_sampleIndex = 0;
};

#endif // WIDGET_H
