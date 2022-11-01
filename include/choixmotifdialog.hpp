#ifndef CHOIXMOTIFDIALOG_HPP
#define CHOIXMOTIFDIALOG_HPP

#include <QtWidgets>
#include "radiobuttonmotif.hpp"

class Window;

class ChoixMotifDialog : public QDialog
{
public:
    ChoixMotifDialog(Window* win);

private:
    Window* mapping;
    QVBoxLayout* mainLayout;
    QVBoxLayout* layoutChoix;
    QLabel* txt;
    QPushButton* valider;
};

#endif // CHOIXMOTIFDIALOG_HPP
