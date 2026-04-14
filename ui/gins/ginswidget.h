#ifndef GINSWIDGET_H
#define GINSWIDGET_H

#include <QWidget>

class GinsDataModel;
class GinsDataListModel;
class GinsDataPanel;
class GinsDataListView;

// GINS 数据展示的统一入口控件。
// - setData(GinsDataModel)：单船模式，隐藏列表，仅显示详情面板；
// - setModel(GinsDataListModel*)：多船模式，左侧列表 + 右侧详情面板联动。
class GinsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GinsWidget(QWidget *parent = nullptr);

    // 单船模式：传入单条数据，隐藏列表，直接显示详情。
    void setData(const GinsDataModel &model);

    // 多船模式：传入列表模型，显示列表与详情面板联动。
    void setModel(GinsDataListModel *listModel);

    // 清空面板与列表。
    void clear();

    GinsDataPanel    *panel() const;
    GinsDataListView *listView() const;

private:
    void buildUi();

    GinsDataListView  *m_listView  = nullptr;
    GinsDataPanel     *m_panel     = nullptr;
    GinsDataListModel *m_listModel = nullptr;
};

#endif // GINSWIDGET_H
