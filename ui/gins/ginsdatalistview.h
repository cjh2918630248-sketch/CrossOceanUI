#ifndef GINSDATALISTVIEW_H
#define GINSDATALISTVIEW_H

#include <QWidget>
#include <QModelIndex>

class GinsDataListModel;
class QListView;
class QLabel;

// GinsDataListModel 的配套列表视图。
// 调用 setModel() 绑定数据源，监听 selectionChanged(int) 获取当前选中行索引。
// index == -1 表示无选中项。
class GinsDataListView : public QWidget
{
    Q_OBJECT
public:
    explicit GinsDataListView(QWidget *parent = nullptr);

    void setModel(GinsDataListModel *model);

    // 返回当前选中行索引，无选中时返回 -1。
    int currentIndex() const;

signals:
    void selectionChanged(int index);

private slots:
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    // void onCountChanged();

private:
    void buildUi();

    QListView         *m_listView = nullptr;
    QLabel            *m_lblCount = nullptr;
    GinsDataListModel *m_model    = nullptr;
};

#endif // GINSDATALISTVIEW_H
