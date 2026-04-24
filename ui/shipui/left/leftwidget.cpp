#include "left/leftwidget.h"

namespace {
constexpr int kLeftWidgetWidth   = 207;
constexpr int kLeftWidgetHeight  = 1509;

constexpr int kLogoSize          = 114;
constexpr int kNavButtonWidth    = 73;
constexpr int kNavButtonHeight   = 75;
constexpr int kSettingBtnWidth   = 66;
constexpr int kSettingBtnHeight  = 85;

constexpr int kTopSpacing        = 92;   // Figma logo.top(1614) - LeftWidget.top(1522)
constexpr int kLogoToNavSpacing  = 139;  // 1867 - (1614 + 114)
constexpr int kNavGap            = 100;  // 用户指定按钮之间间距
} // namespace

LeftWidget::LeftWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("leftWidget"));
    // 使样式表中的 background-color / border-radius 能应用到裸 QWidget
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedSize(kLeftWidgetWidth, kLeftWidgetHeight);
    setupUi();
    setupConnections();
}

QPushButton *LeftWidget::makeNavButton(const QString &text, const QSize &size)
{
    auto *button = new QPushButton(text, this);
    button->setFixedSize(size);
    button->setCursor(Qt::PointingHandCursor);
    button->setCheckable(true);
    button->setFlat(true);
    button->setObjectName(QStringLiteral("navButton"));
    return button;
}

void LeftWidget::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_logoLabel = new QLabel(this);
    m_logoLabel->setObjectName(QStringLiteral("logoLabel"));
    m_logoLabel->setFixedSize(kLogoSize, kLogoSize);
    m_logoLabel->setAlignment(Qt::AlignCenter);
    m_logoLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    QIcon logoIcon(QStringLiteral(":/src/logo.png"));
    if (!logoIcon.isNull()) {
        m_logoLabel->setPixmap(logoIcon.pixmap(kLogoSize, kLogoSize));
    }

    m_homeButton    = makeNavButton(QStringLiteral("⌂"), QSize(kNavButtonWidth, kNavButtonHeight));
    m_mapButton     = makeNavButton(QStringLiteral("🗺"), QSize(kNavButtonWidth, kNavButtonHeight));
    m_shipButton    = makeNavButton(QStringLiteral("⛴"), QSize(kNavButtonWidth, kNavButtonHeight));
    m_chartButton   = makeNavButton(QStringLiteral("▥"), QSize(kNavButtonWidth, kNavButtonHeight));
    m_cameraButton  = makeNavButton(QStringLiteral("📷"), QSize(kNavButtonWidth, kNavButtonHeight));
    m_settingButton = makeNavButton(QStringLiteral("⚙"), QSize(kSettingBtnWidth, kSettingBtnHeight));

    auto *navGroup = new QButtonGroup(this);
    navGroup->setExclusive(true); //同一组内只有一个按键可以处于被选中状态
    for (auto *btn : {m_homeButton, m_mapButton, m_shipButton, m_chartButton, m_cameraButton}) {
        navGroup->addButton(btn);
    }
    m_mapButton->setChecked(true); //默认打开界面选中map选项

    layout->addSpacing(kTopSpacing);
    layout->addWidget(m_logoLabel, 0, Qt::AlignHCenter);
    layout->addSpacing(kLogoToNavSpacing);
    layout->addWidget(m_homeButton,   0, Qt::AlignHCenter);
    layout->addSpacing(kNavGap);
    layout->addWidget(m_mapButton,    0, Qt::AlignHCenter);
    layout->addSpacing(kNavGap);
    layout->addWidget(m_shipButton,   0, Qt::AlignHCenter);
    layout->addSpacing(kNavGap);
    layout->addWidget(m_chartButton,  0, Qt::AlignHCenter);
    layout->addSpacing(kNavGap);
    layout->addWidget(m_cameraButton, 0, Qt::AlignHCenter);
    layout->addStretch(1);
    layout->addWidget(m_settingButton, 0, Qt::AlignHCenter);
    layout->addSpacing(40);
}

void LeftWidget::setupConnections()
{
    connect(m_homeButton,    &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: home";    emit homeButtonClicked(); });
    connect(m_mapButton,     &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: map";     emit mapButtonClicked(); });
    connect(m_shipButton,    &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: ship";    emit shipButtonClicked(); });
    connect(m_chartButton,   &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: chart";   emit chartButtonClicked(); });
    connect(m_cameraButton,  &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: camera";  emit cameraButtonClicked(); });
    connect(m_settingButton, &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: setting"; emit settingButtonClicked(); });
}
