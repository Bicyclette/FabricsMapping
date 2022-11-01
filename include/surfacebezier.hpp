#ifndef SURFACEBEZIER_HPP
#define SURFACEBEZIER_HPP

#include <QtWidgets>
#include "grid.hpp"
#include "node.hpp"
#include "mappingmath.hpp"

class Window;

// Les Node ne seront pas ajoutés à la scene, seulement, les Edge
class SurfaceBezier
{
public:
    SurfaceBezier(Grid* g, Window* win, double grid_step);
    ~SurfaceBezier(); // supprime de la scene les arêtes puis clean la mémoire
    Node** getSurfaceNodes();
    int getDim() const;
    void toggleVisible();

private:
    bool visible;
    Grid* grid;
    Window* mapping;
    double step;
    int nbNodesPerDim;
    int surface_dim;
    Node** nodes;
    QVector<Edge*> edges;

    QPoint computeCoords(double t1, double t2);
    void draw();
};

#endif // SURFACEBEZIER_HPP
