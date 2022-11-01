#ifndef RADIOBUTTONMOTIF_HPP
#define RADIOBUTTONMOTIF_HPP

#include <QtWidgets>
#include <QRadioButton>

class RadioButtonMotif : public QRadioButton
{
    Q_OBJECT

public:
    RadioButtonMotif(QImage* img);
    RadioButtonMotif(QString txt);
    RadioButtonMotif(int num_masque);

public slots:
    void trigger_selected_signal(bool checked);
    void trigger_selected_img(bool checked);
    void trigger_selected_delete(bool checked);
    void trigger_selected_masque(bool checked);

signals:
    void selected(QString name);
    void selected_img(QImage* img);
    void kaboum(QString name);
    void selected_masque(int masqueId);

private:
    QString motif_name;
    QImage* image;
    int masqueId;
};

#endif // RADIOBUTTONMOTIF_HPP
