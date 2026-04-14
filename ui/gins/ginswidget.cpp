#include "ginswidget.h"
#include "ginsdatapanel.h"
#include "ginsdatalistview.h"
#include "ginsdatalistmodel.h"

#include <QHBoxLayout>

GinsWidget::GinsWidget(QWidget *parent)
    : QWidget(parent)
{
    buildUi();
}

void GinsWidget::buildUi()
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(6);

    m_listView = new GinsDataListView(this);
    m_listView->setMinimumWidth(240);
    m_listView->setMaximumWidth(300);
    m_listView->hide();
    root->addWidget(m_listView);

    m_panel = new GinsDataPanel(this);
    root->addWidget(m_panel, 1);

    connect(m_listView, &GinsDataListView::selectionChanged,
            this, [this](int idx) {
        if (!m_listModel) return;
        if (idx >= 0 && idx < m_listModel->count())
            m_panel->setData(m_listModel->get(idx));
        else
            m_panel->clear();
    });
}

void GinsWidget::setData(const GinsDataModel &model)
{
    m_listModel = nullptr;
    m_listView->hide();
    m_panel->setData(model);
}

void GinsWidget::setModel(GinsDataListModel *listModel)
{
    m_listModel = listModel;
    m_listView->setModel(listModel);
    m_listView->show();
    m_panel->clear();
}

void GinsWidget::clear()
{
    m_listModel = nullptr;
    m_listView->hide();
    m_panel->clear();
}

GinsDataPanel *GinsWidget::panel() const
{
    return m_panel;
}

GinsDataListView *GinsWidget::listView() const
{
    return m_listView;
}
