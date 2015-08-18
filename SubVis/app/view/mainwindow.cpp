#include <QLabel>
#include "ui_mainwindow.h"
#include "view/mainwindow.h"
#include "view/viewer_mesh_widget.h"
#include "view/viewer_plugin_widget.h"

namespace SubVis {
namespace View {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setup_status_bar();
    setup_viewer_tabs();
    setup_plugin_tabs();
}

void MainWindow::setup_status_bar()
{
    status_label = new QLabel;
    status_label->setText(STATUS_TEXT);
    ui->statusbar->addPermanentWidget(status_label);
    // remove border around label
    ui->statusbar->setStyleSheet("QStatusBar::item { border: 0; }");
}

void MainWindow::setup_viewer_tabs()
{
    viewer_mesh_widget = new ViewerMeshWidget(ui->tabs_viewer);
    ui->tabs_viewer->addTab(viewer_mesh_widget, TAB_VIEWER_MESH_TEXT);

    viewer_plugin_widget = new ViewerPluginWidget(ui->tabs_viewer);
    ui->tabs_viewer->addTab(viewer_plugin_widget, TAB_VIEWER_PLUGIN_TEXT);
}

void MainWindow::setup_plugin_tabs()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

} // namespace View
} // namespace SubVis
