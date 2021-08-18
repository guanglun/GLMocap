#ifndef FORMCAMWINDOW_H
#define FORMCAMWINDOW_H


#include "workspace.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QMessageBox>

#include "openvio.h"

#include "multipleViewTriangulation.h"

#include "MuItemCam.h"
#include "winusbdriver.h"

class FormVioWindow;
#include "formviowindow.h"

#include <QTimer>
#include <QLabel>
#include <QInputDialog>
#include <QFileDialog>

#include "formvisionwindow.h"
#include "form3dviewwindow.h"
#include "formlogwindow.h"

#include "CtrlProcess.h"

class MuItemCtrl;
#include "MuItemCtrl.h"

namespace Ui {
class FormCamWindow;
}

class FormCamWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTimer *timer;
    QThread ctrlProcessThread;
    
public:
    CtrlProcess *ctrlProcess;
    WinUSBDriver *qwinusb;
    QLabel *status_msg,*status_speed;
    int recv_count_1s;
    FirmwareUpgrade *upgrade = nullptr;
    
    FormVisionWindow fVisionWindow;
    Form3DViewWindow f3DViewWindow;
    FormLogWindow fLogWindow;
    MuItemCtrl *muItemCtrl;
    FormCamConfig *formCamConfig;

    explicit FormCamWindow(QWidget *parent = nullptr);
    ~FormCamWindow();
    OPENVIO *getIndexVio(const QModelIndex &index);
private slots:
    void on_pb_start_clicked();
    void on_pb_stop_clicked();
    void on_pb_capture_clicked();
    void vioItemSelected(const QModelIndex &index);
    void doubleClickedSlot(const QModelIndex &index);
    void onTimeOut(); 
    void ProvideContextMenu(const QPoint &pos);
    void on_actionImg_save_path_triggered();
    void on_action_position_triggered();
    void on_action3d_view_triggered();
    void on_actionLoad_vision_param_triggered();
    void on_actionLog_view_triggered();
    void on_actionUpgrade_triggered();
    void on_actionConfig_triggered();
    
private:
    Ui::FormCamWindow *ui;
    
signals:
    void ctrlMultemCamStartSignal(void);
    void ctrlMultemCamStopSignal(void);
protected: 
     void closeEvent(QCloseEvent *event);     
};

#endif
