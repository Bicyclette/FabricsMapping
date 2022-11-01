#ifndef NODE_HPP
#define NODE_HPP

#include <QtWidgets>
#include "edge.hpp"

class Node: public QGraphicsItem
{
public:
    Node(QGraphicsScene* arg);
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    QPainterPath shape() const;
    void addEdge(Edge* e);
    QList<Edge*> getEdgeList() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
    QPointF position;
    QGraphicsScene* scene;
    QList<Edge*> edgeList;
};

#endif // NODE_HPP
