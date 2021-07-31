#ifndef FORMVISIONWINDOW_H
#define FORMVISIONWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>

#include "workspace.h"

namespace Ui {
class FormVisionWindow;
}

class FormVisionWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormVisionWindow(QWidget *parent = 0);
    ~FormVisionWindow();

private slots:
    void onXYZSlot(double x,double y,double z); 
signals:

private:
    Ui::FormVisionWindow *ui;
    
protected: 

};

#endif // 
