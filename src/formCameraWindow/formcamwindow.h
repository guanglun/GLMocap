#ifndef FORMCAMWINDOW_H
#define FORMCAMWINDOW_H

#include <QWidget>
#include "workspace.h"

namespace Ui {
class FormCamWindow;
}

class FormCamWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormCamWindow(QWidget *parent = 0);
    ~FormCamWindow();

// private slots:


private:
    Ui::FormCamWindow *ui;
};

#endif
