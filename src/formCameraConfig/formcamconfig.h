#ifndef FORMCAMCONFIG_H
#define FORMCAMCONFIG_H

#include <QWidget>
#include "workspace.h"

class OPENVIO;
#include "openvio.h"

class CtrlProcess;
#include "CtrlProcess.h"

namespace Ui {
class FormCamConfig;
}

class FormCamConfig : public QWidget
{
    Q_OBJECT
    
private:
    
public:
    CtrlProcess *ctrlProcess;

    explicit FormCamConfig(CtrlProcess *ctrlProcess,QWidget *parent = 0);
    ~FormCamConfig();
    QMap<uint8_t, OPENVIO*> *vioMap; 
    OPENVIO *vio;
    void setQData(QMap<uint8_t, OPENVIO*> *vioMap = nullptr,OPENVIO *vio = nullptr);
private slots:
    void on_pb_set_config_clicked();
    void on_pb_exit_clicked();
    void on_pb_cam_start_clicked();
    void on_pb_cam_stop_clicked();
    void on_pb_set_config_exposure_clicked();
    void on_pb_set_config_image_size_clicked();
    void on_pb_sync_clicked();
    void on_pb_set_config_fps_clicked();
    void on_pb_set_config_sync_mode_clicked();
    void on_pb_set_config_sync_start_clicked();
    void on_pb_set_config_sync_stop_clicked();
    void on_pb_set_config_pwm_clicked();
    void on_pb_save_num_cam_clicked();
private:
    Ui::FormCamConfig *ui;

signals:
    void setExposureSignal(int exposure);
    void syncSignal();
    void ctrlCamStatusSignal(unsigned char state);
    void ctrlCamSyncStatusSignal(unsigned char state);
    void ctrlCamSyncModeSignal(unsigned char mode);
    void ctrlCamFpsSignal(unsigned char fps);  
    void ctrlInfraredPwmSignal(unsigned char pwm);  
};

#endif // FORMCAMCONFIG_H
