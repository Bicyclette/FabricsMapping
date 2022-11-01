#ifndef MAPPINGDIALOG_HPP
#define MAPPINGDIALOG_HPP

#include <QtWidgets>
#include "radiobuttonmotif.hpp"

class Window;

class MappingDialog : public QDialog
{
public:
    MappingDialog(Window* win);

private:
    Window* mapping;
    QVBoxLayout* main_layout;
    QVBoxLayout* choix_motif_layout;
    QVBoxLayout* choix_masque_layout;
    QHBoxLayout* choix_repeat_layout;
    QPushButton* bouton_valider;
    QLabel* txt_repeat;
    QSlider* slider_repeat;
    QLCDNumber* aff_repeat;
    QGroupBox* motif_group;
    QVBoxLayout* top_lvl_vbox_motif;
    QGroupBox* masque_group;
    QVBoxLayout* top_lvl_vbox_masque;
};

#endif // MAPPINGDIALOG_HPP
