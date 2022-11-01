#include "edge.hpp"
#include "node.hpp"

Edge::Edge(QGraphicsScene* arg, Node* src, Node* dst, QColor c)
{
    setAcceptedMouseButtons(0);
    scene = arg;
    source = src;
    source->addEdge(this);
    dest = dst;
    dest->addEdge(this);
    setZValue(2);
    reshape();
    color = c;
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();
    else
    {
        if(src.x() <= dst.x() && src.y() <= dst.y())
            return QRectF(src.x(), src.y(), dst.x() - src.x(), dst.y() - src.y());
        else if(src.x() <= dst.x() && dst.y() <= src.y())
            return QRectF(src.x(), dst.y(), dst.x() - src.x(), src.y() - dst.y());
        else if(dst.x() <= src.x() && dst.y() <= src.y())
            return QRectF(dst.x(), dst.y(), src.x() - dst.x(), src.y() - dst.y());
        else if(dst.x() <= src.x() && src.y() <= dst.y())
            return QRectF(dst.x(), src.y(), src.x() - dst.x(), dst.y() - src.y());
    }
}

void Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (!source || !dest)
        return;

    QLineF line(src.x(), src.y(), dst.x(), dst.y());

    painter->setPen(QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
}

void Edge::reshape()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    src = line.p1();
    dst = line.p2();
}
