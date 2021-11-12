#include "formcalibrwindow.h"
#include "ui_formcalibrwindow.h"

#include <QFileDialog>

FormCalibrWindow::FormCalibrWindow(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::FormCalibrWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("calibration");

    calibrProcess = new CalibrProcess(this);
    calibrProcess->moveToThread(&calibrProcessThread);
    calibrProcessThread.start();
    connect(this, SIGNAL(startCalibrSignal(QString)), calibrProcess, SLOT(startSlot(QString)));
    connect(&(calibrProcess->calibr), SIGNAL(logSignal(QString)), this, SLOT(logSlot(QString)));   
    connect(calibrProcess, SIGNAL(processSignal(int)), this, SLOT(processSlot(int)));   
}

FormCalibrWindow::~FormCalibrWindow()
{
    calibrProcessThread.quit();
    calibrProcessThread.wait();
    DBG("calibrProcessThread exit");
    delete ui;
}

void FormCalibrWindow::showEvent(QShowEvent *event)
{
    ui->le_floder_path->setText(setting->getCalibrPath());
}

void FormCalibrWindow::on_pb_select_floder_clicked()
{
    QString path = setting->getCalibrPath();
    if (path.length() == 0)
    {
        path = setting->getImagePath();
        if (path.length() == 0)
        {
            path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        }
    }

    QString srcDirPath = QFileDialog::getExistingDirectory(
        this, "choose calibr image directory",
        path);

    if (srcDirPath.isEmpty())
    {
        return;
    }
    else
    {
        ui->le_floder_path->setText(srcDirPath);
        setting->setCalibrPath(srcDirPath);
    }
}

void FormCalibrWindow::on_pb_calibration_clicked()
{
    setting->setCalibrPath(ui->le_floder_path->text());
    emit startCalibrSignal(ui->le_floder_path->text());
}

void FormCalibrWindow::logSlot(QString msg)
{
    this->ui->tb_log->append(msg);
    this->ui->tb_log->moveCursor(QTextCursor::End);
}

void FormCalibrWindow::processSlot(int code)
{
    if(code == PROCESS_CODE_START)
    {
        this->ui->pb_calibration->setEnabled(false);
    }else if(code == PROCESS_CODE_END){
        this->ui->pb_calibration->setEnabled(true);
    }
    
}