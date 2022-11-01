#ifndef SAVETEMPLATEDIALOG_HPP
#define SAVETEMPLATEDIALOG_HPP

#include <QtWidgets>

class Window;

class SaveTemplateDialog : public QDialog
{
public:
    SaveTemplateDialog(Window* win);

private:
    Window* mapping;
    QVBoxLayout* mainLayout;
    QGridLayout* templateLayout;
    QPushButton* valider;
    QLabel* title;
    QList<QImage*> liste_masques;
};

#endif // SAVETEMPLATEDIALOG_HPP
