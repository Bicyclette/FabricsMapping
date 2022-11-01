#include "window.hpp"
#include "radiobuttonmotif.hpp"
#include "pixmap.hpp"
#include "mappingdialog.hpp"
#include "savetemplatedialog.hpp"

Window::Window()
{
    setWindowTitle("FabricsMapping");
    maskTool = false;
    g = nullptr;
    masque_courant = nullptr;
    templateToSave = 0;
    bSurface = nullptr;
    // ########## INIT DES MENUS ##########
    menu_fichier = menuBar()->addMenu("fichier");
    menu_vue = menuBar()->addMenu("vue");
    menu_selection = menuBar()->addMenu("sélection");
    menu_grille = menuBar()->addMenu("grille");
    menu_motif = menuBar()->addMenu("motif");

    // ########## ASSOCIATION ENTRE MENU FICHIER ET ACTIONS ###########
    ouvrir_img = new QAction("ouvrir une image", this);
    sauvegarder = new QAction("sauvegarder", this);
    quitter = new QAction("quitter", this);

    menu_fichier->addAction(ouvrir_img);
    menu_fichier->addAction(sauvegarder);
    menu_fichier->addAction(quitter);

    // ########## ASSOCIATION ENTRE MENU SÉLECTION ET ACTIONS ##########
    nouveau_masque = new QAction("nouveau masque à partir d'une sélection de pixels", this);
    charger_masque = new QAction("charger un masque", this);
    sauvegarder_masque = new QAction("sauvegarder un masque", this);
    sauvegarder_masque->setEnabled(false);

    menu_selection->addAction(nouveau_masque);
    menu_selection->addAction(charger_masque);
    menu_selection->addAction(sauvegarder_masque);

    // ########## ASSOCIATION ENTRE MENU GRILLE ET ACTIONS ##########
    nouvelle_grille = new QAction("nouvelle grille", this);
    supprimer_grille = new QAction("supprimer la grille courante", this);
    supprimer_grille->setEnabled(false);
    charger_grille = new QAction("Charger une grille", this);
    toggle_grille = new QAction("activer/désactiver la visibilité de la grille", this);
    toggle_grille->setEnabled(false);
    sauvegarder_grille = new QAction("sauvegarder la grille", this);
    sauvegarder_grille->setEnabled(false);
    generate_bsurface = new QAction("générer une surface de bézier", this);
    generate_bsurface->setEnabled(false);
    toggle_bsurface = new QAction("activer/désactiver la visibilité de la surface de bézier", this);
    toggle_bsurface->setEnabled(false);
    map = new QAction("map", this);

    menu_grille->addAction(nouvelle_grille);
    menu_grille->addAction(supprimer_grille);
    menu_grille->addAction(charger_grille);
    menu_grille->addAction(toggle_grille);
    menu_grille->addAction(sauvegarder_grille);
    menu_grille->addAction(generate_bsurface);
    menu_grille->addAction(toggle_bsurface);
    menu_grille->addAction(map);

    // ########## ASSOCIATION ENTRE MENU MOTIF ET ACTIONS ##########
    ajouter_motif = new QAction("ajouter motif", this);

    menu_motif->addAction(ajouter_motif);

    // ########## INIT DE LA TAILLE DE LA FENÊTRE PRINCIPALE ##########
    QRect rect = QApplication::desktop()->availableGeometry(this);
    int screen_height = static_cast<int>(rect.height() * 0.95);
    int screen_width = static_cast<int>(rect.width() * 0.95);
    setFixedSize(screen_width, screen_height);

    // ########## ZONE CENTRALE ##########
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);

    // ########## DOCKS ##########
    motifs_tab = new QDockWidget("Motifs", this);
    motifs_tab->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, motifs_tab);
    motifs_tab->setFixedWidth(screen_width * 0.2);
    menu_vue->addAction(motifs_tab->toggleViewAction());

    masques_tab = new QDockWidget("Masques", this);
    masques_tab->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, masques_tab);
    masques_tab->setFixedWidth(screen_width * 0.2);
    menu_vue->addAction(masques_tab->toggleViewAction());

    layout_motifs = new QGridLayout();
    layout_masques = new QGridLayout();

    scroll_motifs = new QScrollArea(motifs_tab);
    scroll_masques = new QScrollArea(masques_tab);

    motifs_widget = new QWidget(scroll_motifs);
    //motifs_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    masques_widget = new QWidget(scroll_masques);
    //masques_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    motifs_tab->setWidget(scroll_motifs);
    masques_tab->setWidget(scroll_masques);

    motifs_widget->setLayout(layout_motifs);
    masques_widget->setLayout(layout_masques);

    scroll_motifs->setWidget(motifs_widget);
    scroll_motifs->setWidgetResizable(true);
    scroll_motifs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll_masques->setWidget(masques_widget);
    scroll_masques->setWidgetResizable(true);
    scroll_masques->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // ########## VARIABLES POUR LE FONCTIONNEMENT DE L'APPLICATION ##########
    image_item = nullptr;
    g = nullptr;

    // ########## CONNECT SIGNAUX ET SLOTS ##########
    connect(quitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ouvrir_img, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(ajouter_motif, SIGNAL(triggered()), this, SLOT(openMotif()));
    connect(nouvelle_grille, SIGNAL(triggered()), this, SLOT(creationGrille()));
    connect(nouveau_masque, SIGNAL(triggered()), this, SLOT(nouveauMasque()));
    connect(map, SIGNAL(triggered()), this, SLOT(mapMotifToGrid()));
    connect(sauvegarder, SIGNAL(triggered()), this, SLOT(saveImg()));
    connect(toggle_grille, SIGNAL(triggered()), this, SLOT(toggleGrid()));
    connect(generate_bsurface, SIGNAL(triggered()), this, SLOT(createBSurface()));
    connect(sauvegarder_grille, SIGNAL(triggered()), this, SLOT(saveGrid()));
    connect(charger_grille, SIGNAL(triggered()), this, SLOT(loadSavedGrid()));
    connect(supprimer_grille, SIGNAL(triggered()), this, SLOT(deleteGrid()));
    connect(sauvegarder_masque, SIGNAL(triggered()), this, SLOT(saveTemplate()));
    connect(charger_masque, SIGNAL(triggered()), this, SLOT(loadSavedTemplate()));
    connect(toggle_bsurface, SIGNAL(triggered()), this, SLOT(toggleBSurface()));

    view->show();
}

QGraphicsScene* Window::getScene() const
{
    return scene;
}

QList<Node*>* Window::getSelectionNodes()
{
    return &selectionNodes;
}

QList<Edge*>* Window::getSelectionEdges()
{
    return &selectionEdges;
}

bool Window::getMaskTool() const
{
    return maskTool;
}

QVector<QString> Window::getListeMotifs() const { return liste_motifs; }

QList<QImage*> Window::getListeMasques() const { return liste_masques; }

void Window::creationMasque(QList<Node*> selection)
{
    QVector<QPoint> pList;
    QImage* masque = new QImage(scene->width(), scene->height(), QImage::Format_RGB32);

    // On remplit la liste de points
    for(int i = 0; i < selection.size(); i++)
    {
        pList.push_back(QPoint(selection.at(i)->x(), selection.at(i)->y()));
    }

    // On construit le polygone associé à la sélection
    QPolygon poly(pList);

    // Enfin on créé le masque
    for(int x = 0; x < scene->width(); x++)
    {
        for(int y = 0; y < scene->height(); y++)
        {
            if(poly.containsPoint(QPoint(x, y), Qt::OddEvenFill))
                masque->setPixel(x, y, qRgb(255, 255, 255));
            else
                masque->setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    // On ajoute le masque à la liste des masques
    liste_masques.push_back(masque);

    // Et on met à jour l'affichage
    update_masques_tab();
}

void Window::openImage()
{
    image = QFileDialog::getOpenFileName(this, "ouvrir une image", QString(), "(*.bmp *.png *.jpg *.jpeg)");
    //image_item = new QGraphicsPixmapItem(QPixmap(image));
    image_item = new Pixmap(QPixmap(image), this);
    QRectF image_item_bbox = image_item->boundingRect();
    scene->setSceneRect(image_item_bbox);
    scene->addItem(image_item);
}

void Window::openMotif()
{
    QString motif_path;
    motif_path = QFileDialog::getOpenFileName(this, "ajouter un motif", QString(), "(*.bmp *.png *.jpg *.jpeg)");
    liste_motifs.push_back(motif_path);

    update_motifs_tab();
}

void Window::creationGrille()
{
    // demande à l'utilisateur la précision de la grille
    int precision = QInputDialog::getInt(this, "Dimension de la grille", "Dimension de la grille en x et y");
    g = new Grid(this, scene, precision);
    toggle_grille->setEnabled(true);
    supprimer_grille->setEnabled(true);
    sauvegarder_grille->setEnabled(true);
    generate_bsurface->setEnabled(true);
}

void Window::nouveauMasque()
{
    maskTool = true;
    QMessageBox::information(this, "Outil de sélection", "Cliquez sur l'image afin de définir un point, puis sur une autre zone de l'image pour en définir un autre afin de créer une arête entre vos deux points. Répétez l'opération et appuyer sur la touche entrer pour que votre forme se ferme entre votre dernier et premier point.");
    image_item->setCursor(Qt::CrossCursor);
}

void Window::mapMotifToGrid()
{
    if(image_item == nullptr)
        QMessageBox::warning(this, "Echec map !", "Aucune image disponible !");
    else if(g == nullptr)
        QMessageBox::warning(this, "Echec map !", "Aucune grille disponible !");
    else if(liste_motifs.size() == 0)
        QMessageBox::warning(this, "Echec map !", "Aucun motif disponible !");
    else
    {
        // L'utilisateur choisit le motif qu'il souhaite appliquer sur la grille et éventuellement un masque de sélection
        MappingDialog mapDialog(this);
        mapDialog.exec();

        // On récupère le motif courant
        QImage img_motif(motif_courant);

        // On récupère la grille sur la photo et ses caractéristiques
        Node** grid;
        int grid_dim;
        if(bSurface != nullptr)
        {
            grid = bSurface->getSurfaceNodes();
            grid_dim = bSurface->getDim();
        }
        else
        {
            grid = g->getGrid();
            grid_dim = g->getGridDim();
        }

        int nbZones = (grid_dim - 1) * (grid_dim - 1);

        if(nbRepeat == 1)
            nbRepeat = grid_dim - 1;
        else
            nbRepeat = nbRepeat * (grid_dim - 1);

        // photo sur laquelle on va travailler
        QPixmap img = image_item->pixmap();
        QImage imgData = img.toImage();
        int nbPixWidth = imgData.width();
        int nbPixHeight = imgData.height();
        int zone = 1;

        for(; zone <= nbZones; zone++)
        {
            ChunkCorners zonePhoto;
            ChunkCorners zoneMotif;
            // On récupère les coins top-left, top-right, bot-left, bot-right de la zone
            // ##### photo
            zonePhoto.processChunkCoords(grid_dim, grid, zone);
            // ##### motif
            zoneMotif.processChunkCoordsImg(img_motif, zone, grid_dim);

            // ########## parcours des lignes de la zone ##########
            int nbLines = qMax(qAbs(zonePhoto.yTR() - zonePhoto.yBR()),
                               qMax(qAbs(zonePhoto.yTL() - zonePhoto.yBL()),
                                    qMax(qAbs(zonePhoto.yTL() - zonePhoto.yBR()),
                                         qMax(qAbs(zonePhoto.yTR() - zonePhoto.yBL()),
                                              qMax(qAbs(zonePhoto.yTL() - zonePhoto.yTR()), qAbs(zonePhoto.yBR() - zonePhoto.yBL()))))));
            // identification des marqueurs
            int m0 = 0, m1 = 0, m2 = 0, m3 = 0;
            findMarksIdentity(&m0, &m1, &m2, &m3, &zonePhoto);

            // ##### photo
            int xStartP = 0, yStartP = 0;
            int xArriveP = 0, yArriveP = 0;
            double Dyl = 0, Dyr = 0;
            double Dxl = 0, Dxr = 0;
            int t = 0, t_ = 0;
            // ##### motif
            int xStartM = 0, yStartM = 0;
            int xArriveM = 0, yArriveM = 0;

            bool guardM2 = true;

            for(int l = m0; l < m0 + nbLines; l++)
            {
                // On définit les points de départ et arrivé
                // ##### photo
                if(zonePhoto.xTL() == zonePhoto.xBL() && zonePhoto.yTL() == zonePhoto.yTR() && zonePhoto.yBR() == zonePhoto.yBL() && zonePhoto.xBR() == zonePhoto.xTR())
                {
                    // carré
                    Dyl = qAbs(zonePhoto.yBL() - zonePhoto.yTL());
                    Dyr = qAbs(zonePhoto.yBR() - zonePhoto.yTR());

                    xStartP = zonePhoto.xTL();
                    yStartP = zonePhoto.yTL() + t;
                    xArriveP = zonePhoto.xTR();
                    yArriveP = zonePhoto.yTR() + t;

                    t++;
                }

                else if(zonePhoto.yTL() == zonePhoto.yTR())
                {
                    guardM2 = true;
                    // Il n'y a que m2 à vérifier
                    if(l <= m2)
                    {
                        Dyl = qAbs(zonePhoto.yBL() - zonePhoto.yTL());
                        Dxl = zonePhoto.xBL() - zonePhoto.xTL();
                        Dyr = qAbs(zonePhoto.yBR() - zonePhoto.yTR());
                        Dxr = zonePhoto.xBR() - zonePhoto.xTR();

                        xStartP = zonePhoto.xTL() + ( t * (Dxl / Dyl) );
                        yStartP = zonePhoto.yTL() + t;
                        xArriveP = zonePhoto.xTR() + ( t_ * (Dxr / Dyr) );
                        yArriveP = zonePhoto.yTR() + t_;

                        t++;
                        t_++;
                    }
                    else
                    {
                        if(m2 == zonePhoto.yBL())
                        {
                            if(guardM2)
                            {
                                t = 0;
                                guardM2 = false;
                            }

                            Dyl = qAbs(zonePhoto.yBR() - zonePhoto.yBL());
                            Dxl = zonePhoto.xBR() - zonePhoto.xBL();

                            xStartP = zonePhoto.xBL() + ( t * (Dxl / Dyl) );
                            yStartP = zonePhoto.yBL() + t;
                            xArriveP = zonePhoto.xTR() + ( t_ * (Dxr / Dyr) );
                            yArriveP = zonePhoto.yTR() + t_;

                            t++;
                            t_++;
                        }
                        else if(m2 == zonePhoto.yBR())
                        {
                            if(guardM2)
                            {
                                t_ = 0;
                                guardM2 = false;
                            }

                            Dyr = qAbs(zonePhoto.yBL() - zonePhoto.yBR());
                            Dxr = zonePhoto.xBL() - zonePhoto.xBR();

                            xStartP = zonePhoto.xTL() + ( t * (Dxl / Dyl) );
                            yStartP = zonePhoto.yTL() + t;
                            xArriveP = zonePhoto.xBR() + ( t_ * (Dxr / Dyr) );
                            yArriveP = zonePhoto.yBR() + t_;

                            t++;
                            t_++;
                        }
                    }
                }

                else
                {
                    calculDiffXY(&Dyl, &Dyr, &Dxl, &Dxr, l, m0, m1, m2, zonePhoto, &xStartP, &yStartP, &xArriveP, &yArriveP, &t, &t_);
                }

                // ##### motif
                if(zonePhoto.xTL() == zonePhoto.xBL() && zonePhoto.yTL() == zonePhoto.yTR() && zonePhoto.yBR() == zonePhoto.yBL() && zonePhoto.xBR() == zonePhoto.xTR())
                {
                    // carré
                    xStartM = zoneMotif.xTL();
                    yStartM = zoneMotif.yTL() + ( t * ( (zoneMotif.yBL() - zoneMotif.yTL()) / Dyl) );
                    xArriveM = zoneMotif.xTR();
                    yArriveM = yStartM;
                }
                else if(l <= m1)
                {
                    if(m0 == zonePhoto.yTL())
                    {
                        xStartM = zoneMotif.xTL();
                        yStartM = zoneMotif.yTL() + ( t * ( (zoneMotif.yBL() - zoneMotif.yTL()) / Dyl) );
                        xArriveM = zoneMotif.xTL() + ( t_ * ( (zoneMotif.xTR() - zoneMotif.xTL()) / Dyr) );
                        yArriveM = zoneMotif.yTL();
                    }
                    else if(m0 == zonePhoto.yTR())
                    {
                        xStartM = zoneMotif.xTR() + ( t * ( (zoneMotif.xTL() - zoneMotif.xTR()) / Dyl) );
                        yStartM = zoneMotif.yTR();
                        xArriveM = zoneMotif.xTR();
                        yArriveM = zoneMotif.yTR() + ( t_ * ( (zoneMotif.yBR() - zoneMotif.yTR()) / Dyr) );
                    }
                }
                else if(l <= m2)
                {
                    if(m0 == zonePhoto.yTL())
                    {
                        if(m1 == zonePhoto.yTR())
                        {
                            xStartM = zoneMotif.xTL();
                            yStartM = zoneMotif.yTL() + ( t * ( (zoneMotif.yBL() - zoneMotif.yTL()) / Dyl) );
                            xArriveM = zoneMotif.xTR();
                            yArriveM = zoneMotif.yTR() + ( t_ * ( (zoneMotif.yBR() - zoneMotif.yTR()) / Dyr) );
                        }
                        else if(m1 == zonePhoto.yBL())
                        {
                            xStartM = zoneMotif.xBL() + ( t * ( (zoneMotif.xBR() - zoneMotif.xBL()) / Dyl) );
                            yStartM = zoneMotif.yBL();
                            xArriveM = zoneMotif.yTL() + ( t_ * ( (zoneMotif.xTR() - zoneMotif.xTL()) / Dyr) );
                            yArriveM = zoneMotif.yTL();
                        }
                    }
                    else if(m0 == zonePhoto.yTR())
                    {
                        if(m1 == zonePhoto.yTL())
                        {
                            xStartM = zoneMotif.xTL();
                            yStartM = zoneMotif.yTL() + ( t * ( (zoneMotif.yBL() - zoneMotif.yTL()) / Dyl) );
                            xArriveM = zoneMotif.xTR();
                            yArriveM = zoneMotif.yTR() + ( t_ * ( (zoneMotif.yBR() - zoneMotif.yTR()) / Dyr) );
                        }
                        else if(m1 == zonePhoto.yBR())
                        {
                            xStartM = zoneMotif.xTR() + ( t * ( (zoneMotif.xTL() - zoneMotif.xTR()) / Dyl) );
                            yStartM = zoneMotif.yTR();
                            xArriveM = zoneMotif.xBR() + ( t_ * ( (zoneMotif.xBL() - zoneMotif.xBR()) / Dyr) );
                            yArriveM = zoneMotif.yBR();
                        }
                    }
                }
                else
                {
                    if(m0 == zonePhoto.yTL())
                    {
                        if(m1 == zonePhoto.yTR())
                        {
                            if(m2 == zonePhoto.yBL())
                            {
                                xStartM = zoneMotif.xTL() + ( t * ( (zoneMotif.xBR() - zoneMotif.xBL()) / Dyl) );
                                yStartM = zoneMotif.yBL();
                                xArriveM = zoneMotif.xBR();
                                yArriveM = zoneMotif.yTR() + ( t_ * ( (zoneMotif.yBR() - zoneMotif.yTR()) / Dyr) );
                            }
                            else if(m2 == zonePhoto.yBR())
                            {
                                xStartM = zoneMotif.xBL();
                                yStartM = zoneMotif.yTL() + ( t * ( (zoneMotif.yBL() - zoneMotif.yTL()) / Dyl) );
                                xArriveM = zoneMotif.xBR() + ( t_ * ( (zoneMotif.xBL() - zoneMotif.xBR()) / Dyr) );
                                yArriveM = zoneMotif.yBR();
                            }
                        }
                        else if(m1 == zonePhoto.yBL())
                        {
                            if(m2 == zonePhoto.yTR())
                            {
                                xStartM = zoneMotif.xBL() + ( t * ( (zoneMotif.xBR() - zoneMotif.xBL()) / Dyl) );
                                yStartM = zoneMotif.yBR();
                                xArriveM = zoneMotif.xTR();
                                yArriveM = zoneMotif.yTR() + ( t_ * ( (zoneMotif.yBR() - zoneMotif.yTR()) / Dyr) );
                            }
                            else if(m2 == zonePhoto.yBR())
                            {
                                xStartM = zoneMotif.xBR();
                                yStartM = zoneMotif.yBR() + ( t * ( (zoneMotif.yTR() - zoneMotif.yBR()) / Dyl) );
                                xArriveM = zoneMotif.xTL() + ( t_ * ( (zoneMotif.xTR() - zoneMotif.xTL()) / Dyr) );
                                yArriveM = zoneMotif.yTR();
                            }
                        }
                    }
                    else if(m0 == zonePhoto.yTR())
                    {
                        if(m1 == zonePhoto.yTL())
                        {
                            if(m2 == zonePhoto.yBL())
                            {
                                xStartM = zoneMotif.xBL() + ( t * ( (zoneMotif.xBR() - zoneMotif.xBL()) / Dyl) );
                                yStartM = zoneMotif.yBL();
                                xArriveM = zoneMotif.xBR();
                                yArriveM = zoneMotif.yTR() + ( t_ * ( (zoneMotif.yBR() - zoneMotif.yTR()) / Dyr) );
                            }
                            else if(m2 == zonePhoto.yBR())
                            {
                                xStartM = zoneMotif.xBL();
                                yStartM = zoneMotif.yTL() + ( t * ( (zoneMotif.yBL() - zoneMotif.yTL()) / Dyl) );
                                xArriveM = zoneMotif.xBR() + ( t_ * ( (zoneMotif.xBL() - zoneMotif.xBR()) / Dyr) );
                                yArriveM = zoneMotif.yBL();
                            }
                        }
                        else if(m1 == zonePhoto.yBR())
                        {
                            if(m2 == zonePhoto.yTL())
                            {
                                xStartM = zoneMotif.xBL();
                                yStartM = zoneMotif.yTL() + ( t * ( (zoneMotif.yBL() - zoneMotif.yTL()) / Dyl) );
                                xArriveM = zoneMotif.xBR() + ( t_ * ( (zoneMotif.xBL() - zoneMotif.xBR()) / Dyr) );
                                yArriveM = zoneMotif.yBL();
                            }
                            else if(m2 == zonePhoto.yBL())
                            {
                                xStartM = zoneMotif.xTR() + ( t * ( (zoneMotif.xTL() - zoneMotif.xTR()) / Dyl) );
                                yStartM = zoneMotif.yTL();
                                xArriveM = zoneMotif.xBL();
                                yArriveM = zoneMotif.yBL() + ( t_ * ( (zoneMotif.yTL() - zoneMotif.yBL()) / Dyr) );
                            }
                        }
                    }
                }

                // Valeur du différentiel en X sur la photo
                double diffP = qAbs(static_cast<double>(xArriveP) - static_cast<double>(xStartP));

                // parcours de la ligne
                int Xphoto = 0, Yphoto = 0;
                int Xmotif = 0, Ymotif = 0;
                double nbMotifsPerChunkDim = static_cast<double>(nbRepeat) / static_cast<double>(grid_dim - 1); // nombre de motifs à placer sur une dimension (x ou y) d'un chunk de la grille
                QRgb motifColor;
                QRgb photoColor;
                QRgb color;

                for(int pos = 0; pos < diffP; pos++)
                {
                    Xphoto = xStartP + pos;
                    Yphoto = yStartP;
                    Xmotif = xStartM + ( static_cast<double>(pos) * ( static_cast<double>(xArriveM - xStartM) / diffP ) );
                    Ymotif = yStartM + ( static_cast<double>(pos) * ( static_cast<double>(yArriveM - yStartM) / diffP ) );

                    Xmotif = static_cast<int>(Xmotif * nbMotifsPerChunkDim) % (img_motif.width() - 1);
                    Ymotif = static_cast<int>(Ymotif * nbMotifsPerChunkDim) % (img_motif.height() - 1);

                    motifColor = img_motif.pixel(Xmotif, Ymotif);
                    photoColor = imgData.pixel(Xphoto, Yphoto);
                    color = qRgb( (qRed(motifColor) + qRed(photoColor)) / 2, (qGreen(motifColor) + qGreen(photoColor)) / 2, (qBlue(motifColor) + qBlue(photoColor)) / 2);
                    if(masque_courant != nullptr)
                    {
                        QColor masque_color = masque_courant->pixelColor(Xphoto, Yphoto);
                        if(masque_color == Qt::white)
                            imgData.setPixelColor(Xphoto, Yphoto, color);
                    }
                    else
                        imgData.setPixelColor(Xphoto, Yphoto, color);
                }
            }
        }

        QPixmap res;
        res = res.fromImage(imgData);
        image_item = new QGraphicsPixmapItem(res);
        scene->addItem(image_item);
        masque_courant = nullptr;
    }
}

void Window::saveImg()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(this, "Enregistrer image", QDir::currentPath(), "*.jpeg *.jpg *.png *.bmp *.tiff *.gif");

    QPixmap img = image_item->pixmap();
    QImage imgSave = img.toImage();

    if( ! imgSave.save(filename) )
        QMessageBox::warning(this, "Echec sauvegarde", "Une erreur est survenue, l'image n'a pas pu être sauvegardée ! ¯\_(⊙︿⊙)_/¯ ");
}

void Window::setMotif(QString arg)
{
    motif_courant = arg;
}

void Window::setMasque(QImage* arg)
{
    masque_courant = arg;
}

void Window::setRepeat(int arg)
{
    nbRepeat = arg;
}

void Window::toggleGrid()
{
    if(g != nullptr)
        g->toggleVisible();
}

void Window::createBSurface()
{
    if(bSurface != nullptr)
        delete bSurface;

    int min = g->getGridDim() - 1;
    int precision = QInputDialog::getInt(this, "Précision", "Entrez un multiplicateur de précision pour la surface de bézier à générer", min, min);
    double step = 1.0 / static_cast<double>(precision);

    bSurface = new SurfaceBezier(g, this, step);

    toggle_bsurface->setEnabled(true);
}

void Window::saveGrid()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(this, "Sauvegarder la grille", QDir::currentPath(), "*.gmap");

    // On rajoute l'extension si elle n'est pas présente dans le nom du fichier
    if(! filename.contains(QRegExp("\.gmap")))
    {
        filename.append(".gmap");
    }

    // Création et ouverture du fichier
    QFile sauvegarde(filename);
    sauvegarde.open(QIODevice::WriteOnly);

    // On sérialise
    const char* posX;
    const char* posY;
    int grid_dim = g->getGridDim();
    int nbNodes = grid_dim * grid_dim;
    Node** noeuds = g->getGrid();

    const char* gDim = QString::number(grid_dim).toStdString().c_str();
    sauvegarde.write(gDim);
    sauvegarde.putChar('\n');

    for(int n = 0; n < nbNodes; n++)
    {
        int x = noeuds[n]->pos().x();
        int y = noeuds[n]->pos().y();

        posX = QString::number(x).toStdString().c_str();
        posY = QString::number(y).toStdString().c_str();
        sauvegarde.write(posX);
        sauvegarde.putChar('.');
        sauvegarde.write(posY);
        sauvegarde.putChar('\n');
    }

    sauvegarde.close();
}

void Window::loadSavedGrid()
{
    QString filename("");
    filename = QFileDialog::getOpenFileName(this, "Charger une grille 2D", QDir::currentPath(), "*.gmap");

    if(filename.compare("") != 0)
    {
        QFile sauvegarde(filename);
        if(sauvegarde.open(QIODevice::ReadOnly))
        {
            if(g != nullptr)
            {
                delete g;
                g = nullptr;
            }
            QTextStream in(&sauvegarde);
            QString line;
            int posX = 0;
            int posY = 0;
            int n = 0;
            QStringList pos;

            line = in.readLine();
            g = new Grid(scene, line.toInt());
            Node** nodes = g->getGrid();

            while( ! in.atEnd() )
            {
                line = in.readLine();
                pos = line.split(".");
                posX = pos.at(0).toInt();
                posY = pos.at(1).toInt();
                nodes[n] = new Node(scene);
                nodes[n]->setPos(posX, posY);
                n++;
            }

            // On peut maintenant dessiner la grille sur la scene
            int grid_dim = g->getGridDim();
            int nbNodes = grid_dim * grid_dim;
            int l = 1;

            for(int n = 0; n < nbNodes; n++)
            {
                scene->addItem(nodes[n]);
            }

            for(int i = 0; i < nbNodes; i++)
            {
                if((l % grid_dim) == 0 && l != 0)
                {
                    // y a t'il une ligne en dessous ?
                    if((i + 1) < nbNodes)
                    {
                        Edge* edgeV = new Edge(scene, nodes[i], nodes[i + grid_dim]);
                        scene->addItem(edgeV);
                    }
                    // sinon on ne fait rien
                    l++;
                }

                else if(((i + 1) % grid_dim) != 0)
                {
                    Edge* edgeH = new Edge(scene, nodes[i], nodes[i+1]);
                    scene->addItem(edgeH);

                    if((i + grid_dim) < nbNodes)
                    {
                        Edge* edgeV = new Edge(scene, nodes[i], nodes[i + grid_dim]);
                        scene->addItem(edgeV);
                    }

                    l++;
                }
            }

            sauvegarde.close();

            toggle_grille->setEnabled(true);
            supprimer_grille->setEnabled(true);
            sauvegarder_grille->setEnabled(true);
            generate_bsurface->setEnabled(true);
        }

        else
        {
            QMessageBox::warning(this, "Echec chargement de la grille", "Le fichier de sauvegarde n'a pas pu être ouvert correctement, veuillez réessayer.");
            return;
        }
    }

    else
    {
        return;
    }
}

void Window::deleteGrid()
{
    if(g != nullptr)
    {
        delete g;
        g = nullptr;

        toggle_grille->setEnabled(false);
        supprimer_grille->setEnabled(false);
        sauvegarder_grille->setEnabled(false);
        generate_bsurface->setEnabled(false);
    }
}

void Window::saveTemplate()
{
    int nbMasques = liste_masques.size();
    if(nbMasques > 0)
    {
        SaveTemplateDialog dialog(this);
        dialog.exec();

        QImage* masque = liste_masques.at(templateToSave);
        QString masque_name = QFileDialog::getSaveFileName(this, "Sauvegarder le masque", QDir::currentPath(), "*.jpeg *.jpg *.png *.bmp *.tiff");

        if( ! masque->save(masque_name) )
            QMessageBox::warning(this, "Echec sauvegarde", "Une erreur est survenue, le masque n'a pas pu être sauvegardé ! ¯\_(⊙︿⊙)_/¯ ");
    }
}

void Window::setTemplateIdToSave(int id)
{
    templateToSave = id;
}

void Window::loadSavedTemplate()
{
    QString masque_path;
    masque_path = QFileDialog::getOpenFileName(this, "charger un masque", QString(), "(*.bmp *.png *.jpg *.jpeg *.tiff)");
    QImage* masque = new QImage(masque_path);
    liste_masques.push_back(masque);

    update_masques_tab();
}

void Window::toggleBSurface()
{
    if(bSurface != nullptr)
        bSurface->toggleVisible();
}

void Window::keyPressEvent(QKeyEvent *event)
{
    if(maskTool)
    {
        if(event->key() == Qt::Key_Return)
        {
            if(selectionNodes.size() > 2)
            {
                int nbElems = selectionNodes.size();
                Edge* closingEdge = new Edge(scene, selectionNodes.at(nbElems - 1), selectionNodes.at(0));
                selectionEdges.push_back(closingEdge);
                scene->addItem(closingEdge);
                creationMasque(selectionNodes);
            }
            else
                QMessageBox::warning(this, "Echec lors de la création d'un masque", "Vous n'avez pas renseigné un nombre suffisant de points afin de former une sélection valide pour la création d'un masque !");

            // destruction des items au-dessus de l'image
            int nbNodes = selectionNodes.size();
            int nbEdges = selectionEdges.size();

            for(int j = 0; j < nbEdges; j++)
            {
                scene->removeItem(selectionEdges.at(j));
            }
            selectionEdges.clear();

            for(int i = 0; i < nbNodes; i++)
            {
                scene->removeItem(selectionNodes.at(i));
            }
            selectionNodes.clear();

            maskTool = false;

            sauvegarder_masque->setEnabled(true);

            image_item->setCursor(Qt::ArrowCursor);
        }
    }
}

void Window::update_motifs_tab()
{
    int nb_motifs = liste_motifs.size();
    int i, count = 0;

    for(i = 0; i < nb_motifs; i++)
    {
        QImage pattern(liste_motifs.at(i));
        QImage pattern2;
        if(pattern.width() > pattern.height())
            pattern2 = pattern.scaledToWidth(pattern.width(), Qt::SmoothTransformation);
        else if(pattern.height() > pattern.width())
            pattern2 = pattern.scaledToHeight(pattern.height(), Qt::SmoothTransformation);
        else
            pattern2 = pattern.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QLabel* pattern_label = new QLabel("");
        pattern_label->setScaledContents(true);
        pattern_label->setFixedSize(100, 100);
        pattern_label->setPixmap(QPixmap::fromImage(pattern2));
        pattern_label->setStyleSheet("border: 2px solid black; border-radius: 4px;");
        if(i % 2 == 0)
        {
            layout_motifs->addWidget(pattern_label, i - count, 0);
            count++;
        }
        else
        {
            layout_motifs->addWidget(pattern_label, i - count, 1);
        }
    }
}

void Window::update_masques_tab()
{
    int nb_masques = liste_masques.size();
    int i, count = 0;

    for(i = 0; i < nb_masques; i++)
    {
        QImage* masque = liste_masques.at(i);
        QImage thumbnail;
        if(masque->width() > masque->height())
            thumbnail = masque->scaledToWidth(masque->width(), Qt::SmoothTransformation);
        else if(masque->height() > masque->width())
            thumbnail = masque->scaledToHeight(masque->height(), Qt::SmoothTransformation);
        else
            thumbnail = masque->scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QLabel* masque_label = new QLabel("");
        masque_label->setScaledContents(true);
        masque_label->setFixedSize(100, 100);
        masque_label->setPixmap(QPixmap::fromImage(thumbnail));
        masque_label->setStyleSheet("border: 2px solid black; border-radius: 4px;");
        if(i % 2 == 0)
        {
            layout_masques->addWidget(masque_label, i - count, 0);
            count++;
        }
        else
        {
            layout_masques->addWidget(masque_label, i - count, 1);
        }
    }
}

void Window::findMarksIdentity(int* m0, int* m1, int* m2, int* m3, ChunkCorners* zonePhoto)
{
    *m0 = qMin(zonePhoto->yTL(), qMin(zonePhoto->yTR(), qMin(zonePhoto->yBL(), zonePhoto->yBR())));
    if(*m0 == zonePhoto->yTL())
    {
        *m1 = qMin(zonePhoto->yTR(), qMin(zonePhoto->yBL(), zonePhoto->yBR()));
        if(*m1 == zonePhoto->yTR())
        {
            *m2 = qMin(zonePhoto->yBL(), zonePhoto->yBR());
            if(*m2 == zonePhoto->yBL())
            {
                *m3 = zonePhoto->yBR();
            }
            else
            {
                *m3 = zonePhoto->yBL();
            }
        }
        else if(*m1 == zonePhoto->yBL())
        {
            *m2 = qMin(zonePhoto->yTR(), zonePhoto->yBR());
            if(*m2 == zonePhoto->yTR())
            {
                *m3 = zonePhoto->yBR();
            }
            else
            {
                *m3 = zonePhoto->yTR();
            }
        }
        else
        {
            *m2 = qMin(zonePhoto->yTR(), zonePhoto->yBL());
            if(*m2 == zonePhoto->yTR())
            {
                *m3 = zonePhoto->yBL();
            }
            else
            {
                *m3 = zonePhoto->yTR();
            }
        }
    }
    else if(*m0 == zonePhoto->yTR())
    {
        *m1 = qMin(zonePhoto->yTL(), qMin(zonePhoto->yBL(), zonePhoto->yBR()));
        if(*m1 == zonePhoto->yTL())
        {
            *m2 = qMin(zonePhoto->yBL(), zonePhoto->yBR());
            if(*m2 == zonePhoto->yBL())
            {
                *m3 = zonePhoto->yBR();
            }
            else
            {
                *m3 = zonePhoto->yBL();
            }
        }
        else if(*m1 == zonePhoto->yBL())
        {
            *m2 = qMin(zonePhoto->yTL(), zonePhoto->yBR());
            if(*m2 == zonePhoto->yTL())
            {
                *m3 = zonePhoto->yBR();
            }
            else
            {
                *m3 = zonePhoto->yTL();
            }
        }
        else
        {
            *m2 = qMin(zonePhoto->yTL(), zonePhoto->yBL());
            if(*m2 == zonePhoto->yTL())
            {
                *m3 = zonePhoto->yBL();
            }
            else
            {
                *m3 = zonePhoto->yTL();
            }
        }
    }
    else if(*m0 == zonePhoto->yBL())
    {
        *m1 = qMin(zonePhoto->yTL(), qMin(zonePhoto->yTR(), zonePhoto->yBR()));
        if(*m1 == zonePhoto->yTL())
        {
            *m2 = qMin(zonePhoto->yTR(), zonePhoto->yBR());
            if(*m2 == zonePhoto->yTR())
            {
                *m3 = zonePhoto->yBR();
            }
            else
            {
                *m3 = zonePhoto->yTR();
            }
        }
        else if(*m1 == zonePhoto->yTR())
        {
            *m2 = qMin(zonePhoto->yTL(), zonePhoto->yBR());
            if(*m2 == zonePhoto->yTL())
            {
                *m3 = zonePhoto->yBR();
            }
            else
            {
                *m3 = zonePhoto->yTL();
            }
        }
        else
        {
            *m2 = qMin(zonePhoto->yTL(), zonePhoto->yTR());
            if(*m2 == zonePhoto->yTL())
            {
                *m3 = zonePhoto->yTR();
            }
            else
            {
                *m3 = zonePhoto->yTL();
            }
        }
    }
    else
    {
        *m1 = qMin(zonePhoto->yTL(), qMin(zonePhoto->yTR(), zonePhoto->yBL()));
        if(*m1 == zonePhoto->yTL())
        {
            *m2 = qMin(zonePhoto->yTR(), zonePhoto->yBL());
            if(*m2 == zonePhoto->yTR())
            {
                *m3 = zonePhoto->yBL();
            }
            else
            {
                *m3 = zonePhoto->yTR();
            }
        }
        else if(*m1 == zonePhoto->yTR())
        {
            *m2 = qMin(zonePhoto->yTL(), zonePhoto->yBL());
            if(*m2 == zonePhoto->yTL())
            {
                *m3 = zonePhoto->yBL();
            }
            else
            {
                *m3 = zonePhoto->yTL();
            }
        }
        else
        {
            *m2 = qMin(zonePhoto->yTL(), zonePhoto->yTR());
            if(*m2 == zonePhoto->yTL())
            {
                *m3 = zonePhoto->yTR();
            }
            else
            {
                *m3 = zonePhoto->yTL();
            }
        }
    }
}

void Window::calculDiffXY(double* Dyl, double* Dyr, double* Dxl, double* Dxr, int l, int m0, int m1, int m2, ChunkCorners zonePhoto, int* xStartP, int* yStartP, int* xArriveP, int* yArriveP, int* t, int* t_)
{
    static bool guardM2 = true;
    static bool guardM3 = true;

    // les 3 possibles zones de correspondance entre deux arêtes
    if(l <= m1)
    {
        guardM2 = true;
        guardM3 = true;
        // on détermine l'origine
        if(m0 == zonePhoto.yTL())
        {
            *Dyl = qAbs(zonePhoto.yBL() - zonePhoto.yTL());
            *Dyr = qAbs(zonePhoto.yTR() - zonePhoto.yTL());
            *Dxl = zonePhoto.xBL() - zonePhoto.xTL();
            *Dxr = zonePhoto.xTR() - zonePhoto.xTL();

            *xStartP = zonePhoto.xTL() + ( *t * (*Dxl/(*Dyl)) );
            *yStartP = zonePhoto.yTL() + *t;
            *xArriveP = zonePhoto.xTL() + ( *t_ * (*Dxr/(*Dyr)) );
            *yArriveP = zonePhoto.yTL() + *t_;

            *t = *t + 1;
            *t_ = *t_ + 1;
        }
        else if(m0 == zonePhoto.yTR())
        {
            *Dyl = qAbs(zonePhoto.yTL() - zonePhoto.yTR());
            *Dyr = qAbs(zonePhoto.yBR() - zonePhoto.yTR());
            *Dxl = zonePhoto.xTL() - zonePhoto.xTR();
            *Dxr = zonePhoto.xBR() - zonePhoto.xTR();

            *xStartP = zonePhoto.xTR() + ( *t * (*Dxl/(*Dyl)) );
            *yStartP = zonePhoto.yTR() + *t;
            *xArriveP = zonePhoto.xTR() + ( *t_ * (*Dxr/(*Dyr)) );
            *yArriveP = zonePhoto.yTR() + *t_;

            *t = *t + 1;
            *t_ = *t_+ 1;
        }
    }
    else if(l <= m2)
    {
        // on détermine l'origine
        if(m0 == zonePhoto.yTL())
        {
            if(m1 == zonePhoto.yTR())
            {
                if(guardM2)
                {
                    *t_ = 0;
                    guardM2 = false;
                }

                *Dyr = qAbs(zonePhoto.yBR() - zonePhoto.yTR());
                *Dxr = zonePhoto.xBR() - zonePhoto.xTR();

                *xStartP = zonePhoto.xTL() + ( *t * (*Dxl/(*Dyl)) );
                *yStartP = zonePhoto.yTL() + *t;
                *xArriveP = zonePhoto.xTR() + ( *t_ * (*Dxr/(*Dyr)) );
                *yArriveP = zonePhoto.yTR() + *t_;

                *t = *t + 1;
                *t_ = *t_ + 1;
            }
            else if(m1 == zonePhoto.yBL())
            {
                if(guardM2)
                {
                    *t = 0;
                    guardM2 = false;
                }

                *Dyl = qAbs(zonePhoto.yBR() - zonePhoto.yBL());
                *Dxl = zonePhoto.xBR() - zonePhoto.xBL();

                *xStartP = zonePhoto.xBL() + ( *t * (*Dxl/(*Dyl)) );
                *yStartP = zonePhoto.yBL() + *t;
                *xArriveP = zonePhoto.xTL() + ( *t_ * (*Dxr/(*Dyr)) );
                *yArriveP = zonePhoto.yTL() + *t_;

                *t = *t + 1;
                *t_ = *t_ + 1;
            }
        }
        else if(m0 == zonePhoto.yTR())
        {
            if(m1 == zonePhoto.yTL())
            {
                if(guardM2)
                {
                    *t = 0;
                    guardM2 = false;
                }

                *Dyl = qAbs(zonePhoto.yBL() - zonePhoto.yTL());
                *Dxl = zonePhoto.xBL() - zonePhoto.xTL();

                *xStartP = zonePhoto.xTL() + ( *t * (*Dxl/(*Dyl)) );
                *yStartP = zonePhoto.yTL() + *t;
                *xArriveP = zonePhoto.xTR() + ( *t_ * (*Dxr/(*Dyr)) );
                *yArriveP = zonePhoto.yTR() + *t_;

                *t = *t + 1;
                *t_ = *t_ + 1;
            }
            else if(m1 == zonePhoto.yBR())
            {
                if(guardM2)
                {
                    *t_ = 0;
                    guardM2 = false;
                }

                *Dyr = qAbs(zonePhoto.yBL() - zonePhoto.yBR());
                *Dxr = zonePhoto.xBL() - zonePhoto.xBR();

                *xStartP = zonePhoto.xTR() + ( *t * (*Dxl/(*Dyl)) );
                *yStartP = zonePhoto.yTR() + *t;
                *xArriveP = zonePhoto.xBR() + ( *t_ * (*Dxr/(*Dyr)) );
                *yArriveP = zonePhoto.yBR() + *t_;

                *t = *t + 1;
                *t_ = *t_ + 1;
            }
        }
    }
    else
    {
        // on détermine l'origine
        if(m0 == zonePhoto.yTL())
        {
            if(m1 == zonePhoto.yTR())
            {
                if(m2 == zonePhoto.yBL())
                {
                    if(guardM3)
                    {
                        *t = 0;
                        guardM3 = false;
                    }

                    *Dyl = qAbs(zonePhoto.yBR() - zonePhoto.yBL());
                    *Dxl = zonePhoto.xBR() - zonePhoto.xBL();

                    *xStartP = zonePhoto.xBL() + ( *t * (*Dxl/(*Dyl)) );
                    *yStartP = zonePhoto.yBL() + *t;
                    *xArriveP = zonePhoto.xTR() + ( *t_ * (*Dxr/(*Dyr)) );
                    *yArriveP = zonePhoto.yTR() + *t_;

                    *t = *t + 1;
                    *t_ = *t_ + 1;
                }
                else if(m2 == zonePhoto.yBR())
                {
                    if(guardM3)
                    {
                        *t_ = 0;
                        guardM3 = false;
                    }

                    *Dyr = qAbs(zonePhoto.yBL() - zonePhoto.yBR());
                    *Dxr = zonePhoto.xBL() - zonePhoto.xBR();

                    *xStartP = zonePhoto.xTL() + ( *t * (*Dxl/(*Dyl)) );
                    *yStartP = zonePhoto.yTL() + *t;
                    *xArriveP = zonePhoto.xBR() + ( *t_ * (*Dxr/(*Dyr)) );
                    *yArriveP = zonePhoto.yBR() + *t_;

                    *t = *t + 1;
                    *t_ = *t_ + 1;
                }
            }
            else if(m1 == zonePhoto.yBL())
            {
                if(m2 == zonePhoto.yTR())
                {
                    if(guardM3)
                    {
                        *t_ = 0;
                        guardM3 = false;
                    }

                    *Dyr = qAbs(zonePhoto.yBR() - zonePhoto.yTR());
                    *Dxr = zonePhoto.xBR() - zonePhoto.xTR();

                    *xStartP = zonePhoto.xBL() + ( *t * (*Dxl/(*Dyl)) );
                    *yStartP = zonePhoto.yBL() + *t;
                    *xArriveP = zonePhoto.xTR() + ( *t_ * (*Dxr/(*Dyr)) );
                    *yArriveP = zonePhoto.yTR() + *t_;

                    *t = *t + 1;
                    *t_ = *t_ + 1;
                }
                else if(m2 == zonePhoto.yBR())
                {
                    if(guardM3)
                    {
                        *t = 0;
                        guardM3 = false;
                    }

                    *Dyl = qAbs(zonePhoto.yBR() - zonePhoto.yTR());
                    *Dxl = zonePhoto.xBR() - zonePhoto.xTR();

                    *xStartP = zonePhoto.xBR() + ( *t * (*Dxl/(*Dyl)) );
                    *yStartP = zonePhoto.yBR() + *t;
                    *xArriveP = zonePhoto.xTL() + ( *t_ * (*Dxr/(*Dyr)) );
                    *yArriveP = zonePhoto.yTL() + *t_;

                    *t = *t + 1;
                    *t_ = *t_ + 1;
                }
            }
        }
        else if(m0 == zonePhoto.yTR())
        {
            if(m1 == zonePhoto.yTL())
            {
                if(m2 == zonePhoto.yBL())
                {
                    if(guardM3)
                    {
                        *t = 0;
                        guardM3 = false;
                    }

                    *Dyl = qAbs(zonePhoto.yBR() - zonePhoto.yBL());
                    *Dxl = zonePhoto.xBR() - zonePhoto.xBL();

                    *xStartP = zonePhoto.xBL() + ( *t * (*Dxl/(*Dyl)) );
                    *yStartP = zonePhoto.yBL() + *t;
                    *xArriveP = zonePhoto.xTR() + ( *t_ * (*Dxr/(*Dyr)) );
                    *yArriveP = zonePhoto.yTR() + *t_;

                    *t = *t + 1;
                    *t_ = *t_ + 1;
                }
                else if(m2 == zonePhoto.yBR())
                {
                    if(guardM3)
                    {
                        *t_ = 0;
                        guardM3 = false;
                    }

                    *Dyr = qAbs(zonePhoto.yBL() - zonePhoto.yBR());
                    *Dxr = zonePhoto.xBL() - zonePhoto.xBR();

                    *xStartP = zonePhoto.xTL() + ( *t * (*Dxl/(*Dyl)) );
                    *yStartP = zonePhoto.yTL() + *t;
                    *xArriveP = zonePhoto.xBR() + ( *t_ * (*Dxr/(*Dyr)) );
                    *yArriveP = zonePhoto.yBR() + *t_;

                    *t = *t + 1;
                    *t_ = *t_ + 1;
                }
            }
            else if(m1 == zonePhoto.yBR())
            {
                if(m2 == zonePhoto.yTL())
                {
                    if(guardM3)
                    {
                        *t = 0;
                        guardM3 = false;
                    }

                    *Dyl = qAbs(zonePhoto.yBL() - zonePhoto.yTL());
                    *Dxl = zonePhoto.xBL() - zonePhoto.xTL();

                    *xStartP = zonePhoto.xTL() + ( *t * (*Dxl/(*Dyl)) );
                    *yStartP = zonePhoto.yTL() + *t;
                    *xArriveP = zonePhoto.xBR() + ( *t_ * (*Dxr/(*Dyr)) );
                    *yArriveP = zonePhoto.yBR() + *t_;

                    *t = *t + 1;
                    *t_ = *t_ + 1;
                }
                else if(m2 == zonePhoto.yBL())
                {
                    if(guardM3)
                    {
                        *t_ = 0;
                        guardM3 = false;
                    }

                    *Dyr = qAbs(zonePhoto.yTL() - zonePhoto.yBL());
                    *Dxr = zonePhoto.xTL() - zonePhoto.xBL();

                    *xStartP = zonePhoto.xTR() + ( *t * (*Dxl/(*Dyl)) );
                    *yStartP = zonePhoto.yTR() + *t;
                    *xArriveP = zonePhoto.xBL() + ( *t_ * (*Dxr/(*Dyr)) );
                    *yArriveP = zonePhoto.yBL() + *t_;

                    *t = *t + 1;
                    *t_ = *t_ + 1;
                }
            }
        }
    }
}
