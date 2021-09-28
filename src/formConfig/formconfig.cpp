#include "formconfig.h"
#include "ui_formconfig.h"
#include <QFileDialog>

FormConfig::FormConfig(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::FormConfig)
{
    ui->setupUi(this);

    this->setWindowTitle("config");
}

FormConfig::~FormConfig()
{
    delete ui;
}

void FormConfig::showEvent(QShowEvent *event)
{
    ui->le_image_save_path->setText(setting->getImagePath());
    ui->sp_threshold->setValue(setting->getThreshold());
    ui->sp_num_cam->setValue(setting->getCamNumber());
}

void FormConfig::on_pb_exit_clicked()
{
    close();
}

void FormConfig::on_pb_save_num_cam_clicked()
{
    setting->setCamNumber(ui->sp_num_cam->value());
}

void FormConfig::on_pb_set_threshold_clicked()
{
    setting->setThreshold(ui->sp_threshold->value());
}

void FormConfig::on_pb_set_image_save_path_clicked()
{
    QString path = setting->getImagePath();
    if (path.length() == 0)
    {
        path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    QString srcDirPath = QFileDialog::getExistingDirectory(
        this, "choose image save directory",
        path);

    if (srcDirPath.isEmpty())
    {
        return;
    }
    else
    {
        ui->le_image_save_path->setText(srcDirPath);
        setting->setImagePath(srcDirPath);
    }
}
