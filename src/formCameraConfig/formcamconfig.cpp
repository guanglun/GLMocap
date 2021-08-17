#include "formcamconfig.h"
#include "ui_formcamconfig.h"

const QStringList FrameSizeStr = {
    "0x0",
    "88x72 QQCIF",
    "176x144 QCIF",
    "352x288 CIF",
    "88x60 QQSIF",
    "176x120 QSIF",
    "352x240 SIF",
    "40x30 QQQQVGA",
    "80x60 QQQVGA",
    "160x120 QQVGA",
    "320x240 QVGA",
    "640x480 VGA",
    "60x40 HQQQVGA",
    "120x80 HQQVGA",
    "240x160 HQVGA",
    "64x32",
    "64x64",
    "128x64",
    "128x128",
    "128x160 LCD",
    "128x160 QQVGA2",
    "720x480 WVGA",
    "752x480 WVGA2",
    "800x600 SVGA",
    "1024x768 XGA",
    "1280x1024 SXGA",
    "1600x1200 UXGA",
    "240x240 LCD"};

FormCamConfig::FormCamConfig(CtrlProcess *ctrlProcess,QWidget *parent) : QWidget(parent),
                                                ui(new Ui::FormCamConfig)
{
    ui->setupUi(this);

    this->ctrlProcess = ctrlProcess;
    this->setWindowTitle("camera config");
    ui->comboBoxCamSize->addItems(FrameSizeStr);

    ui->le_exposure->setValidator(new QIntValidator(0, 99999999, this));




    connect(this, SIGNAL(setExposureSignal(int)), ctrlProcess, SLOT(setExposureSlot(int)));
    connect(this, SIGNAL(syncSignal()), ctrlProcess, SLOT(syncSlot()));

    connect(this, SIGNAL(ctrlCamStatusSignal(unsigned char)), ctrlProcess, SLOT(ctrlCamStatusSlot(unsigned char)));
    connect(this, SIGNAL(ctrlCamSyncStatusSignal(unsigned char)), ctrlProcess, SLOT(ctrlCamSyncStatusSlot(unsigned char)));
    connect(this, SIGNAL(ctrlCamSyncModeSignal(unsigned char)), ctrlProcess, SLOT(ctrlCamSyncModeSlot(unsigned char)));
    connect(this, SIGNAL(ctrlCamFpsSignal(unsigned char)), ctrlProcess, SLOT(ctrlCamFpsSlot(unsigned char)));
    connect(this, SIGNAL(ctrlInfraredPwmSignal(unsigned char)), ctrlProcess, SLOT(ctrlInfraredPwmSlot(unsigned char)));


    
}

void FormCamConfig::setQData(QList<OPENVIO *> *vioList, OPENVIO *vio)
{
    OPENVIO *configvio = NULL;
    this->vioList = vioList;
    this->vio = vio;

    if (vioList != NULL)
    {
        this->setWindowTitle("camera config select " + QString::number(vioList->size()));
        if(vioList->size() != 0)
        {
            configvio = vioList->at(0);
        }
    }else if (vio != NULL)
    {
        this->setWindowTitle("camera config select " + QString(vio->idShort));
        configvio = vio;
    }

    ctrlProcess->setVio(vioList,vio);

    if(configvio != NULL)
    {
        ui->le_exposure->setText(QString::number(configvio->exposure));
        ui->le_fps->setText(QString::number(configvio->camera_fps));
        ui->hs_pwm->setValue(configvio->infrared_pwm);

        if(configvio->is_sync_mode)
        {
            ui->cb_syncmode->setCheckState(Qt::CheckState::Checked);
        }else{
            ui->cb_syncmode->setCheckState(Qt::CheckState::Unchecked);
        }
        
    }
}

FormCamConfig::~FormCamConfig()
{
    delete ui;
}

void FormCamConfig::on_pb_set_config_clicked()
{
}

void FormCamConfig::on_pb_exit_clicked()
{
    close();
}

void FormCamConfig::on_pb_cam_start_clicked()
{
    ctrlProcess->setVio(vioList,vio);
    emit ctrlCamStatusSignal(1);
}

void FormCamConfig::on_pb_cam_stop_clicked()
{
    ctrlProcess->setVio(vioList,vio);
    emit ctrlCamStatusSignal(0);
}

void FormCamConfig::on_pb_set_config_exposure_clicked()
{
    
    int exposure_value = 0;

    exposure_value = ui->le_exposure->text().toInt();

    if (ui->cb_exposure->checkState() == Qt::CheckState::Checked)
    {
        exposure_value = -exposure_value;
    }
    ctrlProcess->setVio(vioList,vio);
    emit setExposureSignal(exposure_value);
}

void FormCamConfig::on_pb_sync_clicked()
{
    ctrlProcess->setVio(vioList,vio);
    emit syncSignal();
}

void FormCamConfig::on_pb_set_config_fps_clicked()
{
    uint8_t fps = ui->le_fps->text().toInt();
    ctrlProcess->setVio(vioList,vio);
    emit ctrlCamFpsSignal(fps);
}

void FormCamConfig::on_pb_set_config_sync_mode_clicked()
{
    ctrlProcess->setVio(vioList,vio);
    if (ui->cb_syncmode->checkState() == Qt::CheckState::Checked)
    {
        emit ctrlCamSyncModeSignal(1);
    }else{
        emit ctrlCamSyncModeSignal(0);
    }
}

void FormCamConfig::on_pb_set_config_sync_start_clicked()
{
    ctrlProcess->setVio(vioList,vio);
    emit ctrlCamSyncStatusSignal(1);
}

void FormCamConfig::on_pb_set_config_sync_stop_clicked()
{
    ctrlProcess->setVio(vioList,vio);
    emit ctrlCamSyncStatusSignal(0);
}

void FormCamConfig::on_pb_set_config_pwm_clicked()
{
    uint8_t pwm = ui->hs_pwm->value();
    ctrlProcess->setVio(vioList,vio);
    emit ctrlInfraredPwmSignal(pwm);
}

void FormCamConfig::on_pb_set_config_image_size_clicked()
{
    int num = ui->comboBoxCamSize->currentIndex();
    DBG("set frame num %d", num);
    //qwinusb->ctrlCamSetFrameSizeNum(num);
}
