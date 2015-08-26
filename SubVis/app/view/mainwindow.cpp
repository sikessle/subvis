#include <QLabel>
#include <QFileDialog>
#include "ui_mainwindow.h"
#include "view/mainwindow.h"

namespace SubVis {

using std::string;

MainWindow::MainWindow(DrawController &draw_controller, IOController &io_ctrl) :
    QMainWindow{0}, ui{new Ui::MainWindow}, io_controller(io_ctrl)
{
    ui->setupUi(this);

    setup_status_bar();
    setup_viewer_tabs(draw_controller);
    setup_plugin_tabs();
    setup_toolbar();
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

void MainWindow::setup_toolbar()
{
    QObject::connect(ui->action_load, SIGNAL(triggered(bool)), this, SLOT(load_dialog()));
    QObject::connect(ui->action_save, SIGNAL(triggered(bool)), this, SLOT(save_dialog()));
}

void MainWindow::load_dialog()
{
    QString file_filter{QString::fromStdString(io_controller.load_file_formats())};
    QString fn{QFileDialog::getOpenFileName(this, kLoadDialogCaption,
                                            QDir::home().absolutePath(), file_filter)};

    if (!fn.isNull()) {
        string filename = fn.toStdString();
        io_controller.load_mesh(filename);
    }
}

void MainWindow::save_dialog()
{
    QString file_filter{QString::fromStdString(io_controller.persist_file_formats())};
    QString fn{QFileDialog::getSaveFileName(this, kSaveDialogCaption,
                                            QDir::home().absolutePath(), file_filter)};

    if (!fn.isNull()) {
        string filename = fn.toStdString();
        io_controller.persist_mesh(filename);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

} // namespace SubVis
