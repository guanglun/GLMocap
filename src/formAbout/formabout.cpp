#include "formabout.h"
#include "ui_formabout.h"
#include <QFileDialog>

FormAbout::FormAbout(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::FormAbout)
{
    ui->setupUi(this);

    this->setWindowTitle("about");
}

FormAbout::~FormAbout()
{
    delete ui;
}
