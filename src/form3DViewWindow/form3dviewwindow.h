#ifndef FORM3DVIEWWINDOW_H
#define FORM3DVIEWWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>
#include <QTimer>

#include "workspace.h"

#include "camview.h"

namespace Ui {
class Form3DViewWindow;
}

class Form3DViewWindow : public QWidget
{
    Q_OBJECT
    
public:
    CamView *camview;
    
    explicit Form3DViewWindow(QWidget *parent = 0);
    ~Form3DViewWindow();
private:
    QTimer *timer;

private slots:
    void onXYZSlot(Vector3d *Xr,int size); 
    void onTimeOut();
signals:

private:
    Ui::Form3DViewWindow *ui;
    
protected: 

};

#endif // 
