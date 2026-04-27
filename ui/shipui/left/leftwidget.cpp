#include "left/leftwidget.h"

#include "main/icondye.h"

#include <QAbstractButton>
#include <algorithm>

namespace {
constexpr int kLeftWidgetWidth   = 207;
constexpr int kLeftWidgetHeight  = 1509;

constexpr int kLogoSize          = 114;
constexpr int kNavButtonWidth    = 96;   // 蓝色圆角矩形外框尺寸（正方形）
constexpr int kNavButtonHeight   = 96;
constexpr int kNavIconSize       = 73;   // 图标保持原尺寸，矩形大于图标形成边距
constexpr int kSettingBtnWidth   = 96;
constexpr int kSettingBtnHeight  = 96;
constexpr int kSettingIconSize   = 73;

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

QPushButton *LeftWidget::makeNavButton(const QIcon &icon, const QSize &size)
{
    auto *button = new QPushButton(this);
    button->setIcon(icon);
    button->setIconSize(QSize(kNavIconSize, kNavIconSize));
    button->setFixedSize(size);
    button->setCursor(Qt::PointingHandCursor);
    button->setCheckable(true);
    button->setFlat(true);
    button->setObjectName(QStringLiteral("navButton"));
    return button;
}

void LeftWidget::setAppNightMode(bool isNight)
{
    m_appNightMode = isNight;
    const QString logoPath = isNight ? QStringLiteral(":/src/logo.svg") : QStringLiteral(":/src/2.svg");
    const QPixmap lpm = loadEmbeddedPixmap(logoPath, QSize(kLogoSize, kLogoSize));
    if (!lpm.isNull() && m_logoLabel) {
        m_logoLabel->setPixmap(lpm);
    }
    refreshNavButtonIcons();
}

void LeftWidget::refreshNavButtonIcons()
{
    const bool night = m_appNightMode;

    const QSize navIconSize(kNavIconSize, kNavIconSize);
    auto applyGroup = [&](QPushButton *b, const QString &path) {
        const bool   selected = b->isChecked();
        const QColor c = selected ? QColor(0xFF, 0xFF, 0xFF)
                                  : (night ? QColor(0x73, 0x73, 0x73) : QColor(0xAB, 0xC2, 0xF2));
        b->setIcon(iconDyedFromResource(path, navIconSize, c));
        b->setIconSize(navIconSize);
    };

    applyGroup(m_homeButton, m_resHome);
    applyGroup(m_mapButton, m_resMap);
    applyGroup(m_shipButton, m_resShip);
    applyGroup(m_dockingButton, m_resDocking);
    applyGroup(m_cameraButton, m_resCamera);

    {
        const QSize settingIconSize(kSettingIconSize, kSettingIconSize);
        const QColor c = m_settingPressed ? QColor(0xFF, 0xFF, 0xFF)
                                          : (night ? QColor(0x73, 0x73, 0x73) : QColor(0xAB, 0xC2, 0xF2));
        m_settingButton->setIcon(iconDyedFromResource(m_resSetting, settingIconSize, c));
        m_settingButton->setIconSize(settingIconSize);
    }
}

void LeftWidget::setupUi()
{
    m_resHome    = QStringLiteral(":/src/home.svg");
    m_resMap     = QStringLiteral(":/src/map.svg");
    m_resShip    = QStringLiteral(":/src/ferry.svg");
    m_resDocking = QStringLiteral(":/src/docking.svg");
    m_resCamera  = QStringLiteral(":/src/camera.svg");
    m_resSetting = QStringLiteral(":/src/setting.svg");

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_logoLabel = new QLabel(this);
    m_logoLabel->setObjectName(QStringLiteral("logoLabel"));
    m_logoLabel->setFixedSize(kLogoSize, kLogoSize);
    m_logoLabel->setAlignment(Qt::AlignCenter);
    m_logoLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    {
        const QPixmap lpm = loadEmbeddedPixmap(QStringLiteral(":/src/logo.svg"),
                                               QSize(kLogoSize, kLogoSize));
        if (!lpm.isNull()) {
            m_logoLabel->setPixmap(lpm);
        }
    }

    m_homeButton    = makeNavButton(QIcon(m_resHome), QSize(kNavButtonWidth, kNavButtonHeight));
    m_mapButton     = makeNavButton(QIcon(m_resMap), QSize(kNavButtonWidth, kNavButtonHeight));
    m_shipButton    = makeNavButton(QIcon(m_resShip), QSize(kNavButtonWidth, kNavButtonHeight));
    m_dockingButton = makeNavButton(QIcon(m_resDocking), QSize(kNavButtonWidth, kNavButtonHeight));
    m_cameraButton  = makeNavButton(QIcon(m_resCamera), QSize(kNavButtonWidth, kNavButtonHeight));
    m_settingButton = makeNavButton(QIcon(m_resSetting), QSize(kSettingBtnWidth, kSettingBtnHeight));
    m_settingButton->setCheckable(false);

    auto *navGroup = new QButtonGroup(this);
    navGroup->setExclusive(true); //同一组内只有一个按键可以处于被选中状态
    for (auto *btn : {m_homeButton, m_mapButton, m_shipButton, m_dockingButton, m_cameraButton}) {
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
    layout->addWidget(m_dockingButton,  0, Qt::AlignHCenter);
    layout->addSpacing(kNavGap);
    layout->addWidget(m_cameraButton, 0, Qt::AlignHCenter);
    layout->addStretch(1);
    layout->addWidget(m_settingButton, 0, Qt::AlignHCenter);
    layout->addSpacing(40);

    refreshNavButtonIcons();
}

void LeftWidget::setupConnections()
{
    for (auto *b : {m_homeButton, m_mapButton, m_shipButton, m_dockingButton, m_cameraButton}) {
        connect(b, &QAbstractButton::toggled, this, [this] { refreshNavButtonIcons(); });
    }
    connect(m_settingButton, &QAbstractButton::pressed, this, [this] {
        m_settingPressed = true;
        refreshNavButtonIcons();
    });
    connect(m_settingButton, &QAbstractButton::released, this, [this] {
        m_settingPressed = false;
        refreshNavButtonIcons();
    });

    connect(m_homeButton,    &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: home";    emit homeButtonClicked(); });
    connect(m_mapButton,     &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: map";     emit mapButtonClicked(); });
    connect(m_shipButton,    &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: ship";    emit shipButtonClicked(); });
    connect(m_dockingButton,   &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: docking";   emit dockingButtonClicked(); });
    connect(m_cameraButton,  &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: camera";  emit cameraButtonClicked(); });
    connect(m_settingButton, &QPushButton::clicked, this, [this]{ qDebug() << "LeftWidget: setting"; emit settingButtonClicked(); });
}
