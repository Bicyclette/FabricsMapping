#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QtWidgets>
#include "grid.hpp"
#include "surfacebezier.hpp"

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window();
    QGraphicsScene* getScene() const;
    QList<Node*>* getSelectionNodes();
    QList<Edge*>* getSelectionEdges();
    bool getMaskTool() const;
    QVector<QString> getListeMotifs() const;
    QList<QImage*> getListeMasques() const;
    void creationMasque(QList<Node*> selection);

public slots:
    void openImage();
    void openMotif();
    void creationGrille();
    void nouveauMasque();
    void mapMotifToGrid();
    void saveImg();
    void setMotif(QString arg);
    void setMasque(QImage* arg);
    void setRepeat(int arg);
    void toggleGrid();
    void createBSurface();
    void saveGrid();
    void loadSavedGrid();
    void deleteGrid();
    void saveTemplate();
    void setTemplateIdToSave(int id);
    void loadSavedTemplate();
    void toggleBSurface();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    /* ####################
     * ####################
     * "###################
     */

    // ########## MENUS DE LA FENÊTRE PRINCIPALE ##########
    QMenu* menu_fichier;
    QMenu* menu_selection;
    QMenu* menu_grille;
    QMenu* menu_motif;
    QMenu* menu_vue;

    // ########## ACTIONS DU MENU FICHIER ##########
    QAction* ouvrir_img;
    QAction* sauvegarder;
    QAction* quitter;

    // ########## ACTIONS DU MENU SÉLECTION ##########
    QAction* nouveau_masque;
    QAction* charger_masque;
    QAction* sauvegarder_masque;

    // ########## ACTIONS DU MENU GRILLE ##########
    QAction* nouvelle_grille;
    QAction* supprimer_grille;
    QAction* charger_grille;
    QAction* toggle_grille;
    QAction* sauvegarder_grille;
    QAction* generate_bsurface;
    QAction* toggle_bsurface;
    QAction* map;

    // ########## ACTIONS DU MENU MOTIF ##########
    QAction* ajouter_motif;

    /* ####################
     * ####################
     * "###################
     */

    // ########## ZONE CENTRALE ##########
    QGraphicsScene* scene;
    QGraphicsView* view;

    // ########## DOCKS ##########
    QDockWidget* motifs_tab;
    QDockWidget* masques_tab;
    QGridLayout* layout_motifs;
    QGridLayout* layout_masques;
    QWidget* motifs_widget;
    QWidget* masques_widget;
    QScrollArea* scroll_motifs;
    QScrollArea* scroll_masques;

    // ########## VARIABLES POUR LE FONCTIONNEMENT DE L'APPLICATION ##########
    QString image;
    QVector<QString> liste_motifs;
    QString motif_courant;
    int nbRepeat;
    QGraphicsPixmapItem* image_item;
    Grid* g;
    bool maskTool;
    QList<Node*> selectionNodes;
    QList<Edge*> selectionEdges;
    QList<QImage*> liste_masques;
    QImage* masque_courant;
    int templateToSave;
    SurfaceBezier* bSurface;

    /* ####################
     * ####################
     * "###################
     */

    void update_motifs_tab();
    void update_masques_tab();
    void findMarksIdentity(int* m0, int* m1, int* m2, int* m3, ChunkCorners* zonePhoto);
    void calculDiffXY(double* Dyl, double* Dyr, double* Dxl, double* Dxr, int l, int m0, int m1, int m2, ChunkCorners zonePhoto, int* xStartP, int* yStartP, int* xArriveP, int* yArriveP, int* t, int* t_);
};

#endif // WINDOW_HPP
