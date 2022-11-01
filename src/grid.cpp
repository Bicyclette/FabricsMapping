#include "grid.hpp"
#include <iostream>

Grid::Grid(QMainWindow* win, QGraphicsScene* arg, int dim) : g(nullptr)
{
    visible = true;
    // dimension de la grille
    if(dim <= 1)
    {
        QMessageBox::warning(win, "Echec de création d'une grille de mapping", "Vous devez renseigner une dimension strictement positive et supérieure à 1 !");
        return;
    }
    grid_dim = dim;

    // on vérifie qu'il y a au moins un objet QGraphicsItem dans la scene
    scene = arg;
    QList<QGraphicsItem*> itemList = scene->items();
    if(itemList.size() >= 1)
    {
        // allocation des noeuds de la grille
        g = new Node*[grid_dim * grid_dim];
        if(g == nullptr)
            exit(EXIT_FAILURE);

        // définition des positions pour chaque noeud
        QRectF img_bbox = itemList.at(0)->boundingRect();
        int img_width = img_bbox.width();
        int img_height = img_bbox.height();

        int dimension = 0;
        int space = 0;

        // quelle dimension est la valeur discriminante pour les dimensions de la grille
        if(img_width < img_height)
            dimension = img_width * 0.6;
        else if(img_height < img_width)
            dimension = img_height * 0.6;
        else
            dimension = img_width * 0.6;

        // espacement en pixels entre chaque noeud
        space = dimension / (grid_dim - 1);

        // position du noeud supérieur gauche
        int x = (img_width - dimension) / 2;
        int y = (img_height - dimension) / 2;
        int initX = x;

        for(int i = 0; i < grid_dim * grid_dim; i++)
        {
            if((i % grid_dim) == 0 && i != 0)
            {
                x = initX;
                y += space;
            }
            g[i] = new Node(scene);
            scene->addItem(g[i]);
            g[i]->setPos(x, y);
            x += space;
        }

        // ajout des arêtes
        int line = 1;

        for(int i = 0; i < grid_dim * grid_dim; i++)
        {
            if((line % grid_dim) == 0 && line != 0)
            {
                // y a t'il une ligne en dessous ?
                if((i + 1) < grid_dim * grid_dim)
                {
                    Edge* edgeV = new Edge(scene, g[i], g[i + grid_dim]);
                    scene->addItem(edgeV);
                }
                // sinon on ne fait rien
                line++;
            }

            else if(((i + 1) % grid_dim) != 0)
            {
                Edge* edgeH = new Edge(scene, g[i], g[i+1]);
                scene->addItem(edgeH);

                if((i + grid_dim) < grid_dim * grid_dim)
                {
                    Edge* edgeV = new Edge(scene, g[i], g[i + grid_dim]);
                    scene->addItem(edgeV);
                }

                line++;
            }
        }
    }

    else
    {
        QMessageBox::warning(win, "Echec de création d'une grille de mapping", "Vous devez ouvrir une image sur laquelle travailler avant de pouvoir créer une grille de mapping.");
    }
}

Grid::Grid(QGraphicsScene* arg, int dim)
{
    visible = true;
    grid_dim = dim;
    g = new Node*[grid_dim * grid_dim];
    scene = arg;
}

int Grid::getGridDim() const
{
    return grid_dim;
}

Node** Grid::getGrid() const
{
    return g;
}

void Grid::toggleVisible()
{
    int nbNodes = grid_dim * grid_dim;

    if(visible)
    {
        for(int i = 0; i < nbNodes; i++)
        {
            QList<Edge*> eList = g[i]->getEdgeList();

            foreach(Edge* e, eList)
            {
                e->setVisible(false);
            }
            g[i]->setVisible(false);
        }
        visible = false;
    }
    else
    {
        for(int i = 0; i < nbNodes; i++)
        {
            QList<Edge*> eList = g[i]->getEdgeList();

            foreach(Edge* e, eList)
            {
                e->setVisible(true);
            }
            g[i]->setVisible(true);
        }
        visible = true;
    }
}

Grid::~Grid()
{
    if(g != nullptr)
    {
        // Suppression des noeuds et arêtes de la scene
        int nbNodes = grid_dim * grid_dim;
        QList<Edge*> eList;
        QList<QGraphicsItem*> itemList;

        for(int n = 0; n < nbNodes; n++)
        {
            // arêtes
            eList = g[n]->getEdgeList();
            foreach(Edge* e, eList)
            {
                itemList = scene->items();
                if(itemList.contains(e))
                {
                    scene->removeItem(e);
                    delete e;
                }
            }
            // noeud
            scene->removeItem(g[n]);
        }

        for(int i = 0; i < grid_dim; i++)
            delete g[i];
        delete[] g;
    }
}

ChunkCorners::ChunkCorners() :
    corner_xTL(0),
    corner_yTL(0),
    corner_xTR(0),
    corner_yTR(0),
    corner_xBL(0),
    corner_yBL(0),
    corner_xBR(0),
    corner_yBR(0)
{}

int ChunkCorners::xTL() const { return corner_xTL; }

int ChunkCorners::yTL() const { return corner_yTL; }

int ChunkCorners::xTR() const { return corner_xTR; }

int ChunkCorners::yTR() const { return corner_yTR; }

int ChunkCorners::xBL() const { return corner_xBL; }

int ChunkCorners::yBL() const { return corner_yBL; }

int ChunkCorners::xBR() const { return corner_xBR; }

int ChunkCorners::yBR() const { return corner_yBR; }

void ChunkCorners::processChunkCoords(int g_dim, Node** g, int chunk)
{
    int chunk_line = 0, chunk_col = 0;
    int nbChunkPerLine = (g_dim - 1);

    // on détermine sur quelle ligne se trouve la zone dans la grille
    if((chunk % nbChunkPerLine) == 0)
    {
        // on est à la fin d'une ligne
        chunk_line = (chunk / nbChunkPerLine) - 1;
    }
    else
        chunk_line = (chunk / nbChunkPerLine);

    // on détermine sur quelle colonne se trouve la zone dans la grille
    if((chunk % nbChunkPerLine) == 0)
    {
        // fin de ligne => dernière colonne
        chunk_col = nbChunkPerLine;
    }
    else
        chunk_col = chunk % nbChunkPerLine;

    // on peut maintenant parcourir les noeuds de la grille
    // reminder => top-left : (chunk_col - 1, chunk_line)
    //          => top-right : (chunk_col, chunk_line)
    //          => bot-left : (chunk_col - 1, chunk_line + 1)
    //          => bot-right : (chunk_col, chunk_line + 1)
    int curr_line = 0;
    int curr_col = 0;
    for(int pos = 0; pos < g_dim * g_dim; pos++)
    {
        if((pos % g_dim) == 0 && pos != 0)
        {
            curr_line++;
            curr_col = 0;
        }

        if(curr_col == (chunk_col - 1) && curr_line == chunk_line)
        {
            // coin top-left
            corner_xTL = g[pos]->x();
            corner_yTL = g[pos]->y();
        }

        else if(curr_col == chunk_col && curr_line == chunk_line)
        {
            // coin top-right
            corner_xTR = g[pos]->x();
            corner_yTR = g[pos]->y();
        }

        else if(curr_col == (chunk_col - 1) && curr_line == (chunk_line + 1))
        {
            // coin bot-left
            corner_xBL = g[pos]->x();
            corner_yBL = g[pos]->y();
        }

        else if(curr_col == chunk_col && curr_line == (chunk_line + 1))
        {
            // coin bot-right
            corner_xBR = g[pos]->x();
            corner_yBR = g[pos]->y();
        }

        curr_col++;
    }
}

void ChunkCorners::processChunkCoordsImg(QImage img, int zone, int dim)
{
    double width = static_cast<double>(img.width() - 1);
    double height = static_cast<double>(img.height() - 1);
    int line = zone / (dim - 1);
    int col = zone % (dim - 1);

    if(col == 0)
    {
        line--;
        col = dim - 2;
    }
    else
        col--;

    // top-left
    corner_xTL = col * (width / static_cast<double>(dim - 1));
    corner_yTL = line * (height / static_cast<double>(dim - 1));
    // top-right
    corner_xTR = (col + 1) * (width / static_cast<double>(dim - 1));
    corner_yTR = line * (height / static_cast<double>(dim - 1));
    // bot-left
    corner_xBL = col * (width / static_cast<double>(dim - 1));
    corner_yBL = (line + 1) * (height / static_cast<double>(dim - 1));
    // bot-right
    corner_xBR = (col + 1) * (width / static_cast<double>(dim - 1));
    corner_yBR = (line + 1) * (height / static_cast<double>(dim - 1));
}
