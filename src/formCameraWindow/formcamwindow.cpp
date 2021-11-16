#include "formcamwindow.h"
#include "ui_formcamwindow.h"

#include <QDesktopWidget>

Setting *setting;
Log *mlog;

FormCamWindow::FormCamWindow(QWidget *parent) : QMainWindow(parent),
                                                ui(new Ui::FormCamWindow)
{
    ui->setupUi(this);

    QDesktopWidget *desktopWidget = QApplication::desktop();
    move(desktopWidget->screenGeometry().width() / 2,
         desktopWidget->screenGeometry().height() / 2 - this->height() / 2);

    this->setWindowTitle("GLMocap");
    setWindowIcon(QIcon(":/image/logo.ico"));

    setting = new Setting();

    qwinusb = new WinUSBDriver();

    mlog = new Log();

    connect(mlog, SIGNAL(logSignal(QString)), &fLogWindow, SLOT(logSlot(QString)));
    mlog->show("GLMocap StartUp", LOG_DEBUG);

    QStandardItemModel *pModelOpenvio = new QStandardItemModel();
    MuItemCam *pItemCam = new MuItemCam(this);

    ui->lv_openvio->setItemDelegate(pItemCam);
    ui->lv_openvio->setModel(pModelOpenvio);
    ui->lv_openvio->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lv_openvio->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->lv_openvio, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ProvideContextMenu(const QPoint &)));

    connect(ui->lv_openvio, SIGNAL(clicked(QModelIndex)), this, SLOT(vioItemSelected(QModelIndex)));
    connect(ui->lv_openvio, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickedSlot(QModelIndex)));

    qwinusb->setModule(pModelOpenvio);

    muItemCtrl = new MuItemCtrl(this);

    status_speed = new QLabel(this);
    ui->statusBar->addWidget(status_speed);
    status_speed->setText("0");

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(1000);

    QString path = setting->getVisionParamPath();
    if (path.length() != 0)
    {
        mlog->show("vision param path :" + path);
        setting->loadVisionParam(path);
    }

    upgrade = new FirmwareUpgrade();
    connect(qwinusb, SIGNAL(newSignal(OPENVIO *)), upgrade->upgradeThread, SLOT(newSlot(OPENVIO *)));

    ctrlProcess = new CtrlProcess(this);
    ctrlProcess->moveToThread(&ctrlProcessThread);
    ctrlProcessThread.start();
    connect(this, SIGNAL(ctrlMultemCamStartSignal()), ctrlProcess, SLOT(ctrlMultemCamStartSlot()));
    connect(this, SIGNAL(ctrlMultemCamStopSignal()), ctrlProcess, SLOT(ctrlMultemCamStopSlot()));

    formCamConfig = new FormCamConfig(ctrlProcess);

    fPx4Window.setPX4Thread(&qwinusb->px4Thread);
}

void FormCamWindow::ProvideContextMenu(const QPoint &pos)
{
    OPENVIO *vio = getIndexVio(ui->lv_openvio->indexAt(pos));

    mlog->show("[[[SELECT]]] " + QString(vio->idShort));

    QPoint item = ui->lv_openvio->mapToGlobal(pos);

    QMenu submenu;
    if (vio->type == TYPE_OPENVIO)
    {
        submenu.addAction("Setup");
    }
    submenu.addAction("Set Camera Number");
    submenu.addAction("Upgrade Firmware");
    if (vio->type == TYPE_OPENVIO)
    {
        submenu.addAction("Camera Start");
        submenu.addAction("Camera Stop");
    }

    submenu.addAction("Reboot Now");
    submenu.addAction("Reboot To Bootloader");

    QAction *rightClickItem = submenu.exec(item);
    if (rightClickItem && rightClickItem->text().contains("Setup"))
    {
        formCamConfig->setQData(nullptr, vio);
        if (formCamConfig->isVisible() == false)
        {
            formCamConfig->show();
        }
    }
    else if (rightClickItem && rightClickItem->text().contains("Set Camera Number"))
    {

        QString dlgTitle = QString(vio->idShort);
        QString txtLabel = QStringLiteral("input camera number :");
        bool ok = false;
        int number = QInputDialog::getInt(this, dlgTitle, txtLabel, 0, 0, 255, 1, &ok);
        if (ok)
        {
            vio->setNumber(number);
        }
    }
    else if (rightClickItem && rightClickItem->text().contains("Upgrade Firmware"))
    {
        QString path = setting->getFirmwarePath();
        if (path.length() == 0)
        {
            path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        }
        QString filePath = QFileDialog::getOpenFileName(
            this,
            tr("choose a firmware file"),
            path,
            tr("param(*.bin);;"));
        if (filePath.isEmpty())
        {
            QMessageBox::warning(this, "Warning!", "Failed to open the firmware!");
        }
        else
        {
            setting->setFirmwarePath(filePath);

            upgrade->setOPENVIO(vio);
            upgrade->setBinPath(filePath);
            upgrade->upgradeStart();
        }
    }
    else if (rightClickItem && rightClickItem->text().contains("Reboot Now"))
    {
        vio->open();
        //vio->camRecvStop();
        vio->ctrlReboot(1);
    }
    else if (rightClickItem && rightClickItem->text().contains("Reboot To Bootloader"))
    {
        vio->open();
        //vio->camRecvStop();
        vio->ctrlReboot(0);
    }
    else if (rightClickItem && rightClickItem->text().contains("Camera Start"))
    {
        vio->open();
        vio->ctrlCamStatus(1);
    }
    else if (rightClickItem && rightClickItem->text().contains("Camera Stop"))
    {
        vio->open();
        vio->ctrlCamStatus(0);
    }
}

FormCamWindow::~FormCamWindow()
{
    ctrlProcessThread.quit();
    ctrlProcessThread.wait();
    DBG("ctrlProcessThread exit");

    delete ui;
}

void FormCamWindow::on_actionstart_triggered()
{
    ctrlProcess->setVio(&qwinusb->vioMap, nullptr);
    qwinusb->visionProcess->init(qwinusb->vioMap.size());
    emit ctrlMultemCamStartSignal();
}

void FormCamWindow::on_actionstop_triggered()
{
    ctrlProcess->setVio(&qwinusb->vioMap, nullptr);
    emit ctrlMultemCamStopSignal();
}

void FormCamWindow::closeEvent(QCloseEvent *event)
{

    qwinusb->timer->stop();

    ctrlProcessThread.quit();
    ctrlProcessThread.wait();

    if (f3DViewWindow.isEnabled())
        f3DViewWindow.close();
    if (fVisionWindow.isEnabled())
        fVisionWindow.close();
    if (fLogWindow.isEnabled())
        fLogWindow.close();
    if (formCamConfig->isEnabled())
        formCamConfig->close();
    if (fPx4Window.isEnabled())
        fPx4Window.close();
    if (fConfig.isEnabled())
        fConfig.close();
    if (fCalibrWindow.isEnabled())
        fCalibrWindow.close();
    if (fAbout.isEnabled())
        fAbout.close();
    qwinusb->closeDevice();
}

static bool isDirExistOrCreat(QString fullPath)
{
    QDir dir(fullPath);
    if (dir.exists())
    {
        return true;
    }
    else
    {
        bool ok = dir.mkdir(fullPath);
        return ok;
    }
}

static bool isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if (dir.exists())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void FormCamWindow::on_actionConfig_triggered()
{
    formCamConfig->setQData(&qwinusb->vioMap, nullptr);
    if (formCamConfig->isVisible() == false)
    {
        formCamConfig->show();
    }
}

void FormCamWindow::on_actionUpgrade_triggered()
{

    if (!muItemCtrl->muItemCtrlThread->isRunning())
    {
        muItemCtrl->setCtrl(CTRL_TYPE_UPGRADE, &qwinusb->vioMap);
        muItemCtrl->start();
    }
}

void FormCamWindow::on_pb_capture_clicked()
{
    QString savePath = setting->imagePath;

    if (ui->pb_capture->text().contains("Capture"))
    {

        if (isDirExistOrCreat(savePath) == false)
        {
            mlog->show(setting->imagePath + " not exist!");
            return;
        }

        if (qwinusb->vioMap.size() > 0)
        {
            QDateTime time = QDateTime::currentDateTime();
            QString current_date = time.toString("yyyy-MM-dd-hh-mm-ss-zzz");
            savePath += +"/" + current_date;
            isDirExistOrCreat(savePath);
        }

        for (QMap<uint8_t, OPENVIO *>::Iterator it = qwinusb->vioMap.begin();
             it != qwinusb->vioMap.end(); it++)
        {
            OPENVIO *vio = it.value();
            if (vio->dev_handle != nullptr)
            {
                if (vio->number == -1)
                {
                    vio->saveImagePath = savePath + "/" + QString(vio->idShort);
                }
                else
                {
                    vio->saveImagePath = savePath + "/" + vio->name;
                }
                isDirExistOrCreat(vio->saveImagePath);
            }
        }

        qwinusb->visionProcess->isCapImage = true;
        ui->pb_capture->setText("Stop");
    }
    else
    {
        qwinusb->visionProcess->isCapImage = false;
        ui->pb_capture->setText("Capture");
    }
}

void FormCamWindow::on_pb_quick_start_clicked()
{
    on_actionopen_all_view_triggered();
    on_actionstart_triggered();
}

void FormCamWindow::on_pb_init_module_clicked()
{
    qwinusb->visionProcess->matchState = MATCH_START;
    qwinusb->visionProcess->findDroneState = FIND_MODULE_START;
}

void FormCamWindow::on_actioninit_gnd_triggered()
{
    qwinusb->visionProcess->matchState = MATCH_START;
    qwinusb->visionProcess->calGNDstate = CAL_START;
}

// void FormCamWindow::on_pb_find_drone_clicked()
// {
//     qwinusb->visionProcess->matchState = MATCH_START;
//     qwinusb->visionProcess->findDroneState = FIND_MODULE_START;
// }

void FormCamWindow::vioItemSelected(const QModelIndex &index)
{
}

OPENVIO *FormCamWindow::getIndexVio(const QModelIndex &index)
{
    OPENVIO *vio = nullptr;
    for (QMap<uint8_t, OPENVIO *>::Iterator it = qwinusb->vioMap.begin();
         it != qwinusb->vioMap.end(); it++)
    {
        if (it.value()->getRow() == index.row())
        {
            vio = it.value();
        }
    }

    return vio;
}

void FormCamWindow::doubleClickedSlot(const QModelIndex &index)
{
    OPENVIO *vio = getIndexVio(index);

    if (vio->type == TYPE_OPENVIO)
    {
        if (vio->formVioWindow == nullptr)
        {
            vio->formVioWindow = new FormVioWindow(ctrlProcess);
            vio->formVioWindow->setQData(vio);
        }

        if (vio->formVioWindow->isVisible() == false)
        {
            vio->formVioWindow->show();
            //vio->open();
            //vio->camStart();
        }
    }
}

void FormCamWindow::on_actionopen_all_view_triggered()
{
    for (QMap<uint8_t, OPENVIO *>::Iterator it = qwinusb->vioMap.begin();
         it != qwinusb->vioMap.end(); it++)
    {

        OPENVIO *vio = it.value();
        if (vio != nullptr)
        {
            if (vio->type == TYPE_OPENVIO || vio->type == TYPE_VIRTUAL)
            {
                if (vio->formVioWindow == nullptr)
                {
                    vio->formVioWindow = new FormVioWindow(ctrlProcess);
                    vio->formVioWindow->setQData(vio);
                }

                if (vio->formVioWindow->isVisible() == false)
                {
                    vio->formVioWindow->show();
                    //vio->open();
                    //vio->camStart();
                }
            }
        }
    }
}

QString getSpeed(int speed)
{
    QString speedStr;
    if (speed < 1024)
    {
        speedStr = QString::number(speed);
        speedStr += "B/s";
    }
    else if (speed < 1024 * 1024)
    {
        speedStr = QString::number(speed / 1024);
        speedStr += "KB/s";
    }
    else if (speed < 1024 * 1024 * 1024)
    {
        speedStr = QString::number(speed / 1024 / 1024);
        speedStr += "MB/s";
    }
    return speedStr;
}

void FormCamWindow::onTimeOut()
{
    // ui->lb_imu_hz->setText(QString::number(qwinusb->imu_hz)+"Hz");
    // qwinusb->imu_hz = 0;
    recv_count_1s = 0;

    for (QMap<uint8_t, OPENVIO *>::Iterator it = qwinusb->vioMap.begin();
         it != qwinusb->vioMap.end(); it++)
    {

        OPENVIO *vio = it.value();
        if (vio != nullptr)
        {
            QString speedStr;
            recv_count_1s += vio->recv_count_1s;
            speedStr += getSpeed(vio->recv_count_1s);
            speedStr += " ";
            speedStr += QString::number(vio->frame_fps);
            speedStr += "fps";
            vio->frame_fps = 0;

            vio->recv_count_1s = 0;
            vio->setSpeed(speedStr);
        }
    }

    status_speed->setText(getSpeed(recv_count_1s) + " " + QString::number(qwinusb->visionProcess->count) + "position/s");
    qwinusb->visionProcess->count = 0;
}

void FormCamWindow::on_action_position_triggered()
{
    if (!fVisionWindow.isVisible())
    {
        connect(qwinusb->visionProcess, SIGNAL(onXYZSignals(Vector3d *, Vector3d *, int)), &fVisionWindow, SLOT(onXYZSlot(Vector3d *, Vector3d *, int)));
        fVisionWindow.show();
    }
}

void FormCamWindow::on_action3d_view_triggered()
{
    if (!f3DViewWindow.isVisible())
    {
        connect(qwinusb->visionProcess, SIGNAL(onXYZSignals(Vector3d *, Vector3d *, int)), &f3DViewWindow, SLOT(onXYZSlot(Vector3d *, Vector3d *, int)));
        connect(&fPx4Window, SIGNAL(onPlanSignals(QList<PlanPoint *>)), &f3DViewWindow, SLOT(onPlanSlot(QList<PlanPoint *>)));
        f3DViewWindow.show();
    }
}

void FormCamWindow::on_actionPx4_view_triggered()
{
    if (!fPx4Window.isVisible())
    {
        connect(&fPx4Window, SIGNAL(onPlanSignals(QList<PlanPoint *>)), &f3DViewWindow, SLOT(onPlanSlot(QList<PlanPoint *>)));
        fPx4Window.show();
    }
}

void FormCamWindow::on_actionsettings_triggered()
{
    if (!fConfig.isVisible())
    {
        fConfig.show();
    }
}

void FormCamWindow::on_actioncalibration_triggered()
{
    if (!fCalibrWindow.isVisible())
    {
        fCalibrWindow.show();
    }
}

void FormCamWindow::on_actionLog_view_triggered()
{
    if (!fLogWindow.isVisible())
    {
        fLogWindow.show();
    }
}

void FormCamWindow::on_actionabout_triggered()
{
    if (!fAbout.isVisible())
    {
        fAbout.show();
    }
}

void FormCamWindow::on_actionLoad_vision_param_triggered()
{
    QString path = setting->getVisionParamPath();
    if (path.length() == 0)
    {
        path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("choose a param file."),
        path,
        tr("param(*.ini);;"));
    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
    }
    else
    {
        setting->setVisionParamPath(filePath);
        setting->loadVisionParam(filePath);
    }
}

void FormCamWindow::on_actionsave_vision_param_triggered()
{
    QString path = setting->getSaveVisionParamPath();
    if (path.length() == 0)
    {
        path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("save param file."),
        path,
        tr("param(*.ini);;"));
    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
    }
    else
    {
        setting->setSaveVisionParamPath(filePath);
        setting->saveVisionParam(filePath);
    }
}

void FormCamWindow::on_actioninit_virtual_cameras_triggered()
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

    for (int i = 0; i < 4; i++)
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

void FormCamWindow::on_actionload_images_triggered()
{
    QString sPath = "C:\\Users\\27207\\Desktop\\temp\\0.image\\2021-11-15-23-30-04-267\\camera";

    QDateTime time = QDateTime::currentDateTime();

    for (QMap<uint8_t, OPENVIO *>::Iterator it = qwinusb->vioMap.begin();
         it != qwinusb->vioMap.end(); it++)
    {
        OPENVIO *vio = it.value();
        QImage image(sPath+QString::number(vio->number)+"\\1636990252145.png");
        if(!image.isNull())
            vio->camProcess->emitImage(image,time);
    }
}
