#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QTextEdit>
#include <QProgressBar>
#include "gins.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void onLoadSampleData();
    void onSerialize();
    void onDeserialize();
    void onClearAll();

private:
    void setupDataPanel();
    void updateDisplay(const GinsModel &model);
    void setLabelValue(QLabel *label, const QString &text, bool highlight = false);
    QGroupBox *createGroupBox(const QString &title, const QStringList &labels,
                              QList<QLabel *> &outputs);

    Ui::MainWidget *ui;

    // GINS 导航数据显示
    QLabel *m_lblPort;
    QLabel *m_lblUtcLocal;
    QLabel *m_lblLatitude;
    QLabel *m_lblLongitude;
    QLabel *m_lblAltitude;

    // 姿态数据显示
    QLabel *m_lblRoll;
    QLabel *m_lblPitch;
    QLabel *m_lblHeading;

    // 速度与加速度显示
    QLabel *m_lblVn;
    QLabel *m_lblVe;
    QLabel *m_lblRoti;
    QLabel *m_lblAx;
    QLabel *m_lblAy;
    QLabel *m_lblAaz;

    // 状态信息显示
    QLabel *m_lblGinsStatus;
    QLabel *m_lblHdop;
    QLabel *m_lblHeadStd;

    // 序列化区域
    QTextEdit *m_txtSerializedHex;
    QLabel *m_lblByteCount;
    QProgressBar *m_barStatus;

    // 数据模型
    GinsModel m_currentModel;
    std::string m_serializedBytes;
};

#endif // MAINWIDGET_H
