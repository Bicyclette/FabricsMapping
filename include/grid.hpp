#ifndef GRID_HPP
#define GRID_HPP

#include <QtWidgets>
#include "node.hpp"

class Grid
{
public:
    Grid(QMainWindow* win, QGraphicsScene* arg, int dim = 5);
    Grid(QGraphicsScene* arg, int dim);
    int getGridDim() const;
    Node** getGrid() const;
    void toggleVisible();
    ~Grid();

private:
    bool visible;
    int grid_dim;
    Node** g;
    QGraphicsScene* scene;
};

class ChunkCorners
{
public:
    ChunkCorners();

    // ########## getters ##########
    int xTL() const;
    int yTL() const;
    int xTR() const;
    int yTR() const;
    int xBL() const;
    int yBL() const;
    int xBR() const;
    int yBR() const;

    // ########## calcul des coins de zone ##########
    void processChunkCoords(int g_dim, Node** g, int chunk);
    void processChunkCoordsImg(QImage img, int zone, int dim);

private:
    int corner_xTL;
    int corner_yTL;
    int corner_xTR;
    int corner_yTR;
    int corner_xBL;
    int corner_yBL;
    int corner_xBR;
    int corner_yBR;
};

#endif // GRID_HPP
