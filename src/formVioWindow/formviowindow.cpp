#include "formviowindow.h"
#include "ui_formviowindow.h"



FormVioWindow::FormVioWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVioWindow)
{
    ui->setupUi(this);

    formCvWindow = new FormCvWindow();
    connect(this, SIGNAL(imageSignals(QImage,int)), formCvWindow, SLOT(imageSlot(QImage,int)));  
    connect(formCvWindow, SIGNAL(visionImageSignals(QImage)), this, SLOT(visionImageSlot(QImage))); 

    this->ui->cb_vision->setCheckState(Qt::CheckState::Unchecked);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(1000);

}

FormVioWindow::~FormVioWindow()
{
    delete ui;
}

void FormVioWindow::closeEvent(QCloseEvent *event) 
{ 
    this->vio->camStop();
    DBG("exit");
} 

void FormVioWindow::setQData(OPENVIO *vio)
{
    this->vio = vio;
    if(QString::compare(this->vio->name,"camera0") == 0)
    {
        formCvWindow->index = 0;
    }else if(QString::compare(this->vio->name,"camera1") == 0)
    {
        formCvWindow->index = 1;
    }
        
    connect(this->vio,SIGNAL(camSignals(int)),this,SLOT(camSlot(int)));
    connect(this->vio,SIGNAL(imuSignals(int)),this,SLOT(imuSlot(int)));

    this->setWindowTitle(this->vio->name +" : "+ this->vio->idStr);
    
    ui->lb_img->setScaledContents(true);
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

void FormVioWindow::camSlot(int index)
{
    static uint32_t timer;
    static uint32_t t1,t1_old;
    static uint16_t t2,t2_old;
    static bool is_first = true;
    static float d_time = 0;

    QImage myImage;
    QPixmap pixImage;
    QDateTime timeNow;

    t1 = (uint32_t)(this->vio->img.time[index][0]<<24);
    t1 |= (uint32_t)(this->vio->img.time[index][1]<<16);
    t1 |= (uint32_t)(this->vio->img.time[index][2]<<8);
    t1 |= (uint32_t)(this->vio->img.time[index][3]<<0);

    t2 = (uint16_t)(this->vio->img.time[index][4]<<8);
    t2 |= (uint16_t)(this->vio->img.time[index][5]<<0);

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

    if(this->vio->cam_id == OV7725_ID)
    {
        if(this->vio->pixformat == PIXFORMAT_GRAYSCALE)
        {
            for(int i=0;i<this->vio->img.size/2;i++)
            {
                this->vio->img.img_tmp[i] = this->vio->img.img[index][i*2];
            }
            myImage = QImage(this->vio->img.img_tmp,this->vio->img.width,this->vio->img.high,QImage::Format_Grayscale8);
            ui->lb_img->setPixmap(QPixmap::fromImage(myImage));
        }else if(this->vio->pixformat == PIXFORMAT_RGB565)
        {
            for(int i=0;i<this->vio->img.size/2;i++)
            {
                this->vio->img.img_tmp[i*2+1] = this->vio->img.img[index][i*2];
                this->vio->img.img_tmp[i*2] = this->vio->img.img[index][i*2+1];
            }
            myImage = QImage(this->vio->img.img_tmp,this->vio->img.width,this->vio->img.high,QImage::Format_RGB16);
            ui->lb_img->setPixmap(QPixmap::fromImage(myImage));
        }
    }else if(this->vio->cam_id == MT9V034_ID)
    {
        if(this->vio->pixformat == PIXFORMAT_GRAYSCALE)
        {
            myImage = QImage(this->vio->img.img[index],this->vio->img.width,this->vio->img.high,QImage::Format_Grayscale8);
            
            
            if(this->ui->cb_vision->checkState() == Qt::CheckState::Unchecked)
            {
                fps_1s++;
                emit imageSignals(myImage,this->ui->cb_vision->checkState());
                ui->lb_img->setPixmap(QPixmap::fromImage(myImage));
            }

            if(this->vio->isCapImage)
            {
                
                // timeNow =  QDateTime::currentDateTime();
                // QString current_date = timeNow.toString("yyyy_MM_dd_hh_mm_ss_zzz");
                QString file_name = this->vio->saveImagePath + "/" + this->vio->name + "_" + QString::number(this->vio->saveCount) + ".png";
                pixImage.save(file_name);
                //mlog->show(file_name);
                this->vio->saveCount++;
                this->vio->isCapImage = false;
            }
        }
    }else{
        return;
    }
    
    //QImage myImage = QImage(this->vio->img.img,this->vio->img.width,this->vio->img.high,QImage::Format_);
    
}

void FormVioWindow::imuSlot(int index)
{
    static short acc[3],gyro[3],temp;
    static T_float_angle Att_Angle;
    static uint32_t timer;
    static uint32_t t1,t1_old;
    static uint16_t t2,t2_old;
    static bool is_first = true;
    static float d_time = 0;


    uint8_t *imu_data = this->vio->img.imu[index];

    this->vio->imu_hz++;

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


    // imu->recvData(imu_data+6,&Att_Angle,d_time);
    // glview->setAngle(Att_Angle.rol,Att_Angle.pit,Att_Angle.yaw);
            
    // acc[0] = (short)((imu_data[0+6]<<8)|imu_data[1+6]);
    // acc[1] = (short)((imu_data[2+6]<<8)|imu_data[3+6]);
    // acc[2] = (short)((imu_data[4+6]<<8)|imu_data[5+6]);
    
    // gyro[0] = (short)((imu_data[6+6]<<8)|imu_data[7+6]);
    // gyro[1] = (short)((imu_data[8+6]<<8)|imu_data[9+6]);
    // gyro[2] = (short)((imu_data[10+6]<<8)|imu_data[11+6]);
    
    // //DBG("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d",acc[0],acc[1],acc[2],gyro[0],gyro[1],gyro[2]);

    // //DBG("%d",gyro[2]);

    // ui->lb_acc_x->setText(QString::number(acc[0]));
    // ui->lb_acc_y->setText(QString::number(acc[1]));
    // ui->lb_acc_z->setText(QString::number(acc[2]));
    
    // ui->lb_gyr_x->setText(QString::number(gyro[0]));
    // ui->lb_gyr_y->setText(QString::number(gyro[1]));
    // ui->lb_gyr_z->setText(QString::number(gyro[2]));

    // ui->lb_att_rol->setText(QString::number(Att_Angle.rol,10,4));
    // ui->lb_att_pit->setText(QString::number(Att_Angle.pit,10,4));
    // ui->lb_att_yaw->setText(QString::number(Att_Angle.yaw,10,4));
    
    // ui->lb_temp->setText(QString::number(temp));
    
}

void FormVioWindow::on_pb_vision_clicked()
{
    if(!formCvWindow->isActiveWindow())
    {
        formCvWindow->show();
    }
}

void FormVioWindow::visionImageSlot(QImage qImage)
{
    if(this->ui->cb_vision->checkState() != Qt::CheckState::Unchecked)
    {
        fps_1s++;
        ui->lb_img->setPixmap(QPixmap::fromImage(qImage));
    }
}

void FormVioWindow::onTimeOut()
{
    QString speedStr = QString::number(fps_1s);
    fps_1s = 0;
    speedStr += "fps";
    ui->lb_fps->setText(speedStr);
}
