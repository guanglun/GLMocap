#ifndef FORMCAMWINDOW_H
#define FORMCAMWINDOW_H


#include "workspace.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QMessageBox>

#include "openvio.h"

#include "MultipleViewTriangulation.h"

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
#include "formconfig.h"
#include "formcalibrwindow.h"
#include "formpx4window.h"
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
    FormPx4Window fPx4Window;
    FormConfig fConfig;
    FormCalibrWindow fCalibrWindow;
    MuItemCtrl *muItemCtrl;
    FormCamConfig *formCamConfig;

    explicit FormCamWindow(QWidget *parent = nullptr);
    ~FormCamWindow();
    OPENVIO *getIndexVio(const QModelIndex &index);
private slots:
    void on_pb_capture_clicked();
    void on_pb_init_module_clicked();

    void vioItemSelected(const QModelIndex &index);
    void doubleClickedSlot(const QModelIndex &index);
    void onTimeOut(); 
    void ProvideContextMenu(const QPoint &pos);
    void on_action_position_triggered();
    void on_action3d_view_triggered();
    void on_actionPx4_view_triggered();
    
    void on_actionLog_view_triggered();
    void on_actionUpgrade_triggered();
    void on_actionConfig_triggered();
    void on_actionLoad_vision_param_triggered();
    void on_actionsave_vision_param_triggered();
    void on_actionsettings_triggered();
    void on_actioncalibration_triggered();
    void on_actioninit_gnd_triggered();
    void on_actionstart_triggered();
    void on_actionstop_triggered();
    void on_actionopen_all_view_triggered();
private:
    Ui::FormCamWindow *ui;
    
signals:
    void ctrlMultemCamStartSignal(void);
    void ctrlMultemCamStopSignal(void);
    
protected: 
     void closeEvent(QCloseEvent *event);     
};

#endif
