#include <QLabel>
#include "ui_mainwindow.h"
#include "view/mainwindow.h"

namespace SubVis {

MainWindow::MainWindow(DrawController &draw_controller) :
    QMainWindow{0}, ui{new Ui::MainWindow}
{
    ui->setupUi(this);

    setup_status_bar();
    setup_viewer_tabs(draw_controller);
    setup_plugin_tabs();
}

void MainWindow::setup_status_bar()
{
    status_label = new QLabel;
    status_label->setText(kStatusText);
    ui->statusbar->addPermanentWidget(status_label);
    // remove border around label
    ui->statusbar->setStyleSheet("QStatusBar::item { border: 0; }");
}

void MainWindow::setup_viewer_tabs(DrawController &draw_controller)
{
    // TODO: MOVE THAT TO THE DESIGNER!!!!!!!!!!!!!
    viewer_mesh_widget = new ViewerMeshWidget{ui->tabs_viewer, draw_controller};
    ui->tabs_viewer->addTab(viewer_mesh_widget, kTabViewerMeshText);

    viewer_plugin_widget = new ViewerPluginWidget{ui->tabs_viewer, draw_controller};
    ui->tabs_viewer->addTab(viewer_plugin_widget, kTabViewerPluginText);
}

void MainWindow::setup_plugin_tabs()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

} // namespace SubVis
