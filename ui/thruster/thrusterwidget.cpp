#include "thrusterwidget.h"

#include "thrusterdatamodel.h"
#include "thrusterdataview.h"
#include "thrusterdatapanel.h"

#include <QHBoxLayout>

ThrusterWidget::ThrusterWidget(QWidget *parent)
    : QWidget(parent)
{
    buildUi();
}

void ThrusterWidget::buildUi()
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(6);

    m_listView = new ThrusterDataView(this);
    m_listView->setMinimumWidth(260);
    m_listView->setMaximumWidth(320);
    root->addWidget(m_listView);

    m_panel = new ThrusterDataPanel(this);
    root->addWidget(m_panel, 1);

    clear();
}

void ThrusterWidget::setModel(ThrusterDataModel *model)
{
    if (m_model == model)
        return;

    if (m_model)
        disconnect(m_model, nullptr, this, nullptr);

    m_model = model;
    m_listView->setModel(m_model);

    if (!m_model) {
        m_panel->clear();
        return;
    }

    connect(m_model, &ThrusterDataModel::dataUpdated,
            this, &ThrusterWidget::refreshPanel);
    refreshPanel();
}

void ThrusterWidget::setData(const QVariantMap &data)
{
    if (!m_internalModel)
        m_internalModel = new ThrusterDataModel(this);

    setModel(m_internalModel);
    m_internalModel->fromMap(data);
}

void ThrusterWidget::clear()
{
    if (m_model)
        disconnect(m_model, nullptr, this, nullptr);

    m_model = nullptr;
    m_listView->setModel(nullptr);
    m_panel->clear();
}

ThrusterDataModel *ThrusterWidget::model() const
{
    return m_model;
}

ThrusterDataPanel *ThrusterWidget::panel() const
{
    return m_panel;
}

ThrusterDataView *ThrusterWidget::summaryView() const
{
    return m_listView;
}

void ThrusterWidget::refreshPanel()
{
    if (!m_model) {
        m_panel->clear();
        return;
    }

    m_panel->setData(m_model->toMap());
}
