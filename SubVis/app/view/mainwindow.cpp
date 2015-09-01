#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "view/mainwindow.h"

namespace subvis {

MainWindow::MainWindow(DrawController& draw_controller,
                       IOController& io_controller,
                       const std::map<QString, PluginWrapper>& plugins)
  : QMainWindow{0},
    ui_{new Ui::MainWindow},
io_controller_(io_controller),
plugins_(plugins) {
  ui_->setupUi(this);

  setup_status_bar();
  setup_viewer_tabs(draw_controller);
  setup_toolbar();
  setup_plugin_guis();
}

void MainWindow::setup_status_bar() {
  status_label_ = new QLabel;
  status_label_->setText(kStatusText);
  ui_->statusbar->addPermanentWidget(status_label_);
  // remove border around label
  ui_->statusbar->setStyleSheet("QStatusBar::item { border: 0; }");
}

void MainWindow::setup_viewer_tabs(DrawController& draw_controller) {
  ui_->tab_viewer_mesh->set_draw_controller(&draw_controller);
  ui_->tab_viewer_plugin->set_draw_controller(&draw_controller);

  QObject::connect(this, SIGNAL(mesh_updated()),
                   ui_->tab_viewer_mesh, SLOT(enforce_redraw()));
  QObject::connect(this, SIGNAL(mesh_updated()),
                   ui_->tab_viewer_plugin, SLOT(enforce_redraw()));
}

void MainWindow::setup_plugin_guis() {
  for (const auto& it : plugins_) {
    const auto& wrapper = it.second;
    QWidget* plugin_container = new QWidget(this);
    ui_->tabs_plugins->addTab(plugin_container, wrapper.name);
    wrapper.plugin->create_gui(plugin_container);
    QObject::connect(ui_->tabs_plugins, SIGNAL(currentChanged(int)),
                     this, SLOT(plugin_tab_changed(int)));
  }

  if (plugins_.size() > 0) {
    plugin_tab_changed(0);
  }
}

void MainWindow::plugin_tab_changed(int current) {
  auto it = plugins_.begin();

  for (int i = 0; i < current; i++) {
    it++;
  }
  ui_->tab_viewer_plugin->set_drawing_plugin(it->second.plugin.get());
  ui_->tabs_viewer->setTabText(1, it->second.name);
}

void MainWindow::setup_toolbar() {
  QObject::connect(ui_->action_load, SIGNAL(triggered(bool)), this,
                   SLOT(load_dialog()));
  QObject::connect(ui_->action_save, SIGNAL(triggered(bool)), this,
                   SLOT(save_dialog()));
  QObject::connect(ui_->action_snapshot, SIGNAL(triggered(bool)),
                   ui_->tab_viewer_mesh, SLOT(saveSnapshot(bool)));
}

void MainWindow::load_dialog() {
  QString file_filter{QString::fromStdString(io_controller_.load_file_formats())};
  QString fn{QFileDialog::getOpenFileName(this, kLoadDialogCaption,
                                          QDir::home().absolutePath(), file_filter)};

  if (!fn.isNull()) {
    const std::string filename = fn.toStdString();
    if (!io_controller_.load_mesh(filename)) {
      QString msg = "Failed to load file " + fn;
      QMessageBox::warning(this, "Error", msg);
    }
  }
}

void MainWindow::save_dialog() {
  QString file_filter{QString::fromStdString(io_controller_.persist_file_formats())};
  QString fn{QFileDialog::getSaveFileName(this, kSaveDialogCaption,
                                          QDir::home().absolutePath(), file_filter)};

  if (!fn.isNull()) {
    const std::string filename = fn.toStdString();
    if (!io_controller_.persist_mesh(filename)) {
      QString msg = "Failed to save to file " + fn;
      QMessageBox::warning(this, "Error", msg);
    }
  }
}

MainWindow::~MainWindow() {
  delete ui_;
}

} // namespace subvis
