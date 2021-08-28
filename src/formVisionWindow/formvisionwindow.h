#ifndef FORMVISIONWINDOW_H
#define FORMVISIONWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>

#include "workspace.h"

#include "MultipleViewTriangulation.h"

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
    void onXYZSlot(Vector3d *Xr,int size); 
signals:

private:
    Ui::FormVisionWindow *ui;
    
protected: 

};

#endif // 
