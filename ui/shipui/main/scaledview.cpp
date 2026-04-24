#include "scaledview.h"

static constexpr int kDesignWidth  = 2560;
static constexpr int kDesignHeight = 1600;

ScaledView::ScaledView(QWidget *parent)
    : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //关闭水平滚动条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //关闭垂直滚动条
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); //开启抗锯齿和图像平滑
    setFrameShape(QFrame::NoFrame); //关闭边框(无边框)
    setBackgroundBrush(QBrush(QColor("#1e1f23")));

    auto *scene = new QGraphicsScene(0, 0, kDesignWidth, kDesignHeight, this);
    setScene(scene);

    m_content = new ShipUiWidget;
    m_content->setFixedSize(kDesignWidth, kDesignHeight); // 设置ShipUiWidget固定大小
    scene->addWidget(m_content); // 连接ShipUiWidget与QGraphicsScene
}

void ScaledView::resizeEvent(QResizeEvent *event) //窗口发生改变时，自动调整ShipUiWidget的大小
{
    QGraphicsView::resizeEvent(event);
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio); //计算缩放比例，并保维持宽高比
}
