#ifndef FORM3DVIEWWINDOW_H
#define FORM3DVIEWWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>
#include <QTimer>

#include "workspace.h"

#include "camview.h"
#include "glwidget.h"

namespace Ui {
class Form3DViewWindow;
}

class Form3DViewWindow : public QWidget
{
    Q_OBJECT
    
public:
    CamView *camview;
    GLWidget* view;
    explicit Form3DViewWindow(QWidget *parent = 0);
    ~Form3DViewWindow();
private:
    QTimer *timer;

private slots:
    void onXYZSlot(Vector3d *Xr,Vector3d *pos,int size); 
    void onPlanSlot(QList<PlanPoint *> list);
    void onTimeOut();

    void cbShowTrajectoryStateChangedSlot(int);
    void cbShowPlanStateChangedSlot(int); 
    void cbShowAxisStateChangedSlot(int); 
    void cbShowCamStateChangedSlot(int); 
    
    void on_pb_reset_trajectory_clicked();
    
signals:

private:
    Ui::Form3DViewWindow *ui;
    
protected: 

};

#endif // 
