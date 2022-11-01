#include "savetemplatedialog.hpp"
#include "radiobuttonmotif.hpp"
#include "window.hpp"

SaveTemplateDialog::SaveTemplateDialog(Window* win)
{
    mapping = win;
    mainLayout = new QVBoxLayout();
    templateLayout = new QGridLayout();
    valider = new QPushButton("valider");
    title = new QLabel("Choisissez le masque à sauvegarder:");
    liste_masques = mapping->getListeMasques();

    setWindowTitle("Sauvegarde d'un masque");

    int nbMasques = liste_masques.size();
    int count = 0;

    for(int m = 0; m < nbMasques; m++)
    {
        QImage masque(*(liste_masques.at(m)));
        QImage masque2;
        if(masque.width() > masque.height())
            masque2 = masque.scaledToWidth(masque.width(), Qt::SmoothTransformation);
        else if(masque.height() > masque.width())
            masque2 = masque.scaledToHeight(masque.height(), Qt::SmoothTransformation);
        else
            masque2 = masque.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QLabel* masque_label = new QLabel("");
        masque_label->setScaledContents(true);
        masque_label->setFixedSize(100, 100);
        masque_label->setPixmap(QPixmap::fromImage(masque2));
        masque_label->setStyleSheet("border: 2px solid black; border-radius: 4px;");

        RadioButtonMotif* radioButton = new RadioButtonMotif(m);
        connect(radioButton, SIGNAL(selected_masque(int)), mapping, SLOT(setTemplateIdToSave(int)));

        QVBoxLayout* layout = new QVBoxLayout();
        layout->setAlignment(Qt::AlignHCenter);
        layout->addWidget(masque_label);
        layout->addWidget(radioButton);

        if(m % 4 == 0)
        {
            count++;
            templateLayout->addLayout(layout, count, m % 4);
        }
        else
        {
            templateLayout->addLayout(layout, count, m % 4);
        }
    }

    mainLayout->addWidget(title);
    mainLayout->addLayout(templateLayout);
    mainLayout->addWidget(valider);
    setLayout(mainLayout);

    connect(valider, SIGNAL(clicked()), this, SLOT(close()));
}
