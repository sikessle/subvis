#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "view/mainwindow.h"

namespace SubVis {

using std::string;

MainWindow::MainWindow(DrawController &draw_controller,
                       IOController &io_ctrl,
                       PluginManager &plugin_mngr)
    : QMainWindow{0},
      ui{new Ui::MainWindow},
      io_controller(io_ctrl),
      plugin_manager(plugin_mngr)
{
    ui->setupUi(this);

    setup_status_bar();
    setup_viewer_tabs(draw_controller);
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

void MainWindow::create_plugin_guis()
{   
    for (const auto &it : plugin_manager.list_plugins()) {
        const auto &info = it.second;
        QWidget *plugin_container = new QWidget(this);
        ui->tabs_plugins->addTab(plugin_container, info.name);
        info.plugin->create_gui(plugin_container);
        QObject::connect(ui->tabs_plugins, SIGNAL(currentChanged(int)),
                         this, SLOT(plugin_tab_changed(int)));
    }
    plugin_tab_changed(0);
}

void MainWindow::plugin_tab_changed(int current)
{
    auto it = plugin_manager.list_plugins().begin();

    for (int i = 0; i < current; i++)
    {
        it++;
    }
    ui->tab_viewer_plugin->set_drawing_plugin(it->second.plugin.get());
    ui->tabs_viewer->setTabText(1, it->second.name);
}

void MainWindow::setup_toolbar()
{
    QObject::connect(ui->action_load, SIGNAL(triggered(bool)), this, SLOT(load_dialog()));
    QObject::connect(ui->action_save, SIGNAL(triggered(bool)), this, SLOT(save_dialog()));
    QObject::connect(ui->action_snapshot, SIGNAL(triggered(bool)),
                     ui->tab_viewer_mesh, SLOT(saveSnapshot(bool)));
}

void MainWindow::load_dialog()
{
    QString file_filter{QString::fromStdString(io_controller.load_file_formats())};
    QString fn{QFileDialog::getOpenFileName(this, kLoadDialogCaption,
                                            QDir::home().absolutePath(), file_filter)};

    if (!fn.isNull()) {
        const string filename = fn.toStdString();
        if (!io_controller.load_mesh(filename)) {
            QString msg = "Failed to load file " + fn;
            QMessageBox::warning(this, "Error", msg);
        }
    }
}

void MainWindow::save_dialog()
{
    QString file_filter{QString::fromStdString(io_controller.persist_file_formats())};
    QString fn{QFileDialog::getSaveFileName(this, kSaveDialogCaption,
                                            QDir::home().absolutePath(), file_filter)};

    if (!fn.isNull()) {
        const string filename = fn.toStdString();
        if (!io_controller.persist_mesh(filename)) {
            QString msg = "Failed to save to file " + fn;
            QMessageBox::warning(this, "Error", msg);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

} // namespace SubVis
