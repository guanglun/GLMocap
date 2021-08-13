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

FormCamConfig::FormCamConfig(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::FormCamConfig)
{
    ui->setupUi(this);
    this->setWindowTitle("camera config");
    ui->comboBoxCamSize->addItems(FrameSizeStr);

    ui->le_exposure->setValidator(new QIntValidator(0, 99999999, this));
}

void FormCamConfig::setQData(QList<OPENVIO *> *vioList, OPENVIO *vio)
{
    this->vioList = vioList;
    this->vio = vio;

    if (vioList != NULL)
    {
        this->setWindowTitle("camera config select " + QString::number(vioList->size()));
    }

    if (vio != NULL)
    {
        this->setWindowTitle("camera config select " + QString(vio->idShort));
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
    //qwinusb->ctrlCamStart();
}

void FormCamConfig::on_pb_cam_stop_clicked()
{
    //qwinusb->ctrlCamStop();
}

void FormCamConfig::on_pb_set_config_exposure_clicked()
{
    int exposure_value = 0;

    exposure_value = ui->le_exposure->text().toInt();

    if (ui->cb_exposure->checkState() == Qt::CheckState::Checked)
    {
        exposure_value = -exposure_value;
    }

    //DBG("exposure_value %d",exposure_value);
    if (vioList != NULL)
    {
        for (int i = 0; i < vioList->size(); i++)
        {
            OPENVIO *vio = vioList->at(i);
            vio->open();
            if (vio->ctrlCamSetExposure(exposure_value) == 0)
            {
                vio->setStatus("Set Exposure " + QString::number(exposure_value) + " Success");
            }
            else
            {
                vio->setStatus("Set Exposure " + QString::number(exposure_value) + " Fail");
            }
        }
    }
    else if (vio != NULL)
    {
        vio->open();
        if (vio->ctrlCamSetExposure(exposure_value) == 0)
        {
            vio->setStatus("Set Exposure " + QString::number(exposure_value) + " Success");
        }
        else
        {
            vio->setStatus("Set Exposure " + QString::number(exposure_value) + " Fail");
        }
    }
}

void FormCamConfig::on_pb_set_config_image_size_clicked()
{
    int num = ui->comboBoxCamSize->currentIndex();
    DBG("set frame num %d", num);
    //qwinusb->ctrlCamSetFrameSizeNum(num);
}
