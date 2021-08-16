#include "formviowindow.h"
#include "ui_formviowindow.h"



FormVioWindow::FormVioWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVioWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(1000);

    formCvWindow = new FormCvWindow();
    connect(this, SIGNAL(imageSignals(QImage,int)), formCvWindow, SLOT(imageSlot(QImage,int)));  
    connect(formCvWindow, SIGNAL(visionImageSignals(QPixmap)), this, SLOT(visionImageSlot(QPixmap))); 

    this->ui->cb_vision->setCheckState(Qt::CheckState::Unchecked);
    connect(this->ui->cb_vision,SIGNAL(stateChanged(int)), this, SLOT(cbVisionStateChangedSlot(int))); 

    ctrlProcess = new CtrlProcess(this);
    ctrlProcess->moveToThread(&ctrlProcessThread);

    connect(this, SIGNAL(ctrlCamStatusSignal(unsigned char)), ctrlProcess, SLOT(ctrlCamStatusSlot(unsigned char)));
    connect(this, SIGNAL(ctrlCamSyncStatusSignal(unsigned char)), ctrlProcess, SLOT(ctrlCamSyncStatusSlot(unsigned char)));

    ctrlProcessThread.start();
}

FormVioWindow::~FormVioWindow()
{
    delete ui;
}

void FormVioWindow::closeEvent(QCloseEvent *event) 
{ 
    //this->vio->camStop();
    DBG("exit");
} 

void FormVioWindow::setQData(OPENVIO *vio)
{
    this->vio = vio;

    connect(vio->camProcess, SIGNAL(visionImageSignals(QPixmap)), this, SLOT(visionImageSlot(QPixmap))); 
    connect(vio, SIGNAL(getCameraStatusSignal()), this, SLOT(getCameraStatusSlot())); 

    this->setWindowTitle(this->vio->name +" : "+ this->vio->idShort);
    
    ui->lb_img->setScaledContents(true);

    ctrlProcess->setVio(NULL,vio);

    getCameraStatusSlot();

}

static bool isDirExist(QString fullPath)
{
    QFileInfo fileInfo(fullPath);
    if(fileInfo.isDir())
    {
      return true;
    }
    return false;
}

void FormVioWindow::cbVisionStateChangedSlot(int)
{
    vio->camProcess->setShowFlag(this->ui->cb_vision->checkState());
}

// void FormVioWindow::imuSlot(int index)
// {
//     static short acc[3],gyro[3],temp;
//     static T_float_angle Att_Angle;
//     static uint32_t timer;
//     static uint32_t t1,t1_old;
//     static uint16_t t2,t2_old;
//     static bool is_first = true;
//     static float d_time = 0;


//     uint8_t *imu_data = this->vio->img.imu[index];

//     this->vio->imu_hz++;

//     t1 = (uint32_t)(imu_data[0]<<24);
//     t1 |= (uint32_t)(imu_data[1]<<16);
//     t1 |= (uint32_t)(imu_data[2]<<8);
//     t1 |= (uint32_t)(imu_data[3]<<0);

//     t2 = (uint16_t)(imu_data[4]<<8);
//     t2 |= (uint16_t)(imu_data[5]<<0);

//     if(is_first == true)
//     {
//         is_first = false;
//         t1_old = t1;
//         t2_old = t2;
//         return;
//     }

//     if(t2 > t2_old)
//     {
//         timer = t2-t2_old;
//     }else{
//         timer = (uint32_t)t2 + 50000 - t2_old;
//     }

//     t1_old = t1;
//     t2_old = t2;

//     d_time = timer*0.00001;
//     //DBG("imu %d\t%d\t%d\t%f",t1,t2,timer,d_time);


//     // imu->recvData(imu_data+6,&Att_Angle,d_time);
//     // glview->setAngle(Att_Angle.rol,Att_Angle.pit,Att_Angle.yaw);
            
//     // acc[0] = (short)((imu_data[0+6]<<8)|imu_data[1+6]);
//     // acc[1] = (short)((imu_data[2+6]<<8)|imu_data[3+6]);
//     // acc[2] = (short)((imu_data[4+6]<<8)|imu_data[5+6]);
    
//     // gyro[0] = (short)((imu_data[6+6]<<8)|imu_data[7+6]);
//     // gyro[1] = (short)((imu_data[8+6]<<8)|imu_data[9+6]);
//     // gyro[2] = (short)((imu_data[10+6]<<8)|imu_data[11+6]);
    
//     // //DBG("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d",acc[0],acc[1],acc[2],gyro[0],gyro[1],gyro[2]);

//     // //DBG("%d",gyro[2]);

//     // ui->lb_acc_x->setText(QString::number(acc[0]));
//     // ui->lb_acc_y->setText(QString::number(acc[1]));
//     // ui->lb_acc_z->setText(QString::number(acc[2]));
    
//     // ui->lb_gyr_x->setText(QString::number(gyro[0]));
//     // ui->lb_gyr_y->setText(QString::number(gyro[1]));
//     // ui->lb_gyr_z->setText(QString::number(gyro[2]));

//     // ui->lb_att_rol->setText(QString::number(Att_Angle.rol,10,4));
//     // ui->lb_att_pit->setText(QString::number(Att_Angle.pit,10,4));
//     // ui->lb_att_yaw->setText(QString::number(Att_Angle.yaw,10,4));
    
//     // ui->lb_temp->setText(QString::number(temp));
    
// }

void FormVioWindow::on_pb_vision_clicked()
{
    if(!formCvWindow->isActiveWindow())
    {
        formCvWindow->show();
    }
}

void FormVioWindow::on_pb_cam_clicked()
{
    if(ui->pb_cam->text().contains("cam start"))
    {
        emit ctrlCamStatusSignal(1);
    }else{
        emit ctrlCamStatusSignal(0);
    }
}

void FormVioWindow::on_pb_sync_clicked()
{
    DBG("sync start");
    if(ui->pb_sync->text().contains("sync start"))
    {
        emit ctrlCamSyncStatusSignal(1);
    }else{
        emit ctrlCamSyncStatusSignal(0);
    }
}

void FormVioWindow::getCameraStatusSlot(void)
{
    if(vio->camStatus != 0)
    {
        ui->pb_cam->setText("cam stop ");
    }else{
        ui->pb_cam->setText("cam start");
    }

    if(vio->is_sync_start != 0)
    {
        ui->pb_sync->setText("sync stop ");
    }else{
        ui->pb_sync->setText("sync start");
    }
}

void FormVioWindow::visionImageSlot(QPixmap qImage)
{
    //if(this->ui->cb_vision->checkState() != Qt::CheckState::Unchecked)
    {
        fps_1s++;
        ui->lb_img->setPixmap(qImage);
    }
}

void FormVioWindow::onTimeOut()
{
    QString speedStr = QString::number(fps_1s);
    fps_1s = 0;
    speedStr += "fps";
    ui->lb_fps->setText(speedStr);
}
