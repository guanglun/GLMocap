#ifndef FORMPX4WINDOW_H
#define FORMPX4WINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>

#include "workspace.h"
#include "PX4Thread.h"

namespace Ui {
class FormPx4Window;
}

class FormPx4Window : public QWidget
{
    Q_OBJECT
    PX4Thread *px4;
private:
    void updatePosText();
public:
    explicit FormPx4Window(QWidget *parent = 0);
    ~FormPx4Window();
    void setPX4Thread(PX4Thread *px4);
private slots:
    void on_pb_ini_mavlink_clicked();
    void on_pb_disarm_clicked();
    void on_pb_arm_clicked();
    void on_pb_take_off_clicked();
    void on_pb_land_clicked();
    void on_pb_set_position_clicked();

signals:

private:
    Ui::FormPx4Window *ui;
    
protected: 

};

#endif // 
