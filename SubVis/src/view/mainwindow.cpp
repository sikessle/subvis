#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace SubVis {
namespace View {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

} // namespace View
} // namespace SubVis
