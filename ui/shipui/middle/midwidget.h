#ifndef MIDWIDGET_H
#define MIDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
/**
 * 中间内容区域。
 * 固定尺寸 1440 × 1390，预留给实时地图显示，无布局。
 */
class MidWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MidWidget(QWidget *parent = nullptr);

    /** 对外预留：设置地图背景图。 */
    void setMapBackground(const QPixmap &pixmap);

    /** 返回内部用于显示地图的 label，后续可替换为真实地图控件。 */
    QLabel *mapSurface() const { return m_mapSurface; }

private:
    QLabel *m_mapSurface = nullptr;
};

#endif // MIDWIDGET_H
