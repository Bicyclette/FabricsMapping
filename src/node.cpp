#include "node.hpp"

Node::Node(QGraphicsScene* arg)
{
    scene = arg;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);
}

QRectF Node::boundingRect() const
{
    // carré de 11 par 11
    QRectF hitBox(-5, -5, 10, 10);
    return hitBox;
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-5, -5, 10, 10);
    return path;
}

void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);
    painter->drawEllipse(-5, -5, 10, 10);
}

void Node::addEdge(Edge* e)
{
    edgeList.push_back(e);
}

QList<Edge*> Node::getEdgeList() const
{
    return edgeList;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionHasChanged)
    {
        position = value.toPointF();
        QRectF sceneBoundingBox = scene->sceneRect();

        if( ! sceneBoundingBox.contains(position) )
        {
            position.setX(qMin(sceneBoundingBox.right(), qMax(position.x(), sceneBoundingBox.left())));
            position.setY(qMin(sceneBoundingBox.bottom(), qMax(position.x(), sceneBoundingBox.top())));
            return position;
        }

        // on redessine aussi les arêtes liées à ce point
        foreach(Edge* elem, edgeList)
        {
            elem->reshape();
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
