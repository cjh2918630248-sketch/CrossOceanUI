#ifndef SCALEDVIEW_H
#define SCALEDVIEW_H

#include <QGraphicsView>
#include <QBrush>
#include <QColor>
#include <QFrame>
#include <QGraphicsScene>
#include <QPainter>
#include <QResizeEvent>

#include "main/shipuiwidget.h"
/**
 * 等比缩放视图。
 * 将 ShipUiWidget 以设计稿逻辑尺寸（2560×1600）嵌入 QGraphicsScene，
 * 每次窗口 resize 时调用 fitInView 保持等比缩放。
 */
class ScaledView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ScaledView(QWidget *parent = nullptr);

    ShipUiWidget *shipUiWidget() const { return m_content; } //内联写法

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    ShipUiWidget *m_content = nullptr;
};

#endif // SCALEDVIEW_H
