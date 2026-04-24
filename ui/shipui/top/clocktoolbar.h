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

    QLabel *m_clockLabel = nullptr;
    QPushButton *m_nightThemeButton = nullptr;
    QPushButton *m_dayThemeButton = nullptr;
    QPushButton *m_ringAlertButton = nullptr;
};

#endif // CLOCKTOOLBAR_H
