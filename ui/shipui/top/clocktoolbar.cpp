#include "top/clocktoolbar.h"

#include "main/icondye.h"

#include <QAbstractButton>
#include <QButtonGroup>
#include <QColor>
#include <QIcon>
#include <QString>

namespace {
constexpr int kClockToolbarWidth  = 500;
constexpr int kClockToolbarHeight = 90;
constexpr int kRoundButtonSize    = 90;
constexpr int kRoundIconSize      = 60;  // 图标尺寸，小于按钮使圆形背景更明显

const char *kResMoon   = ":/src/moon.fill 2.svg";
const char *kResSun    = ":/src/sun.max.fill 1.svg";
const char *kResBell   = ":/src/bell.fill 1.svg";

QPushButton *makeRoundButton(const QIcon &icon, QWidget *parent)
{
    auto *button = new QPushButton(parent);
    button->setIcon(icon);
    button->setIconSize(QSize(kRoundIconSize, kRoundIconSize));
    button->setCursor(Qt::PointingHandCursor);
    button->setFixedSize(kRoundButtonSize, kRoundButtonSize);
    button->setFlat(true);
    return button;
}

void applyDayStyle(QPushButton *b, const char *qrc)
{
    const QString name  = b->objectName();
    const bool    isOn  = b->isChecked();
    const QString bg    = isOn ? QStringLiteral("#017FFF") : QStringLiteral("#F3F6FF");
    const QColor  iconC = isOn ? QColor(0xFF, 0xFF, 0xFF) : QColor(0xAB, 0xC2, 0xF2);
    b->setStyleSheet(QStringLiteral(
        "QPushButton#%1{"
        "background-color:%2;"
        "border:none;"
        "border-radius:45px;"
        "min-width:90px;max-width:90px;min-height:90px;max-height:90px;"
        "padding:0px;"
        "}"
    ).arg(name, bg));
    b->setIcon(iconDyedFromResource(QString::fromUtf8(qrc), QSize(kRoundIconSize, kRoundIconSize), iconC));
    b->setIconSize(QSize(kRoundIconSize, kRoundIconSize));
}

void applyNightStyleButton(QPushButton *b, const char *qrc, bool isOn)
{
    const QString name = b->objectName();
    const QString bg   = isOn ? QStringLiteral("#FFFFFF") : QStringLiteral("#323232");
    const QColor  iconC = isOn ? QColor(0x32, 0x32, 0x32) : QColor(0xE0, 0xE0, 0xE0);
    b->setStyleSheet(QStringLiteral(
        "QPushButton#%1{"
        "background-color:%2;"
        "border:none;"
        "border-radius:45px;"
        "min-width:90px;max-width:90px;min-height:90px;max-height:90px;"
        "padding:0px;"
        "}"
    ).arg(name, bg));
    b->setIcon(iconDyedFromResource(QString::fromUtf8(qrc), QSize(kRoundIconSize, kRoundIconSize), iconC));
    b->setIconSize(QSize(kRoundIconSize, kRoundIconSize));
}
} // namespace

ClockToolbar::ClockToolbar(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
    updateClockLabel();
    setAppNightMode(m_appNightMode);

    auto *clockTimer = new QTimer(this);
    clockTimer->setInterval(1000);
    connect(clockTimer, &QTimer::timeout, this, &ClockToolbar::updateClockLabel);
    clockTimer->start();
}

void ClockToolbar::setAppNightMode(bool isNight)
{
    m_appNightMode = isNight;
    refreshClockToolbarVisuals();
}

void ClockToolbar::refreshClockToolbarVisuals()
{
    if (!m_nightThemeButton || !m_dayThemeButton || !m_ringAlertButton) {
        return;
    }
    if (!m_appNightMode) {
        applyDayStyle(m_nightThemeButton, kResMoon);
        applyDayStyle(m_dayThemeButton, kResSun);
        applyDayStyle(m_ringAlertButton, kResBell);
        return;
    }
    applyNightStyleButton(m_nightThemeButton, kResMoon, m_nightThemeButton->isChecked());
    applyNightStyleButton(m_dayThemeButton, kResSun, m_dayThemeButton->isChecked());
    applyNightStyleButton(m_ringAlertButton, kResBell, m_ringAlertButton->isChecked());
}

void ClockToolbar::setupUi()
{
    setObjectName(QStringLiteral("clockToolbar"));
    setFixedSize(kClockToolbarWidth, kClockToolbarHeight);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(16);

    m_clockLabel = new QLabel(this);
    m_clockLabel->setObjectName(QStringLiteral("topBarClockLabel"));
    m_clockLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_clockLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    layout->addWidget(m_clockLabel, 1, Qt::AlignVCenter);

    m_nightThemeButton = makeRoundButton(QIcon(QString::fromUtf8(kResMoon)), this);
    m_nightThemeButton->setObjectName(QStringLiteral("nightModeButton"));
    m_nightThemeButton->setCheckable(true);
    m_nightThemeButton->setChecked(true);

    m_dayThemeButton = makeRoundButton(QIcon(QString::fromUtf8(kResSun)), this);
    m_dayThemeButton->setObjectName(QStringLiteral("dayModeButton"));
    m_dayThemeButton->setCheckable(true);

    m_ringAlertButton = makeRoundButton(QIcon(QString::fromUtf8(kResBell)), this);
    m_ringAlertButton->setObjectName(QStringLiteral("ringAlertButton"));
    m_ringAlertButton->setCheckable(true);

    auto *themeGroup = new QButtonGroup(this);
    themeGroup->setExclusive(true);
    themeGroup->addButton(m_nightThemeButton);
    themeGroup->addButton(m_dayThemeButton);

    layout->addWidget(m_nightThemeButton, 0, Qt::AlignVCenter);
    layout->addWidget(m_dayThemeButton,   0, Qt::AlignVCenter);
    layout->addWidget(m_ringAlertButton,  0, Qt::AlignVCenter);
}

void ClockToolbar::setupConnections()
{
    connect(m_nightThemeButton, &QPushButton::clicked, this, &ClockToolbar::onNightThemeButtonClicked);
    connect(m_dayThemeButton,   &QPushButton::clicked, this, &ClockToolbar::onDayThemeButtonClicked);
    connect(m_ringAlertButton,  &QPushButton::clicked, this, &ClockToolbar::onRingAlertButtonClicked);

    for (auto *b : {m_nightThemeButton, m_dayThemeButton, m_ringAlertButton}) {
        connect(b, &QAbstractButton::toggled, this, [this] { refreshClockToolbarVisuals(); });
    }
}

void ClockToolbar::onNightThemeButtonClicked()
{
    emit nightThemeButtonClicked();
    qDebug() << "ClockToolbar::onNightThemeButtonClicked()";
}

void ClockToolbar::onDayThemeButtonClicked()
{
    emit dayThemeButtonClicked();
    qDebug() << "ClockToolbar::onDayThemeButtonClicked()";
}

void ClockToolbar::onRingAlertButtonClicked()
{
    emit ringAlertButtonClicked();
    qDebug() << "ClockToolbar::onRingAlertButtonClicked()";
}

void ClockToolbar::updateClockLabel()
{
    if (!m_clockLabel) {
        return;
    }
    m_clockLabel->setText(QLocale::c().toString(QDateTime::currentDateTime(), QStringLiteral("h:mm ap")));
}
