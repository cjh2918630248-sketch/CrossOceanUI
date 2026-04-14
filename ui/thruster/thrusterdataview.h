#ifndef THRUSTERDATAVIEW_H
#define THRUSTERDATAVIEW_H

#include <QWidget>

class ThrusterDataModel;
class QLabel;

// ThrusterDataModel 的单数据摘要视图。
// 在单数据模型模式下，currentIndex() 固定为 0（模型存在时），
// 用于兼容旧接口调用习惯。
class ThrusterDataView : public QWidget
{
    Q_OBJECT
public:
    explicit ThrusterDataView(QWidget *parent = nullptr);

    void setModel(ThrusterDataModel *model);

    // 返回当前选中行索引，无选中时返回 -1。
    int currentIndex() const;

private slots:
    void onModelChanged();

private:
    void buildUi();

    QLabel            *m_lblMode    = nullptr;
    QLabel            *m_lblSummary = nullptr;
    QLabel            *m_lblMeta    = nullptr;
    ThrusterDataModel *m_model      = nullptr;
};

#endif // THRUSTERDATAVIEW_H
