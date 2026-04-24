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
 * 自上而下：logo、home、map、ship、chart、camera、setting。
 */
class LeftWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeftWidget(QWidget *parent = nullptr);

    QLabel      *logoLabel()    const { return m_logoLabel; }
    QPushButton *homeButton()   const { return m_homeButton; }
    QPushButton *mapButton()    const { return m_mapButton; }
    QPushButton *shipButton()   const { return m_shipButton; }
    QPushButton *chartButton()  const { return m_chartButton; }
    QPushButton *cameraButton() const { return m_cameraButton; }
    QPushButton *settingButton()const { return m_settingButton; }

signals:
    void homeButtonClicked();
    void mapButtonClicked();
    void shipButtonClicked();
    void chartButtonClicked();
    void cameraButtonClicked();
    void settingButtonClicked();

private:
    void setupUi();
    void setupConnections();
    QPushButton *makeNavButton(const QString &text, const QSize &size);

    QLabel      *m_logoLabel     = nullptr;
    QPushButton *m_homeButton    = nullptr;
    QPushButton *m_mapButton     = nullptr;
    QPushButton *m_shipButton    = nullptr;
    QPushButton *m_chartButton   = nullptr;
    QPushButton *m_cameraButton  = nullptr;
    QPushButton *m_settingButton = nullptr;
};

#endif // LEFTWIDGET_H
