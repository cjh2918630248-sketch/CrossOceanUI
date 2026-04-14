#include "thrusterdataview.h"
#include "thrusterdatamodel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>

ThrusterDataView::ThrusterDataView(QWidget *parent)
    : QWidget(parent)
{
    buildUi();
}

void ThrusterDataView::buildUi()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(6, 6, 6, 6);
    root->setSpacing(4);

    // 标题栏
    auto *header = new QHBoxLayout;

    auto *title = new QLabel(tr("推进器摘要"), this);
    QFont f = title->font();
    f.setBold(true);
    f.setPointSize(12);
    title->setFont(f);
    title->setStyleSheet("color: #1a5276;");

    m_lblMode = new QLabel(tr("单数据模型"), this);
    m_lblMode->setStyleSheet("color: #888; font-size: 11px;");

    header->addWidget(title);
    header->addStretch();
    header->addWidget(m_lblMode);
    root->addLayout(header);

    m_lblSummary = new QLabel(tr("状态: --  RPM: --  功率: --"), this);
    m_lblSummary->setStyleSheet("color: #1f2328; font-size: 12px;");
    root->addWidget(m_lblSummary);

    m_lblMeta = new QLabel(tr("端口: --  推进器编号: --"), this);
    m_lblMeta->setStyleSheet("color: #555; font-size: 11px;");
    root->addWidget(m_lblMeta);
}

void ThrusterDataView::setModel(ThrusterDataModel *model)
{
    if (m_model) {
        disconnect(m_model, &ThrusterDataModel::dataUpdated,
                   this,    &ThrusterDataView::onModelChanged);
        disconnect(m_model, &ThrusterDataModel::portChanged,
                   this,    &ThrusterDataView::onModelChanged);
        disconnect(m_model, &ThrusterDataModel::thrusterNumChanged,
                   this,    &ThrusterDataView::onModelChanged);
    }

    m_model = model;

    if (m_model) {
        connect(m_model, &ThrusterDataModel::dataUpdated,
                this,    &ThrusterDataView::onModelChanged);
        connect(m_model, &ThrusterDataModel::portChanged,
                this,    &ThrusterDataView::onModelChanged);
        connect(m_model, &ThrusterDataModel::thrusterNumChanged,
                this,    &ThrusterDataView::onModelChanged);
        onModelChanged();
    }
}

int ThrusterDataView::currentIndex() const
{
    return m_model ? 0 : -1;
}

void ThrusterDataView::onModelChanged()
{
    if (!m_model) {
        m_lblSummary->setText(tr("状态: --  RPM: --  功率: --"));
        m_lblMeta->setText(tr("端口: --  推进器编号: --"));
        return;
    }

    const QVariantMap map = m_model->toMap();
    m_lblSummary->setText(
        tr("状态: %1  RPM: %2  功率: %3")
            .arg(map.value("statusText").toString())
            .arg(map.value("rpm").toInt())
            .arg(map.value("power").toInt()));
    m_lblMeta->setText(
        tr("端口: %1  推进器编号: %2")
            .arg(m_model->port())
            .arg(m_model->thrusterNum()));
}
