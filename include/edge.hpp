#ifndef EDGE_HPP
#define EDGE_HPP

#include <QtWidgets>

class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(QGraphicsScene* arg, Node* src, Node* dst, QColor c = Qt::black);
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void reshape();

private:
    QGraphicsScene* scene;
    Node* source;
    Node* dest;
    QPointF src;
    QPointF dst;
    QColor color;
};

#endif // EDGE_HPP
