#ifndef FORMABOUT_H
#define FORMABOUT_H

#include <QWidget>
#include "workspace.h"


namespace Ui {
class FormAbout;
}

class FormAbout : public QWidget
{
    Q_OBJECT
    
private:
    Ui::FormAbout *ui;
    
public:

    explicit FormAbout(QWidget *parent = 0);
    ~FormAbout();

private slots:
    void on_pb_openlink_clicked();

};

#endif // FORMABOUT_H
