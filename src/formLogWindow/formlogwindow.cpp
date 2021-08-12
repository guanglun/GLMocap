#include "formlogwindow.h"
#include "ui_formlogwindow.h"

FormLogWindow::FormLogWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLogWindow)
{
    ui->setupUi(this);
}

FormLogWindow::~FormLogWindow()
{
    delete ui;
}

void FormLogWindow::logSlot(QString msg)
{
    this->ui->tb_log->append(msg);
    this->ui->tb_log->moveCursor(QTextCursor::End);
}