#include "widget.h"
#include "../ginsdatalistmodel.h"

#include <QListView>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

// 生成可复现的演示数据，便于观察模型变化和序列化结果。
static QVariantMap sampleData(int seq)
{
    return {
        { "port",       static_cast<uint>(seq % 4) },
        { "utcLocal",   100000.0 + seq * 1.5       },
        { "latitude",   31.2304 + seq * 0.001      },
        { "longitude",  121.4737 + seq * 0.001     },
        { "altitude",   3.5 + seq * 0.1            },
        { "rollDeg",    0.5 + seq * 0.2             },
        { "pitchDeg",   -0.3 + seq * 0.1            },
        { "headingDeg", 45.0 + seq * 10.0           },
        { "vnMps",      1.2 + seq * 0.05            },
        { "veMps",      -0.8 + seq * 0.03           },
        { "roti",       0.01 * seq                   },
        { "ax",         0.02 * seq                   },
        { "ay",         -0.01 * seq                  },
        { "aaz",        9.8 + 0.001 * seq            },
        { "ginsStatus", 4u                           },
        { "hdop",       1.2 + 0.1 * seq              },
        { "headStd",    0.5 + 0.05 * seq             },
    };
}

// 将模型返回的 map 格式化为多行文本，便于 UI 阅读。
static QString mapToString(const QVariantMap &m)
{
    if (m.isEmpty()) return QStringLiteral("(empty)");

    return QStringLiteral(
        "port=%1  utc=%2\n"
        "lat=%3  lon=%4  alt=%5\n"
        "roll=%6  pitch=%7  heading=%8\n"
        "vn=%9  ve=%10  roti=%11\n"
        "ax=%12  ay=%13  aaz=%14\n"
        "status=%15  hdop=%16  headStd=%17")
        .arg(m["port"].toUInt())
        .arg(m["utcLocal"].toDouble(), 0, 'f', 2)
        .arg(m["latitude"].toDouble(), 0, 'f', 6)
        .arg(m["longitude"].toDouble(), 0, 'f', 6)
        .arg(m["altitude"].toDouble(), 0, 'f', 2)
        .arg(m["rollDeg"].toDouble(), 0, 'f', 2)
        .arg(m["pitchDeg"].toDouble(), 0, 'f', 2)
        .arg(m["headingDeg"].toDouble(), 0, 'f', 2)
        .arg(m["vnMps"].toDouble(), 0, 'f', 3)
        .arg(m["veMps"].toDouble(), 0, 'f', 3)
        .arg(m["roti"].toDouble(), 0, 'f', 4)
        .arg(m["ax"].toDouble(), 0, 'f', 4)
        .arg(m["ay"].toDouble(), 0, 'f', 4)
        .arg(m["aaz"].toDouble(), 0, 'f', 4)
        .arg(m["ginsStatus"].toUInt())
        .arg(m["hdop"].toDouble(), 0, 'f', 1)
        .arg(m["headStd"].toDouble(), 0, 'f', 2);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("GinsDataListModel 示例"));
    resize(620, 420);

    // 模型归属当前 Widget，随窗口生命周期自动释放。
    m_model = new GinsDataListModel(this);

    m_listView = new QListView;
    m_listView->setModel(m_model);
    m_listView->setModelColumn(0);
    m_listView->setFixedWidth(200);

    m_detailLabel = new QLabel(QStringLiteral("选中一行查看详情"));
    m_detailLabel->setWordWrap(true); //长字符串自动换行功能
    m_detailLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft); //内容对齐到顶部且靠左
    m_detailLabel->setMinimumHeight(160);
    m_detailLabel->setStyleSheet("QLabel { background: #f5f5f5; padding: 6px; border: 1px solid #ccc; }");

    m_hexEdit = new QTextEdit;
    m_hexEdit->setReadOnly(true); //设置为只读
    m_hexEdit->setFixedHeight(60);
    //QStringLiteral是在编译时创建字符串QString对象，不会再堆上开辟空间，具有高效的特点
    //setPlaceholderText是设置占位符
    m_hexEdit->setPlaceholderText(QStringLiteral("序列化 hex 数据"));

    m_statusLabel = new QLabel(QStringLiteral("就绪 | 数据条数: 0"));

    auto *btnAppend      = new QPushButton(QStringLiteral("添加"));
    auto *btnRemove      = new QPushButton(QStringLiteral("删除"));
    auto *btnEdit        = new QPushButton(QStringLiteral("修改"));
    auto *btnClear       = new QPushButton(QStringLiteral("清空"));
    auto *btnSerialize   = new QPushButton(QStringLiteral("序列化"));
    auto *btnDeserialize = new QPushButton(QStringLiteral("反序列化"));

    auto *btnLayout = new QVBoxLayout;
    btnLayout->addWidget(btnAppend);
    btnLayout->addWidget(btnRemove);
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnClear);
    btnLayout->addSpacing(12);
    btnLayout->addWidget(btnSerialize);
    btnLayout->addWidget(btnDeserialize);
    btnLayout->addStretch();

    auto *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(m_detailLabel, 1);
    rightLayout->addWidget(new QLabel(QStringLiteral("序列化数据 (hex):")));
    rightLayout->addWidget(m_hexEdit);

    auto *topLayout = new QHBoxLayout;
    topLayout->addWidget(m_listView);
    topLayout->addLayout(btnLayout);
    topLayout->addLayout(rightLayout, 1);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addWidget(m_statusLabel);

    connect(btnAppend,      &QPushButton::clicked, this, &Widget::onAppend);
    connect(btnRemove,      &QPushButton::clicked, this, &Widget::onRemoveSelected);
    connect(btnEdit,        &QPushButton::clicked, this, &Widget::onEditSelected);
    connect(btnClear,       &QPushButton::clicked, this, &Widget::onClear);
    connect(btnSerialize,   &QPushButton::clicked, this, &Widget::onSerialize);
    connect(btnDeserialize, &QPushButton::clicked, this, &Widget::onDeserialize);

    // 监听选中项变化，刷新详情面板。
    connect(m_listView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &Widget::onSelectionChanged);

    connect(m_model, &GinsDataListModel::countChanged, this, [this]() {
        m_statusLabel->setText(
            QStringLiteral("数据条数: %1").arg(m_model->count()));
    });

    connect(m_model, &GinsDataListModel::errorOccurred, this,
            [this](int /*type*/, const QString &msg) {
        m_statusLabel->setText(QStringLiteral("错误: %1").arg(msg));
    });
}

void Widget::onAppend()
{
    // 每次追加使用不同序号，保证数据有明显变化。
    m_model->append(sampleData(m_sampleIndex++));
    m_model->printAllData();
}

void Widget::onRemoveSelected()
{
    QModelIndex idx = m_listView->currentIndex();
    if (!idx.isValid()) return;
    m_model->remove(idx.row());
    m_detailLabel->setText(QStringLiteral("已删除"));
}

void Widget::onEditSelected()
{
    QModelIndex idx = m_listView->currentIndex();
    if (!idx.isValid()) return;

    // 这里直接构造一条新样本覆盖旧值，演示 set() 的更新能力。
    QVariantMap modified = sampleData(m_sampleIndex++);
    modified["ginsStatus"] = 6u;
    m_model->set(idx.row(), modified);
    onSelectionChanged();
}

void Widget::onClear()
{
    m_model->clear();
    m_detailLabel->setText(QStringLiteral("已清空"));
    m_hexEdit->clear();
}

void Widget::onSerialize()
{
    QModelIndex idx = m_listView->currentIndex();
    if (!idx.isValid()) return;

    // protobuf 二进制 -> hex，便于人工检查与复制。
    QByteArray bytes = m_model->serializeItem(idx.row());
    m_hexEdit->setPlainText(bytes.toHex(' '));
    m_statusLabel->setText(
        QStringLiteral("序列化完成, %1 字节").arg(bytes.size()));
}

void Widget::onDeserialize()
{
    // 容忍用户粘贴带空格的 hex，先规整再转换。
    QString hex = m_hexEdit->toPlainText().simplified().remove(' ');
    if (hex.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("hex 输入为空"));
        return;
    }

    QByteArray bytes = QByteArray::fromHex(hex.toLatin1());
    if (m_model->deserializeAndAppend(bytes))
        m_statusLabel->setText(QStringLiteral("反序列化并追加成功"));
}

void Widget::onSelectionChanged()
{
    QModelIndex idx = m_listView->currentIndex();
    if (!idx.isValid()) {
        m_detailLabel->setText(QStringLiteral("选中一行查看详情"));
        return;
    }

    QVariantMap map = m_model->get(idx.row());
    m_detailLabel->setText(mapToString(map));
}
