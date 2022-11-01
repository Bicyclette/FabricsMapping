#include "radiobuttonmotif.hpp"

RadioButtonMotif::RadioButtonMotif(QImage* img) { image = img; }

RadioButtonMotif::RadioButtonMotif(QString txt) : motif_name(txt) {image = nullptr;}

RadioButtonMotif::RadioButtonMotif(int num_masque) : masqueId(num_masque)
{
    image = nullptr;
}

void RadioButtonMotif::trigger_selected_signal(bool checked)
{
    if(checked)
        emit selected(motif_name);
}

void RadioButtonMotif::trigger_selected_img(bool checked)
{
    if(checked)
        emit selected_img(image);
}

void RadioButtonMotif::trigger_selected_delete(bool checked)
{
    if(checked)
        emit kaboum(motif_name);
}

void RadioButtonMotif::trigger_selected_masque(bool checked)
{
    if(checked)
        emit selected_masque(masqueId);
}
