#include "top/clocktoolbar.h"

namespace {
constexpr int kClockToolbarWidth  = 460;
constexpr int kClockToolbarHeight = 70;
constexpr int kRoundButtonSize    = 70;

QPushButton *makeRoundButton(const QString &text, QWidget *parent)
{
    auto *button = new QPushButton(text, parent);
    button->setCursor(Qt::PointingHandCursor);
    button->setFixedSize(kRoundButtonSize, kRoundButtonSize);
    button->setFlat(true);
    button->setObjectName(QStringLiteral("actionButton"));
    return button;
}
} // namespace

ClockToolbar::ClockToolbar(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
    updateClockLabel();

    auto *clockTimer = new QTimer(this);
    clockTimer->setInterval(1000);
    connect(clockTimer, &QTimer::timeout, this, &ClockToolbar::updateClockLabel);
    clockTimer->start();
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

    m_nightThemeButton = makeRoundButton(QStringLiteral("🌙"), this);
    m_nightThemeButton->setObjectName(QStringLiteral("nightModeButton"));
    m_nightThemeButton->setCheckable(true);
    m_nightThemeButton->setChecked(true);

    m_dayThemeButton = makeRoundButton(QStringLiteral("☀"), this);
    m_dayThemeButton->setObjectName(QStringLiteral("dayModeButton"));
    m_dayThemeButton->setCheckable(true);

    m_ringAlertButton = makeRoundButton(QStringLiteral("🔔"), this);
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
