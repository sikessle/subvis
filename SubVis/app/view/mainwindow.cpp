#include "QLabel"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view/mesh_render_widget.h"
#include "view/plugin_render_widget.h"

namespace SubVis {
namespace View {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setup_status_bar();
    setup_render_tabs();
    setup_plugin_tabs();
}

void MainWindow::setup_status_bar()
{
    status_label = new QLabel(this);
    status_label->setText(STATUS_TEXT);
    ui->statusbar->addPermanentWidget(status_label);
    // remove border around label
    ui->statusbar->setStyleSheet("QStatusBar::item { border: 0; }");
}

void MainWindow::setup_render_tabs()
{
    mesh_render_widget = new MeshRenderWidget;
    ui->tabs_rendering->addTab(mesh_render_widget, TAB_RENDER_MESH_TEXT);
    plugin_render_widget = new PluginRenderWidget;
    ui->tabs_rendering->addTab(plugin_render_widget, TAB_RENDER_PLUGIN_TEXT);
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
