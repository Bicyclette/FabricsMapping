#ifndef PIXMAP_HPP
#define PIXMAP_HPP

#include <QtWidgets>

class Window;

class Pixmap : public QGraphicsPixmapItem
{
public:
    Pixmap(QPixmap img, Window* win);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);//AJOUT
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);//AJOUT

private:
    Window* mapping;
    //CI-DESSOUS LES AJOUTS
    int cursorX;
    int cursorY;
    int deltaX;
    int deltaY;
    bool navigate;
    QGraphicsView* view;
    QScrollBar* v;
    QScrollBar* h;
};

#endif // PIXMAP_HPP
