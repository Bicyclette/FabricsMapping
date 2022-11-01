#include "surfacebezier.hpp"
#include "window.hpp"

SurfaceBezier::SurfaceBezier(Grid* g, Window* win, double grid_step)
{
    visible = true;
    grid = g;
    mapping = win;
    QGraphicsScene* scene = mapping->getScene();
    step = grid_step;
    nbNodesPerDim = static_cast<int>((1.0 / step) + 1);
    surface_dim = nbNodesPerDim;
    nodes = new Node*[surface_dim * surface_dim];

    // trace de la surface
    double t1 = 0.0;
    double t2 = 0.0;
    int n = 0;

    for(t2 = 0.0; t2 <= (1.0 + (step / 2.0)); t2 += step)
    {
        for(t1 = 0.0; t1 <= (1.0 + (step / 2.0)); t1 += step)
        {
            QPoint p = computeCoords(t1, t2);
            nodes[n] = new Node(scene);
            nodes[n]->setPos(p.x(), p.y());
            n++;
        }
    }

    draw();
}

SurfaceBezier::~SurfaceBezier()
{
    int nbEdges = edges.size();
    QGraphicsScene* scene = mapping->getScene();

    for(int i = 0; i < nbEdges; i++)
    {
        scene->removeItem(edges.at(i));
    }
    edges.clear();

    delete[] nodes;
}

QPoint SurfaceBezier::computeCoords(double t1, double t2)
{
    Node** nodes = grid->getGrid();
    int grid_dim = grid->getGridDim();
    QPoint p(0, 0);

    for(int i = 0; i < grid_dim; i++) // colonne
    {
        for(int j = 0; j < grid_dim; j++) // ligne
        {
            int pos = (j * grid_dim) + i;
            Node* n = nodes[pos];
            p.setX(p.x() + (n->x() * MappingMath::polyBernstein(t1, grid_dim - 1, i) * MappingMath::polyBernstein(t2, grid_dim - 1, j)));
            p.setY(p.y() + (n->y() * MappingMath::polyBernstein(t1, grid_dim - 1, i) * MappingMath::polyBernstein(t2, grid_dim - 1, j)));
        }
    }

    return p;
}

void SurfaceBezier::draw()
{
    QGraphicsScene* scene = mapping->getScene();
    int nbNodes = nbNodesPerDim * nbNodesPerDim;

    for(int i = 0; i < nbNodes; i++)
    {
        if((i + 1) < nbNodes && ( (i + 1) % nbNodesPerDim != 0 ))
        {
            if((i + nbNodesPerDim) >= nbNodes)
            {
                Edge* e = new Edge(scene, nodes[i], nodes[i + 1], Qt::red);
                edges.push_back(e);
                scene->addItem(e);
            }
            else
            {
                Edge* e1 = new Edge(scene, nodes[i], nodes[i + 1], Qt::red);
                Edge* e2 = new Edge(scene, nodes[i], nodes[i + nbNodesPerDim], Qt::red);
                edges.push_back(e1);
                edges.push_back(e2);
                scene->addItem(e1);
                scene->addItem(e2);
            }
        }

        else if((i + 1) < nbNodes && ( (i + 1) % nbNodesPerDim == 0 ) && (i + nbNodesPerDim) < nbNodes)
        {
            Edge* e = new Edge(scene, nodes[i], nodes[i + nbNodesPerDim], Qt::red);
            edges.push_back(e);
            scene->addItem(e);
        }
    }
}

Node** SurfaceBezier::getSurfaceNodes()
{
    return nodes;
}

int SurfaceBezier::getDim() const
{
    return nbNodesPerDim;
}

void SurfaceBezier::toggleVisible()
{
    int nbNodes = surface_dim * surface_dim;

    if(visible)
    {
        for(int i = 0; i < nbNodes; i++)
        {
            QList<Edge*> eList = nodes[i]->getEdgeList();

            foreach(Edge* e, eList)
            {
                e->setVisible(false);
            }
            nodes[i]->setVisible(false);
        }
        visible = false;
    }
    else
    {
        for(int i = 0; i < nbNodes; i++)
        {
            QList<Edge*> eList = nodes[i]->getEdgeList();

            foreach(Edge* e, eList)
            {
                e->setVisible(true);
            }
            nodes[i]->setVisible(true);
        }
        visible = true;
    }
}
