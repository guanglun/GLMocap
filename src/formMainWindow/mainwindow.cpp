#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"
#include "formcamconfig.h"

QLabel *status_msg,*status_speed;
Log *mlog;
bool isCapImage = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    setting = new Setting();
    
    glview = new GLView(this);
    ui->verticalLayout->addWidget(glview,0,0);
    
    this->setWindowTitle("OPENVIO上位机");
    
    qwinusb = new WinUSBDriver();
    
    connect(qwinusb,SIGNAL(sendStatusSignals(int)),this,SLOT(sendStatusSlot(int)));
    connect(qwinusb,SIGNAL(camSignals(int)),this,SLOT(camSlot(int)));
    connect(qwinusb,SIGNAL(imuSignals(int)),this,SLOT(imuSlot(int)));
    connect(qwinusb,SIGNAL(disconnectSignals()),this,SLOT(disconnectSlot()));

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    timer->start(1000);
    
    imu = new IMU();
    
    //状态栏
    status_msg = new QLabel(this);
    ui->statusBar->addWidget(status_msg);
    status_msg->setText("OPENVIO");

    status_msg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    status_speed = new QLabel(this);
    ui->statusBar->addWidget(status_speed);
    status_speed->setText("0");
    
    status_speed->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    mlog = new Log();
    mlog->show("OPENVIO已启动");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::camSlot(int index)
{
    static uint32_t timer;
    static uint32_t t1,t1_old;
    static uint16_t t2,t2_old;
    static bool is_first = true;
    static float d_time = 0;

    QImage myImage;
    QPixmap pixImage;
    QDateTime timeNow;

    t1 = (uint32_t)(qwinusb->img.time[index][0]<<24);
    t1 |= (uint32_t)(qwinusb->img.time[index][1]<<16);
    t1 |= (uint32_t)(qwinusb->img.time[index][2]<<8);
    t1 |= (uint32_t)(qwinusb->img.time[index][3]<<0);

    t2 = (uint16_t)(qwinusb->img.time[index][4]<<8);
    t2 |= (uint16_t)(qwinusb->img.time[index][5]<<0);

    if(is_first == true)
    {
        is_first = false;
        t1_old = t1;
        t2_old = t2;
        return;
    }

    if(t2 > t2_old)
    {
        timer = t2-t2_old;
    }else{
        timer = (uint32_t)t2 + 50000 - t2_old;
    }

    t1_old = t1;
    t2_old = t2;

    d_time = timer*0.00001;
    //DBG("cam %d\t%d\t%d\t%f",t1,t2,timer,d_time);


    if(qwinusb->cam_id == OV7725_ID)
    {
        if(qwinusb->pixformat == PIXFORMAT_GRAYSCALE)
        {
            for(int i=0;i<qwinusb->img.size/2;i++)
            {
                qwinusb->img.img_tmp[i] = qwinusb->img.img[index][i*2];
            }
            myImage = QImage(qwinusb->img.img_tmp,qwinusb->img.width,qwinusb->img.high,QImage::Format_Grayscale8);
            ui->lb_img->setPixmap(QPixmap::fromImage(myImage));
        }else if(qwinusb->pixformat == PIXFORMAT_RGB565)
        {
            for(int i=0;i<qwinusb->img.size/2;i++)
            {
                qwinusb->img.img_tmp[i*2+1] = qwinusb->img.img[index][i*2];
                qwinusb->img.img_tmp[i*2] = qwinusb->img.img[index][i*2+1];
            }
            myImage = QImage(qwinusb->img.img_tmp,qwinusb->img.width,qwinusb->img.high,QImage::Format_RGB16);
            ui->lb_img->setPixmap(QPixmap::fromImage(myImage));
        }
    }else if(qwinusb->cam_id == MT9V034_ID)
    {
        if(qwinusb->pixformat == PIXFORMAT_GRAYSCALE)
        {
            myImage = QImage(qwinusb->img.img[index],qwinusb->img.width,qwinusb->img.high,QImage::Format_Grayscale8);
            pixImage = QPixmap::fromImage(myImage);
            ui->lb_img->setPixmap(pixImage);
            if(isCapImage)
            {
                timeNow =  QDateTime::currentDateTime();
                QString current_date = timeNow.toString("yyyy_MM_dd_hh_mm_ss_zzz");
                QString file_name = "image/"+current_date+".png";
                pixImage.save(file_name);

                isCapImage = false;
            }
        }
    }else{
        return;
    }
    
    //QImage myImage = QImage(qwinusb->img.img,qwinusb->img.width,qwinusb->img.high,QImage::Format_);
    
}

void MainWindow::on_pb_open_clicked()
{
    if(ui->pb_open->text().compare(QString::fromUtf8("打开")) == 0)
    {
        ui->pb_open->setEnabled(false);

        int vid = ui->le_vid->text().toInt();
        int pid = ui->le_pid->text().toInt();
        qwinusb->open(vid,pid);
    }else
    {
        ui->pb_open->setEnabled(false);

        qwinusb->close();
    }   
}
void MainWindow::disconnectSlot(void)
{
    qwinusb->close();
    ui->pb_open->setText("打开");
}

void MainWindow::sendStatusSlot(int msg)
{
    switch(msg)
    {
    case USB_MSG_OPEN_SUCCESS:
        ui->pb_open->setEnabled(true);

        ui->pb_open->setText("关闭");        
        break;
    case USB_MSG_CLOSE_SUCCESS:
    case USB_MSG_OPEN_FAIL:
        ui->pb_open->setEnabled(true);

        ui->pb_open->setText("打开");        
        break;
    default:
        break;
    }
}

void MainWindow::imuSlot(int index)
{
    static short acc[3],gyro[3],temp;
    static T_float_angle Att_Angle;
    static uint32_t timer;
    static uint32_t t1,t1_old;
    static uint16_t t2,t2_old;
    static bool is_first = true;
    static float d_time = 0;


    uint8_t *imu_data = qwinusb->img.imu[index];

    qwinusb->imu_hz++;

    t1 = (uint32_t)(imu_data[0]<<24);
    t1 |= (uint32_t)(imu_data[1]<<16);
    t1 |= (uint32_t)(imu_data[2]<<8);
    t1 |= (uint32_t)(imu_data[3]<<0);

    t2 = (uint16_t)(imu_data[4]<<8);
    t2 |= (uint16_t)(imu_data[5]<<0);

    if(is_first == true)
    {
        is_first = false;
        t1_old = t1;
        t2_old = t2;
        return;
    }

    if(t2 > t2_old)
    {
        timer = t2-t2_old;
    }else{
        timer = (uint32_t)t2 + 50000 - t2_old;
    }

    t1_old = t1;
    t2_old = t2;

    d_time = timer*0.00001;
    //DBG("imu %d\t%d\t%d\t%f",t1,t2,timer,d_time);


    imu->recvData(imu_data+6,&Att_Angle,d_time);
    glview->setAngle(Att_Angle.rol,Att_Angle.pit,Att_Angle.yaw);
            
    acc[0] = (short)((imu_data[0+6]<<8)|imu_data[1+6]);
    acc[1] = (short)((imu_data[2+6]<<8)|imu_data[3+6]);
    acc[2] = (short)((imu_data[4+6]<<8)|imu_data[5+6]);
    
    gyro[0] = (short)((imu_data[6+6]<<8)|imu_data[7+6]);
    gyro[1] = (short)((imu_data[8+6]<<8)|imu_data[9+6]);
    gyro[2] = (short)((imu_data[10+6]<<8)|imu_data[11+6]);
    
    //DBG("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d",acc[0],acc[1],acc[2],gyro[0],gyro[1],gyro[2]);

    //DBG("%d",gyro[2]);

    ui->lb_acc_x->setText(QString::number(acc[0]));
    ui->lb_acc_y->setText(QString::number(acc[1]));
    ui->lb_acc_z->setText(QString::number(acc[2]));
    
    ui->lb_gyr_x->setText(QString::number(gyro[0]));
    ui->lb_gyr_y->setText(QString::number(gyro[1]));
    ui->lb_gyr_z->setText(QString::number(gyro[2]));

    ui->lb_att_rol->setText(QString::number(Att_Angle.rol,10,4));
    ui->lb_att_pit->setText(QString::number(Att_Angle.pit,10,4));
    ui->lb_att_yaw->setText(QString::number(Att_Angle.yaw,10,4));
    
    ui->lb_temp->setText(QString::number(temp));
    
}

void MainWindow::on_pb_send_clicked()
{
    //QByteArray bytes = ui->le_send->text().toUtf8();
    //winusb.send(bytes);
    
    //winusb.ctrl();
}

void MainWindow::onTimeOut()
{
    ui->lb_imu_hz->setText(QString::number(qwinusb->imu_hz)+"Hz");
    qwinusb->imu_hz = 0;

    QString str;
    if(qwinusb->recv_count_1s < 1024)
    {
        str = QString::number(qwinusb->recv_count_1s);
        str += "B/s";
    }else if(qwinusb->recv_count_1s < 1024*1024)
    {
        str = QString::number(qwinusb->recv_count_1s/1024);
        str += "KB/s";
    }else if(qwinusb->recv_count_1s < 1024*1024*1024)
    {
        str = QString::number(qwinusb->recv_count_1s/1024/1024);
        str += "MB/s";
    }
    
    str += " ";
    str += QString::number(qwinusb->frame_fps);
    str += "fps";
    qwinusb->frame_fps = 0;
    
    qwinusb->recv_count_1s = 0;
    status_speed->setText(str);
    

}

void MainWindow::on_pb_cam_start_clicked()
{
    qwinusb->ctrlCamStart();
}

void MainWindow::on_pb_cam_stop_clicked()
{
    qwinusb->ctrlCamStop();
}

void MainWindow::on_pb_imu_start_clicked()
{
    qwinusb->ctrlIMUStart();
}

void MainWindow::on_pb_imu_stop_clicked()
{
    qwinusb->ctrlIMUStop();
}

void MainWindow::on_pb_imu_calibration_clicked()
{
    imu->startCalibration();
}

void MainWindow::on_action_config_cam_triggered()
{
    FormCamConfig *formCamConfig = new FormCamConfig();
    formCamConfig->setQData(qwinusb);
    formCamConfig->show();
}

void MainWindow::on_pb_img_cap_clicked()
{
    isCapImage = true;
}
