#ifndef FORMVIOWINDOW_H
#define FORMVIOWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 

#include "workspace.h"
#include "openvio.h"

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
    void setQData(OPENVIO *vio);
private slots:
    void camSlot(int index);
    void imuSlot(int index);

private:
    Ui::FormVioWindow *ui;
    
protected: 
     void closeEvent(QCloseEvent *event);     
};

#endif // 
