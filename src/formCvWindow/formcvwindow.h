#ifndef FORMCVWINDOW_H
#define FORMCVWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>

#include "workspace.h"
#include "openvio.h"

namespace Ui {
class FormCvWindow;
}

class FormCvWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormCvWindow(QWidget *parent = 0);
    ~FormCvWindow();


private slots:

    void imageSlot(QImage qImage);
private:
    Ui::FormCvWindow *ui;
    
protected: 
 
};

#endif // 
