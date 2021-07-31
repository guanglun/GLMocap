#ifndef FORMVIOWINDOW_H
#define FORMVIOWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>

#include "workspace.h"
#include "openvio.h"

#include "formcvwindow.h"

namespace Ui {
class FormVioWindow;
}

class FormVioWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormVioWindow(QWidget *parent = 0);
    ~FormVioWindow();
    OPENVIO *vio;
    FormCvWindow *formCvWindow;
    
    void setQData(OPENVIO *vio);

private slots:
    void camSlot(int index);
    void imuSlot(int index);

signals:
    void imageSignals(QImage qImage);

private:
    Ui::FormVioWindow *ui;
    
protected: 
     void closeEvent(QCloseEvent *event);     
};

#endif // 
