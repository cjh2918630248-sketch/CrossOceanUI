#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QDebug>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
/**
 * 左侧导航栏。
 * 固定尺寸 207 × 1509（设计稿 2560×1600 画布坐标系）。
 * 自上而下：logo、home、map、ship、docking、camera、setting。
 */
class LeftWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeftWidget(QWidget *parent = nullptr);

    /// 夜间为 true 时：导航为灰/白且 logo 为 logo.svg；日间为 false 时：黑/白且 logo 为 2.svg
    void setAppNightMode(bool isNight);

    QLabel      *logoLabel()    const { return m_logoLabel; }
    QPushButton *homeButton()   const { return m_homeButton; }
    QPushButton *mapButton()    const { return m_mapButton; }
    QPushButton *shipButton()   const { return m_shipButton; }
    QPushButton *dockingButton() const { return m_dockingButton; }
    QPushButton *cameraButton() const { return m_cameraButton; }
    QPushButton *settingButton()const { return m_settingButton; }

signals:
    void homeButtonClicked();
    void mapButtonClicked();
    void shipButtonClicked();
    void dockingButtonClicked();
    void cameraButtonClicked();
    void settingButtonClicked();

private:
    void setupUi();
    void setupConnections();
    void refreshNavButtonIcons();
    QPushButton *makeNavButton(const QIcon &icon, const QSize &size);

    bool m_appNightMode    = true;
    bool m_settingPressed  = false;
    QString m_resHome;
    QString m_resMap;
    QString m_resShip;
    QString m_resDocking;
    QString m_resCamera;
    QString m_resSetting;

    QLabel      *m_logoLabel     = nullptr;
    QPushButton *m_homeButton    = nullptr;
    QPushButton *m_mapButton     = nullptr;
    QPushButton *m_shipButton    = nullptr;
    QPushButton *m_dockingButton = nullptr;
    QPushButton *m_cameraButton  = nullptr;
    QPushButton *m_settingButton = nullptr;
};

#endif // LEFTWIDGET_H
