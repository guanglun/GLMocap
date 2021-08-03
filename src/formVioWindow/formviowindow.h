#ifndef FORMVIOWINDOW_H
#define FORMVIOWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>
#include <QStatusBar>
#include <QLabel>
#include <QTimer>

#include "workspace.h"
#include "openvio.h"

#include "formcvwindow.h"

namespace Ui {
class FormVioWindow;
}

class FormVioWindow : public QWidget
{
    Q_OBJECT
    
public:
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
    void camSlot(int index);
    void imuSlot(int index);
    void on_pb_vision_clicked();
    void visionImageSlot(QImage qImage);  
signals:
    void imageSignals(QImage qImage,int flag);
    
private:
    Ui::FormVioWindow *ui;
    
protected: 
     void closeEvent(QCloseEvent *event);     
};

#endif // 
