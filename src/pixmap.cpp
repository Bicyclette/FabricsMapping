#include "pixmap.hpp"
#include "window.hpp"
#include <iostream>

Pixmap::Pixmap(QPixmap img, Window* win) : QGraphicsPixmapItem(QPixmap(img)), mapping(win)
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::MidButton | Qt::MiddleButton);
    cursorX = 0;
    cursorY = 0;
    deltaX = 0;
    deltaY = 0;
    navigate = false;
    view = nullptr;
}

void Pixmap::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    bool maskTool = mapping->getMaskTool();
    QList<Node*>* selectionNodes = mapping->getSelectionNodes();
    QList<Edge*>* selectionEdges = mapping->getSelectionEdges();
    QGraphicsScene* scene = mapping->getScene();
    view = scene->views().at(0);
    v = view->verticalScrollBar();
    h = view->horizontalScrollBar();

    if(maskTool && event->button() == Qt::LeftButton)
    {
        int mouseX = event->scenePos().x();
        int mouseY = event->scenePos().y();
        Node* n = new Node(scene);
        n->setPos(mouseX, mouseY);
        selectionNodes->push_back(n);
        scene->addItem(n);

        if(selectionNodes->size() >= 2)
        {
            if(selectionEdges->size() > 0)
            {
                int nbEdges = selectionEdges->size();
                for(int e = 0; e < nbEdges; e++)
                {
                    scene->removeItem(selectionEdges->at(e));
                }
                selectionEdges->clear();
            }

            int nbElems = selectionNodes->size();

            for(int i = 0; i < nbElems; i++)
            {
                if((i + 1) < nbElems)
                {
                    Edge* e = new Edge(scene, selectionNodes->at(i), selectionNodes->at(i + 1));
                    selectionEdges->push_back(e);
                    scene->addItem(e);
                }
            }
        }

        update();
        QGraphicsPixmapItem::mousePressEvent(event);
    }

    else if(event->button() == Qt::MiddleButton && ! navigate)
    {
        cursorX = event->scenePos().x();
        cursorY = event->scenePos().y();
        navigate = true;
        setCursor(Qt::OpenHandCursor);
    }
}

void Pixmap::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(navigate)
    {
        deltaX = cursorX - event->scenePos().x();
        deltaY = cursorY - event->scenePos().y();

        h->setValue(h->value() + deltaX);
        v->setValue(v->value() + deltaY);
    }
}

void Pixmap::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MiddleButton && navigate)
    {
        bool maskTool = mapping->getMaskTool();
        navigate = false;
        if(maskTool)
            setCursor(Qt::CrossCursor);
        else
            setCursor(Qt::ArrowCursor);
    }
}
