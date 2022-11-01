#include "formvirtual.h"
#include "ui_formvirtual.h"
#include <QFileDialog>

FormVirtual::FormVirtual(QWidget *parent) : QWidget(parent),
                                            ui(new Ui::FormVirtual)
{
    ui->setupUi(this);

    this->setWindowTitle("virtual");
}

FormVirtual::~FormVirtual()
{
    delete ui;
}

void FormVirtual::set(WinUSBDriver *qwinusb)
{
    this->qwinusb = qwinusb;
}

void FormVirtual::showEvent(QShowEvent *event)
{
    ui->le_load_image_path->setText(setting->getVirtualImagePath());

    if(qwinusb->vioMap.size() == 0)
    {
        init(ui->le_load_image_path->text());
    }
}

void FormVirtual::on_pb_load_image_path_clicked()
{
    QString path = setting->getVirtualImagePath();
    if (path.length() == 0)
    {
        path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    QString srcDirPath = QFileDialog::getExistingDirectory(
        this, "choose virtual image load directory",
        path);

    if (srcDirPath.isEmpty())
    {
        return;
    }
    else
    {
        ui->le_load_image_path->setText(srcDirPath);
        setting->setVirtualImagePath(srcDirPath);
    }
    init(srcDirPath);
}

void FormVirtual::init(QString srcDirPath)
{
    imageDirPath = srcDirPath;
    // mlog->show("virtual image path: " + imageDirPath);
    files = checkFile(imageDirPath,&camVirtualNumber);
    if(!files.isEmpty())
    {
        initVirtualCamera();
        index = setting->getVirtualIndex();
        if(index >= files.size())
            index = 0;

        ui->lb_now->setText(files.at(index));
        ui->lb_index->setText(QString::number(index + 1)+"/"+QString::number(files.size()));
        ui->lb_cam_number->setText(QString::number(camVirtualNumber));            
    }



}

void FormVirtual::initVirtualCamera()
{
    QMap<uint8_t, OPENVIO *>::Iterator it = qwinusb->vioMap.begin();
    while (it != qwinusb->vioMap.end())
    {
        if (it.value() != nullptr)
        {
            it.value()->removeReady();
            it = qwinusb->vioMap.erase(it);
        }
    }

    for (int i = 0; i < camVirtualNumber; i++)
    {
        OPENVIO *vio = new OPENVIO(nullptr, TYPE_VIRTUAL);
        vio->devAddr = i;
        qwinusb->vioMap.insert(vio->devAddr, vio);
        vio->setItem(qwinusb->pModelOpenvio, i);
        vio->visionProcess = qwinusb->visionProcess;
        qwinusb->visionProcess->init(qwinusb->vioMap.size());
        connect(vio->camProcess, SIGNAL(positionSignals(CAMERA_RESULT)), vio->visionProcess, SLOT(positionSlot(CAMERA_RESULT)));
    }
}

void FormVirtual::load()
{
    QDateTime time = QDateTime::currentDateTime();

    for (QMap<uint8_t, OPENVIO *>::Iterator it = qwinusb->vioMap.begin();
         it != qwinusb->vioMap.end(); it++)
    {
        OPENVIO *vio = it.value();
        QString imgPath = imageDirPath + "/camera" + QString::number(vio->number) + "/" + files.at(index);
        if (EasyTool::isFileExist(imgPath) == false)
        {
            mlog->show("virtual not found " + imgPath + ",exit");
            return;
        }
        QImage image(imgPath);
        if (!image.isNull())
        {
            vio->camProcess->emitImage(image, time);
        }
    }

    ui->lb_now->setText(files.at(index));
    ui->lb_index->setText(QString::number(index + 1)+"/"+QString::number(files.size()));
    ui->lb_cam_number->setText(QString::number(camVirtualNumber));
    setting->setVirtualIndex(index);
}

void FormVirtual::on_pb_previous_clicked()
{
    if (index > 0)
        index--;
    load();
    
}
void FormVirtual::on_pb_load_clicked()
{
    load();
}
void FormVirtual::on_pb_next_clicked()
{
    if (index < (files.size() - 1))
        index++;
    load();
}

QStringList FormVirtual::checkFile(QString path,int *num)
{
    int cameraNumber = 0;
    char buff[100];
    QString dirPath;
    QStringList nameFilters;

    QStringList files, camFiles;

    QDir camDir(path);
    nameFilters << "camera*";
    camFiles = camDir.entryList(nameFilters);
    if (camFiles.size() < 1)
    {
        mlog->show("virtual check camera number error,files.size < 1,exit");
        return files;
    }
    else
    {
        *num = camFiles.size();
        mlog->show("cam number : "+QString::number(*num));
    }

    nameFilters.clear();
    nameFilters << "*.png";
    for (int i = 0; i < *num; i++)
    {
        dirPath = path + "/camera" + QString::number(i);
        if (EasyTool::isDirExist(dirPath) == false)
        {
            mlog->show("virtual check not found " + dirPath + ",exit");
            return files;
        }
        else
        {
            if (i == 0)
            {
                QDir dir(dirPath);
                files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
                if (files.size() < 1)
                {
                    mlog->show("virtual check error,files.size < 1,exit");
                }
            }
            else
            {
                for (int ii = 0; ii < files.size(); ii++)
                {
                    if (EasyTool::isFileExist(dirPath + "/" + files.at(ii)) == false)
                    {
                        mlog->show("virtual check not found " + dirPath + "/" + files.at(ii) + ",exit");
                        return files;
                    }
                }
            }
        }
    }
    return files;
}
