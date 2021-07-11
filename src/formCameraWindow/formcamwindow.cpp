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
}

void FormCamWindow::ProvideContextMenu(const QPoint &pos)
{
    QPoint item = ui->lv_openvio->mapToGlobal(pos);
    
    QMenu submenu;
    submenu.addAction("Rename");
    //submenu.addAction("Delete");
    QAction *rightClickItem = submenu.exec(item);
    if (rightClickItem && rightClickItem->text().contains("Rename"))
    {
        
        QString dlgTitle = QString(openvioList.at(ui->lv_openvio->indexAt(pos).row())->idStr);
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

void FormCamWindow::vioItemSelected(const QModelIndex &index)
{
}
void FormCamWindow::doubleClickedSlot(const QModelIndex &index)
{
    if (openvioList.at(index.row())->is_open == false)
    {
        FormVioWindow *formVioWindow = new FormVioWindow();
        formVioWindow->setQData(openvioList.at(index.row()));
        formVioWindow->show();
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
        vio->setStatus(speedStr);
    }

    status_speed->setText(getSpeed(recv_count_1s));
}

void FormCamWindow::on_actionImg_save_path_triggered()
{

        
    //     QString dlgTitle = QStringLiteral("set images save path"); 
    //     QString txtLabel = QStringLiteral("input new path："); 
    //     QString defaultInput = QStringLiteral("");  

	// QInputDialog inputDialog=new QInputDialog(this);
    // inputDialog->setOkButtonText(QString::fromLocal8Bit("确定"));
    // inputDialog->setCancelButtonText(QString::fromLocal8Bit("取消"));
    // inputDialog->setInputMode(QInputDialog::TextInput);
    // inputDialog->setLabelText("Select a channel");

    // connect(getChNo,SIGNAL(intValueSelected(int)),this,SLOT(getChSelected(int)));
	// inputDialog->show();    //在需要获取用户输入的地方调用show()


    QString srcDirPath = QFileDialog::getExistingDirectory(
               this, "choose src Directory",
                "/");

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