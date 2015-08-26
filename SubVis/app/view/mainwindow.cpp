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
    ui->tab_viewer_mesh->set_draw_controller(&draw_controller);
    ui->tab_viewer_plugin->set_draw_controller(&draw_controller);

    QObject::connect(this, SIGNAL(mesh_updated()),
                     ui->tab_viewer_mesh, SLOT(enforce_redraw()));
    QObject::connect(this, SIGNAL(mesh_updated()),
                     ui->tab_viewer_plugin, SLOT(enforce_redraw()));
}

void MainWindow::setup_plugin_tabs()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

} // namespace SubVis
