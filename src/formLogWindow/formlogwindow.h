#ifndef FORMLOGWINDOW_H
#define FORMLOGWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>

#include "workspace.h"

namespace Ui {
class FormLogWindow;
}

class FormLogWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormLogWindow(QWidget *parent = 0);
    ~FormLogWindow();

private slots:
    void logSlot(QString msg); 
signals:

private:
    Ui::FormLogWindow *ui;
    
protected: 

};

#endif // 
