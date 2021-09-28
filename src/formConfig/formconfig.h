#ifndef FORMCONFIG_H
#define FORMCONFIG_H

#include <QWidget>
#include "workspace.h"


namespace Ui {
class FormConfig;
}

class FormConfig : public QWidget
{
    Q_OBJECT
    
private:
    
public:

    explicit FormConfig(QWidget *parent = 0);
    ~FormConfig();

private slots:
    void on_pb_exit_clicked();
    void on_pb_set_image_save_path_clicked();
    void on_pb_set_threshold_clicked();
    void on_pb_save_num_cam_clicked();
private:
    Ui::FormConfig *ui;
protected:
    void showEvent(QShowEvent *event);
signals:

};

#endif // FORMCONFIG_H
