#ifndef CLOCKTOOLBAR_H
#define CLOCKTOOLBAR_H

#include <QWidget>
#include <QButtonGroup>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QPushButton>
#include <QSizePolicy>
#include <QTimer>
#include <QDebug>

class ClockToolbar : public QWidget
{
    Q_OBJECT

public:
    explicit ClockToolbar(QWidget *parent = nullptr);

    /// 顶栏三枚圆钮：夜间下带描边与着色；日间下恢复默认 SVG。
    void setAppNightMode(bool isNight);

    QLabel *clockLabel() const { return m_clockLabel; }
    QPushButton *nightThemeButton() const { return m_nightThemeButton; }
    QPushButton *dayThemeButton() const { return m_dayThemeButton; }
    QPushButton *ringAlertButton() const { return m_ringAlertButton; }

signals:
    void nightThemeButtonClicked();
    void dayThemeButtonClicked();
    void ringAlertButtonClicked();

private slots:
    void onNightThemeButtonClicked();
    void onDayThemeButtonClicked();
    void onRingAlertButtonClicked();
    void updateClockLabel();

private:
    void setupUi();
    void setupConnections();
    void refreshClockToolbarVisuals();

    bool m_appNightMode = true;

    QLabel *m_clockLabel = nullptr;
    QPushButton *m_nightThemeButton = nullptr;
    QPushButton *m_dayThemeButton = nullptr;
    QPushButton *m_ringAlertButton = nullptr;
};

#endif // CLOCKTOOLBAR_H
