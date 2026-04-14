#include "ginsdatalistview.h"
#include "ginsdatalistmodel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QLabel>
#include <QFont>
#include <QAbstractItemView>
#include <QItemSelectionModel>

#include <QTextStream>
#include <QFile>
#include <QApplication>
#include <QDebug>

GinsDataListView::GinsDataListView(QWidget *parent)
    : QWidget(parent)
{
    buildUi();
}

void GinsDataListView::buildUi()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(6, 6, 6, 6);
    root->setSpacing(4);

    // 标题栏
    // auto *header = new QHBoxLayout;

    // auto *title = new QLabel(tr("GINS 数据列表"), this);
    // QFont f = title->font();
    // f.setBold(true);
    // f.setPointSize(12);
    // title->setFont(f);
    // title->setStyleSheet("color: #1a5276;");

    // m_lblCount = new QLabel(tr("共 0 条"), this);
    // m_lblCount->setStyleSheet("color: #888; font-size: 11px;");

    // header->addWidget(title);
    // header->addStretch();
    // header->addWidget(m_lblCount);
    // root->addLayout(header);

    // 列表视图
    m_listView = new QListView(this);
    m_listView->setAlternatingRowColors(true);
    m_listView->setSelectionMode(QAbstractItemView::SingleSelection);
    // outline:0 消除 Linux 上选中时 QStyle 绘制的虚线焦点框（"滑块"外观的根源）。
    // 所有 item 状态（包括 :alternate:selected）均显式声明，
    // 避免 Qt 将 :alternate 与 :selected 的背景叠加渲染。
    QFile styleFile(":/example/gins.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream styleStream(&styleFile);
        QString styleSheet = styleStream.readAll();
        /*qApp->setStyleSheet(styleSheet);*/ // 应用到整个应用
       /* 或仅应用到特定控件：*/
        m_listView->setStyleSheet(styleSheet);
        // qDebug() << m_listView->styleSheet();
        styleFile.close();
    }
    root->addWidget(m_listView);
}

void GinsDataListView::setModel(GinsDataListModel *model)
{
    // 断开旧模型的信号
    // if (m_model) {
    //     disconnect(m_model, &GinsDataListModel::countChanged,
    //                this,    &GinsDataListView::onCountChanged);
    // }

    m_model = model;
    m_listView->setModel(model);

    if (m_model) {
        // connect(m_model, &GinsDataListModel::countChanged,
        //         this,    &GinsDataListView::onCountChanged);
        // onCountChanged();

        // selectionModel() 在 setModel() 后才可用
        connect(m_listView->selectionModel(),
                &QItemSelectionModel::currentChanged,
                this, &GinsDataListView::onCurrentChanged);
    }
}

int GinsDataListView::currentIndex() const
{
    if (!m_listView->selectionModel())
        return -1;
    const QModelIndex idx = m_listView->selectionModel()->currentIndex();
    return idx.isValid() ? idx.row() : -1;
}

void GinsDataListView::onCurrentChanged(const QModelIndex &current,
                                         const QModelIndex & /*previous*/)
{
    emit selectionChanged(current.isValid() ? current.row() : -1);
}

// void GinsDataListView::onCountChanged()
// {
//     const int n = m_model ? m_model->count() : 0;
//     m_lblCount->setText(tr("共 %1 条").arg(n));
// }
