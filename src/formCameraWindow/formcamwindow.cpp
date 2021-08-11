#include "formcamwindow.h"
#include "ui_formcamwindow.h"

Setting *setting;

FormCamWindow::FormCamWindow(QWidget *parent) : QMainWindow(parent),
                                                ui(new Ui::FormCamWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("OPENVIO");

    setting = new Setting();

    qwinusb = new WinUSBDriver();

    mlog = new Log();
    mlog->show("OPENVIO StartUp", LOG_DEBUG);

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
    qwinusb->setOpenvioList(&openvioList);

    status_speed = new QLabel(this);
    ui->statusBar->addWidget(status_speed);
    status_speed->setText("0");

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(1000);

    QString path = setting->getVisionParamPath();
    if (path.length() != 0)
    {
        setting->loadVisionParam(path);
    }

    qwinusb->scan();
}

void FormCamWindow::ProvideContextMenu(const QPoint &pos)
{
    OPENVIO *vio = openvioList.at(ui->lv_openvio->indexAt(pos).row());

    mlog->show("[[[SELECT]]] " + QString(vio->idShort));

    QPoint item = ui->lv_openvio->mapToGlobal(pos);

    QMenu submenu;
    submenu.addAction("Rename");
    submenu.addAction("Upgrade Firmware");
    QAction *rightClickItem = submenu.exec(item);
    if (rightClickItem && rightClickItem->text().contains("Rename"))
    {

        QString dlgTitle = QString(vio->idShort);
        QString txtLabel = QStringLiteral("input new name：");
        QString defaultInput = QStringLiteral("camera0");
        QLineEdit::EchoMode echoMode = QLineEdit::Normal;
        bool ok = false;
        QString text = QInputDialog::getText(this, dlgTitle, txtLabel, echoMode, defaultInput, &ok);
        if (ok && !text.isEmpty())
        {
            openvioList.at(ui->lv_openvio->indexAt(pos).row())->setName(text);
        }
    }
    if (rightClickItem && rightClickItem->text().contains("Upgrade Firmware"))
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
            upgrade = new FirmwareUpgrade(vio);
            connect(qwinusb, SIGNAL(newSignal(OPENVIO *)),upgrade->upgradeThread, SLOT(newSlot(OPENVIO *)));
            upgrade->setBinPath(filePath);
            upgrade->upgradeStart();
        }
    }
}

FormCamWindow::~FormCamWindow()
{
    delete ui;
}

void FormCamWindow::on_pb_scan_camera_clicked()
{
    //mlog->show("Scan",LOG_DEBUG);
    qwinusb->scan();
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
        bool ok = dir.mkdir(fullPath); //只创建一级子目录，即必须保证上级目录存在
        return ok;
    }
}

void FormCamWindow::on_pb_capture_clicked()
{
    for (int i = 0; i < openvioList.length(); i++)
    {
        OPENVIO *vio = openvioList.at(i);
        if (vio->is_open == true)
        {
            if (vio->name.isEmpty() == true)
            {
                vio->saveImagePath = setting->imagePath + "/" + QString(vio->idShort);
            }
            else
            {
                vio->saveImagePath = setting->imagePath + "/" + vio->name;
            }
            isDirExist(vio->saveImagePath);
            //mlog->show(vio->saveImagePath);

            //vio->saveImagePath = setting->imagePath;
        }
    }

    for (int i = 0; i < openvioList.length(); i++)
    {
        OPENVIO *vio = openvioList.at(i);
        if (vio->is_open == true)
        {
            vio->isCapImage = true;
        }
    }
}

void FormCamWindow::vioItemSelected(const QModelIndex &index)
{
}
void FormCamWindow::doubleClickedSlot(const QModelIndex &index)
{
    OPENVIO *vio = openvioList.at(index.row());

    if (vio->isCamRecv == false)
    {
        if(vio->formVioWindow == NULL)
        {
            vio->formVioWindow = new FormVioWindow();
            vio->formVioWindow->setQData(openvioList.at(index.row()));

            connect(vio->formVioWindow->formCvWindow, SIGNAL(positionSignals(int, double, double)), &multipleViewTriangulation, SLOT(positionSlot(int, double, double)));
        }

        if(vio->formVioWindow->isVisible() == false)
        {
            vio->formVioWindow->show();
            if(vio->open())
            {
                vio->camStart();
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
    for (int i = 0; i < openvioList.length(); i++)
    {
        OPENVIO *vio = openvioList.at(i);
        QString speedStr;
        recv_count_1s += vio->recv_count_1s;
        speedStr += getSpeed(vio->recv_count_1s);
        speedStr += " ";
        speedStr += QString::number(vio->frame_fps);
        speedStr += "fps";
        vio->frame_fps = 0;

        vio->recv_count_1s = 0;

        if(vio->isShowSpeed)
            vio->setStatus(speedStr);
    }

    status_speed->setText(getSpeed(recv_count_1s));
}

void FormCamWindow::on_action_position_triggered()
{
    if (!fVisionWindow.isActiveWindow())
    {
        connect(&multipleViewTriangulation, SIGNAL(onXYZSignals(double, double, double)), &fVisionWindow, SLOT(onXYZSlot(double, double, double)));
        fVisionWindow.show();
    }
}

void FormCamWindow::on_action3d_view_triggered()
{
    if (!f3DViewWindow.isActiveWindow())
    {
        connect(&multipleViewTriangulation, SIGNAL(onXYZSignals(double, double, double)), &f3DViewWindow, SLOT(onXYZSlot(double, double, double)));
        f3DViewWindow.show();
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

void FormCamWindow::on_actionImg_save_path_triggered()
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
        setting->setImagePath(srcDirPath);
        //qDebug() << "srcDirPath=" << srcDirPath;
    }
}