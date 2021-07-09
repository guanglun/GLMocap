#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "workspace.h"
#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include "winusbdriver.h"
#include "ui_mainwindow.h"
#include "imu.h"

#include "glview.h"
#include "setting.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    
    
    GLView *glview;
    IMU *imu;
    WinUSBDriver *qwinusb;
    QTimer *timer;
    Ui::MainWindow *ui;
    QLabel *imuStatusLabel;
    
private slots:
    void camSlot(int index);
    void imuSlot(int index);


    void disconnectSlot(void);
    void on_pb_open_clicked();
    void on_pb_send_clicked();
    void onTimeOut();    
    void on_pb_cam_start_clicked();
    void on_pb_cam_stop_clicked();
    void on_pb_imu_start_clicked();
    void on_pb_imu_stop_clicked();
    void on_pb_imu_calibration_clicked();
    
    void sendStatusSlot(int);
    void on_action_config_cam_triggered();
    void on_pb_img_cap_clicked();
};

#endif // MAINWINDOW_H
