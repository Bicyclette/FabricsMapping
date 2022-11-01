#include "choixmotifdialog.hpp"
#include "window.hpp"

ChoixMotifDialog::ChoixMotifDialog(Window* win)
{
    setWindowTitle("Choix du motif");
    mapping = win;
    mainLayout = new QVBoxLayout();
    layoutChoix = new QVBoxLayout();
    txt = new QLabel();
    txt->setText("Choisissez le motif à appliquer sur la grille: ");
    valider = new QPushButton("valider");

    QVector<QString> liste = mapping->getListeMotifs();
    int nbMotifs = liste.size();

    for(int i = 0; i < nbMotifs; i++)
    {
        RadioButtonMotif* choix = new RadioButtonMotif(liste.at(i));
        choix->setText(liste.at(i));
        connect(choix, SIGNAL(toggled(bool)), choix, SLOT(trigger_selected_signal(bool)));
        connect(choix, SIGNAL(selected(QString)), mapping, SLOT(setMotif(QString)));
        layoutChoix->addWidget(choix);
    }

    mainLayout->addWidget(txt);
    mainLayout->addLayout(layoutChoix);
    mainLayout->addWidget(valider);

    setLayout(mainLayout);
    connect(valider, SIGNAL(clicked()), this, SLOT(close()));
}
