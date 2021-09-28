#ifndef FORMCALIBRWINDOW_H
#define FORMCALIBRWINDOW_H

#include <QWidget>
#include "workspace.h"

#include "CalibrProcess.h"

namespace Ui {
class FormCalibrWindow;
}

class FormCalibrWindow : public QWidget
{
    Q_OBJECT
    
private:
    QThread calibrProcessThread;
    CalibrProcess *calibrProcess;
public:

    explicit FormCalibrWindow(QWidget *parent = 0);
    ~FormCalibrWindow();

private slots:
    void on_pb_exit_clicked();
    void on_pb_select_floder_clicked();
    void on_pb_calibration_clicked();
private:
    Ui::FormCalibrWindow *ui;

protected:
    void showEvent(QShowEvent *event);

signals:
    void startCalibrSignal(QString path);
};

#endif // FORMCALIBRWINDOW_H
