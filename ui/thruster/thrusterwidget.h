#ifndef THRUSTERWIDGET_H
#define THRUSTERWIDGET_H

#include <QWidget>
#include <QVariantMap>

class ThrusterDataModel;
class ThrusterDataPanel;
class ThrusterDataView;

// Thruster UI 的统一入口控件。
// - setModel(): 绑定外部传入的单推进器数据模型；
// - setData(): 适合外部只想直接喂一份数据，不自行管理模型；
// - clear(): 解除绑定并清空界面。
class ThrusterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ThrusterWidget(QWidget *parent = nullptr);

    void setModel(ThrusterDataModel *model);
    void setData(const QVariantMap &data);
    void clear();

    ThrusterDataModel    *model() const;
    ThrusterDataPanel    *panel() const;
    ThrusterDataView *summaryView() const;

private:
    void buildUi();
    void refreshPanel();

    ThrusterDataView *m_listView      = nullptr;
    ThrusterDataPanel    *m_panel         = nullptr;
    ThrusterDataModel    *m_model         = nullptr;
    ThrusterDataModel    *m_internalModel = nullptr;
};

#endif // THRUSTERWIDGET_H
