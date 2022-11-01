#include "mappingdialog.hpp"
#include "window.hpp"

MappingDialog::MappingDialog(Window* win)
{
    setWindowTitle("Map");
    mapping = win;

    txt_repeat = new QLabel();
    txt_repeat->setText("Répétition du motif:");

    bouton_valider = new QPushButton("valider");

    main_layout = new QVBoxLayout();
    choix_masque_layout = new QVBoxLayout();
    choix_motif_layout = new QVBoxLayout();
    choix_repeat_layout = new QHBoxLayout();
    top_lvl_vbox_masque = new QVBoxLayout();
    top_lvl_vbox_motif = new QVBoxLayout();

    motif_group = new QGroupBox(tr("Choix du motif"));
    masque_group = new QGroupBox(tr("Choix du masque de sélection"));

    slider_repeat = new QSlider(Qt::Horizontal);
    slider_repeat->setMaximum(100);
    slider_repeat->setMinimum(1);
    slider_repeat->resize(200, 10);
    aff_repeat = new QLCDNumber();
    aff_repeat->setSegmentStyle(QLCDNumber::Flat);
    connect(slider_repeat, SIGNAL(valueChanged(int)), aff_repeat, SLOT(display(int)));
    connect(slider_repeat, SIGNAL(valueChanged(int)), mapping, SLOT(setRepeat(int)));

    // ajout des radio buttons pour les motifs
    QVector<QString> liste_motifs = mapping->getListeMotifs();
    int nbMotifs = liste_motifs.size();

    for(int i = 0; i < nbMotifs; i++)
    {
        RadioButtonMotif* choix = new RadioButtonMotif(liste_motifs.at(i));
        choix->setText(liste_motifs.at(i));
        connect(choix, SIGNAL(toggled(bool)), choix, SLOT(trigger_selected_signal(bool)));
        connect(choix, SIGNAL(selected(QString)), mapping, SLOT(setMotif(QString)));
        choix_motif_layout->addWidget(choix);
    }
    motif_group->setLayout(choix_motif_layout);
    top_lvl_vbox_motif->addWidget(motif_group);

    // ajout des radio buttons pour les masques
    QList<QImage*> liste_masques = mapping->getListeMasques();
    int nbMasques = liste_masques.size();

    for(int i = 0; i < nbMasques; i++)
    {
        RadioButtonMotif* choix = new RadioButtonMotif(liste_masques.at(i));
        choix->setText("masque " + QString::number(i+1));
        connect(choix, SIGNAL(toggled(bool)), choix, SLOT(trigger_selected_img(bool)));
        connect(choix, SIGNAL(selected_img(QImage*)), mapping, SLOT(setMasque(QImage*)));
        choix_masque_layout->addWidget(choix);
    }
    masque_group->setLayout(choix_masque_layout);
    top_lvl_vbox_masque->addWidget(masque_group);

    // on regroupe
    choix_repeat_layout->addWidget(txt_repeat);

    choix_repeat_layout->addWidget(txt_repeat);
    choix_repeat_layout->addWidget(slider_repeat);
    choix_repeat_layout->addWidget(aff_repeat);

    main_layout->addLayout(top_lvl_vbox_motif);
    main_layout->addLayout(choix_repeat_layout);
    main_layout->addLayout(top_lvl_vbox_masque);
    main_layout->addWidget(bouton_valider);
    setLayout(main_layout);

    connect(bouton_valider, SIGNAL(clicked()), this, SLOT(close()));
}
