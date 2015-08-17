#include <QLabel>
#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace SubVis {
namespace View {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setup_status_bar();
}

void MainWindow::setup_status_bar()
{
    status_label = new QLabel(this);
    status_label->setText(status_text);
    // remove grey border around label
    ui->statusbar->setStyleSheet("QStatusBar::item { border: 0; }");
    ui->statusbar->addPermanentWidget(status_label);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete status_label;
}

} // namespace View
} // namespace SubVis
