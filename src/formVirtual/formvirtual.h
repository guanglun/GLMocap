#ifndef FORMVIRTUAL_H
#define FORMVIRTUAL_H

#include <QWidget>
#include "workspace.h"
#include "winusbdriver.h"

namespace Ui {
class FormVirtual;
}

class FormVirtual : public QWidget
{
    Q_OBJECT
    
private:
    int index = 0,camVirtualNumber = 0;
    QStringList files;
    QString imageDirPath;
    WinUSBDriver *qwinusb;
    void load();
    QStringList checkFile(QString path,int *num);
    void initVirtualCamera();
    void init(QString srcDirPath);
public:
    void set(WinUSBDriver *qwinusb);
    explicit FormVirtual(QWidget *parent = 0);
    ~FormVirtual();

private slots:
    
    void on_pb_load_image_path_clicked();
    void on_pb_previous_clicked();
    void on_pb_load_clicked();
    void on_pb_next_clicked();
private:
    Ui::FormVirtual *ui;
protected:
    void showEvent(QShowEvent *event);
signals:

};

#endif // FORMVIRTUAL_H
