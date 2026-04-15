#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QListView;
class QLabel;
class QTextEdit;
class GinsDataListModel;

// 演示窗口：
// - 左侧列表展示模型条目；
// - 右侧显示选中条目详情与序列化 hex；
// - 通过按钮演示增删改查 + protobuf 编解码流程。
class Widget : public QWidget {
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);

private slots:
    // 使用 sampleData 追加一条模拟数据。
    void onAppend();
    // 删除当前选中行。
    void onRemoveSelected();
    // 用新的模拟值覆盖当前选中行。
    void onEditSelected();
    // 清空整个模型。
    void onClear();
    // 将选中条目序列化为二进制并显示为 hex。
    void onSerialize();
    // 从 hex 反序列化并追加到模型。
    void onDeserialize();
    // 更新右侧详情显示。
    void onSelectionChanged();

private:
    GinsDataListModel *m_model;
    QListView         *m_listView;
    QLabel            *m_detailLabel;
    QLabel            *m_statusLabel;
    QTextEdit         *m_hexEdit;

    int m_sampleIndex = 0;
};

#endif // WIDGET_H
