#ifndef FORMVIOWINDOW_H
#define FORMVIOWINDOW_H

#include "workspace.h"

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>
#include <QStatusBar>
#include <QLabel>
#include <QTimer>
#include <QThread>

class OPENVIO;
#include "openvio.h"

class FormCvWindow;
#include "formcvwindow.h"

class CtrlProcess;
#include "CtrlProcess.h"

namespace Ui {
class FormVioWindow;
}

class FormVioWindow : public QWidget
{
    Q_OBJECT
    
private:
    QThread ctrlProcessThread;
    
public:
    CtrlProcess *ctrlProcess;
    explicit FormVioWindow(QWidget *parent = 0);
    ~FormVioWindow();
    OPENVIO *vio;
    FormCvWindow *formCvWindow;
    void setQData(OPENVIO *vio);

private:
    QTimer *timer;
    int fps_1s = 0;

private slots:
    void onTimeOut(); 
    void on_pb_vision_clicked();
    void on_pb_cam_clicked();
    void on_pb_sync_clicked();

    void visionImageSlot(QPixmap qImage);  
    void getCameraStatusSlot(void); 
signals:
    void imageSignals(QImage qImage,int flag);
    void ctrlCamStatusSignal(unsigned char state);
    void ctrlCamSyncStatusSignal(unsigned char state);
private:
    Ui::FormVioWindow *ui;
    
protected: 
     void closeEvent(QCloseEvent *event);     
};

#endif // 
