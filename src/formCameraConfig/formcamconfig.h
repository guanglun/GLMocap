#ifndef FORMCAMCONFIG_H
#define FORMCAMCONFIG_H

#include <QWidget>
#include "workspace.h"

class OPENVIO;
#include "openvio.h"

namespace Ui {
class FormCamConfig;
}

class FormCamConfig : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormCamConfig(QWidget *parent = 0);
    ~FormCamConfig();
    OPENVIO *vio;
    void setQData(OPENVIO *vio);
private slots:
    void on_pb_set_config_clicked();
    
    void on_pb_exit_clicked();
    
    void on_pb_cam_start_clicked();
    
    void on_pb_cam_stop_clicked();
    
    void on_pb_set_config_exposure_clicked();

    void on_pb_set_config_image_size_clicked();

private:
    Ui::FormCamConfig *ui;
};

#endif // FORMCAMCONFIG_H
