#include "ginsdatapanel.h"
#include "ginsdatamodel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDateTime>

// namespace {

// const QString kCardStyle =
//     "QGroupBox {"
//     "  font-weight: bold; font-size: 12px;"
//     "  border: 1px solid #c8cdd3; border-radius: 6px;"
//     "  margin-top: 14px; padding: 10px 8px 8px 8px;"
//     "  background: #fafbfc;"
//     "}"
//     "QGroupBox::title {"
//     "  subcontrol-origin: margin; left: 10px; top: 2px;"
//     "  padding: 0 6px; color: #2c6fbb;"
//     "}";

// const QString kValueStyle =
//     "QLabel { font-family: 'Consolas','Courier New',monospace;"
//     "  font-size: 12px; color: #1a1a2e; }";

// const QString kHighlightStyle =
//     "QLabel { font-family: 'Consolas','Courier New',monospace;"
//     "  font-size: 12px; font-weight: bold; color: #0e6655; }";

QString ginsStatusText(uint s)
{
    switch (s) {
    case 0:  return QStringLiteral("未初始化");
    case 1:  return QStringLiteral("单点定位");
    case 2:  return QStringLiteral("差分定位");
    case 4:  return QStringLiteral("组合导航");
    default: return QString("未知 (%1)").arg(s);
    }
}

// } // namespace

GinsDataPanel::GinsDataPanel(QWidget *parent)
    : QWidget(parent)
{
    buildUi();
}

QGroupBox *GinsDataPanel::makeGroup(const QString &title,
                                     const QStringList &rowNames,
                                     QList<QLabel *> &outLabels)
{
    auto *box  = new QGroupBox(title, this);
    // box->setStyleSheet(kCardStyle);

    auto *form = new QFormLayout(box);
    form->setLabelAlignment(Qt::AlignRight);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(5);

    for (const QString &name : rowNames) {
        auto *val = new QLabel(QStringLiteral("--"), this);
        // val->setStyleSheet(kValueStyle);
        val->setObjectName("valueLabel");

        val->setTextInteractionFlags(Qt::TextSelectableByMouse);
        form->addRow(name + "：", val);
        outLabels.append(val);
    }
    return box;
}

void GinsDataPanel::buildUi()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(6);

    auto *cols  = new QHBoxLayout;
    cols->setSpacing(8);
    auto *left  = new QVBoxLayout;
    auto *right = new QVBoxLayout;

    // 位置组
    {
        QList<QLabel *> out;
        auto *box = makeGroup(tr("位置"),
                              {tr("端口号"), tr("UTC 时间"), tr("纬度 (°)"),
                               tr("经度 (°)"), tr("高度 (m)"), tr("横滚 (°)"), tr("俯仰 (°)"), tr("航向 (°)")},
                              out);
        m_lblPort      = out[0];
        m_lblUtcLocal  = out[1];
        m_lblLatitude  = out[2];
        m_lblLongitude = out[3];
        m_lblAltitude  = out[4];
        m_lblRoll      = out[5];
        m_lblPitch     = out[6];
        m_lblHeading   = out[7];
        left->addWidget(box);
    }

    // 姿态组
    // {
    //     QList<QLabel *> out;
    //     auto *box = makeGroup(tr("姿态"),
    //                           {tr("横滚 (°)"), tr("俯仰 (°)"), tr("航向 (°)")},
    //                           out);
        // m_lblRoll    = out[0];
        // m_lblPitch   = out[1];
        // m_lblHeading = out[2];
    //     left->addWidget(box);
    // }

    left->addStretch();

    // 速度与加速度组
    {
        QList<QLabel *> out;
        auto *box = makeGroup(tr("状态"),
                              {tr("北向速度 (m/s)"), tr("东向速度 (m/s)"),
                               tr("转向速率"), tr("Ax"), tr("Ay"), tr("Aaz"),
                                tr("GINS 状态"), tr("HDOP"), tr("航向标准差")},
                              out);
        m_lblVn         = out[0];
        m_lblVe         = out[1];
        m_lblRoti       = out[2];
        m_lblAx         = out[3];
        m_lblAy         = out[4];
        m_lblAaz        = out[5];
        m_lblGinsStatus = out[6];
        m_lblHdop       = out[7];
        m_lblHeadStd    = out[8];
        right->addWidget(box);
    }

    // 状态组
    // {
    //     QList<QLabel *> out;
    //     auto *box = makeGroup(tr("状态"),
    //                           {tr("GINS 状态"), tr("HDOP"), tr("航向标准差")},
    //                           out);
    //     m_lblGinsStatus = out[0];
    //     m_lblHdop       = out[1];
    //     m_lblHeadStd    = out[2];
    //     right->addWidget(box);
    // }

    right->addStretch();

    cols->addLayout(left);
    cols->addLayout(right);
    root->addLayout(cols);
}

void GinsDataPanel::setLabel(QLabel *lbl, const QString &text, bool highlight)
{
    lbl->setText(text);
    // lbl->setStyleSheet(highlight ? kHighlightStyle : kValueStyle);

    lbl->setObjectName(highlight ? "highlightLabel" : "valueLabel");
}

void GinsDataPanel::setData(const QVariantMap &m)
{
    setLabel(m_lblPort, QString::number(m.value("port").toUInt()));

    const double utc = m.value("utcLocal").toDouble();
    setLabel(m_lblUtcLocal,
             QDateTime::fromSecsSinceEpoch(static_cast<qint64>(utc), Qt::UTC)
                 .toString("yyyy-MM-dd HH:mm:ss"));

    setLabel(m_lblLatitude,  QString::number(m.value("latitude").toDouble(),  'f', 6), true);
    setLabel(m_lblLongitude, QString::number(m.value("longitude").toDouble(), 'f', 6), true);
    setLabel(m_lblAltitude,  QString::number(m.value("altitude").toDouble(),  'f', 2));

    setLabel(m_lblRoll,    QString::number(m.value("rollDeg").toDouble(),    'f', 2));
    setLabel(m_lblPitch,   QString::number(m.value("pitchDeg").toDouble(),   'f', 2));
    setLabel(m_lblHeading, QString::number(m.value("headingDeg").toDouble(), 'f', 2), true);

    setLabel(m_lblVn,   QString::number(m.value("vnMps").toDouble(), 'f', 3));
    setLabel(m_lblVe,   QString::number(m.value("veMps").toDouble(), 'f', 3));
    setLabel(m_lblRoti, QString::number(m.value("roti").toDouble(),  'f', 4));
    setLabel(m_lblAx,   QString::number(m.value("ax").toDouble(),    'f', 4));
    setLabel(m_lblAy,   QString::number(m.value("ay").toDouble(),    'f', 4));
    setLabel(m_lblAaz,  QString::number(m.value("aaz").toDouble(),   'f', 4));

    setLabel(m_lblGinsStatus, ginsStatusText(m.value("ginsStatus").toUInt()), true);
    setLabel(m_lblHdop,       QString::number(m.value("hdop").toDouble(),    'f', 2));
    setLabel(m_lblHeadStd,    QString::number(m.value("headStd").toDouble(), 'f', 3));
}

void GinsDataPanel::setData(const GinsDataModel &model)
{
    setLabel(m_lblPort, QString::number(model.port()));
    const double utc = model.utcLocal();
    setLabel(m_lblUtcLocal,
             QDateTime::fromSecsSinceEpoch(static_cast<qint64>(utc), Qt::UTC)
                 .toString("yyyy-MM-dd HH:mm:ss"));

    setLabel(m_lblLatitude,  QString::number(model.latitude(),  'f', 6), true);
    setLabel(m_lblLongitude, QString::number(model.longitude(), 'f', 6), true);
    setLabel(m_lblAltitude,  QString::number(model.altitude(),  'f', 2));

    setLabel(m_lblRoll,    QString::number(model.rollDeg(),    'f', 2));
    setLabel(m_lblPitch,   QString::number(model.pitchDeg(),   'f', 2));
    setLabel(m_lblHeading, QString::number(model.headingDeg(), 'f', 2), true);

    setLabel(m_lblVn,   QString::number(model.vnMps(), 'f', 3));
    setLabel(m_lblVe,   QString::number(model.veMps(), 'f', 3));
    setLabel(m_lblRoti, QString::number(model.roti(),  'f', 4));
    setLabel(m_lblAx,   QString::number(model.ax(),    'f', 4));
    setLabel(m_lblAy,   QString::number(model.ay(),    'f', 4));
    setLabel(m_lblAaz,  QString::number(model.aaz(),   'f', 4));

    setLabel(m_lblGinsStatus, ginsStatusText(model.ginsStatus()));
    setLabel(m_lblHdop,       QString::number(model.hdop(),    'f', 2));
    setLabel(m_lblHeadStd,    QString::number(model.headStd(), 'f', 3));
}

void GinsDataPanel::clear()
{
    for (auto *lbl : {m_lblPort, m_lblUtcLocal, m_lblLatitude, m_lblLongitude,
                      m_lblAltitude, m_lblRoll, m_lblPitch, m_lblHeading,
                      m_lblVn, m_lblVe, m_lblRoti, m_lblAx, m_lblAy, m_lblAaz,
                      m_lblGinsStatus, m_lblHdop, m_lblHeadStd}) {
        lbl->setText(QStringLiteral("--"));
        // lbl->setStyleSheet(kValueStyle);
        lbl->setObjectName("valueLabel");
    }
}
