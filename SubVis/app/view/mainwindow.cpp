#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "view/mainwindow.h"

namespace subvis {

MainWindow::MainWindow(MeshData& mesh_data,
                       const std::map<const QString, PluginWrapper>& plugins)
  : QMainWindow{0},
    ui_{new Ui::MainWindow},
    mesh_data_(mesh_data),
    plugins_(plugins) {

  ui_->setupUi(this);

  setup_status_bar();
  setup_viewer_tabs();
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

void MainWindow::setup_viewer_tabs() {
  ui_->tab_viewer_mesh->set_model(mesh_data_);
  ui_->tab_viewer_plugin->set_model(mesh_data_);
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
                   SLOT(show_load_dialog()));
  QObject::connect(ui_->action_save, SIGNAL(triggered(bool)), this,
                   SLOT(show_save_dialog()));
  QObject::connect(ui_->action_snapshot, SIGNAL(triggered(bool)),
                   ui_->tab_viewer_mesh, SLOT(saveSnapshot(bool)));
  QObject::connect(ui_->action_triangulate, SIGNAL(triggered(bool)),
                   this, SLOT(triangulate_mesh(void)));
}

void MainWindow::triangulate_mesh() {
  mesh_data_.triangulate();
}

void MainWindow::show_load_dialog() {
  QString file_filter{QString::fromStdString(mesh_data_.get_load_file_formats())};
  QString fn{QFileDialog::getOpenFileName(this, kLoadDialogCaption,
                                          QDir::home().absolutePath(), file_filter)};

  if (!fn.isNull()) {
    const std::string filename = fn.toStdString();
    if (!mesh_data_.load(filename)) {
      QString msg = "Failed to load file " + fn;
      QMessageBox::warning(this, "Error", msg);
    }
  }
}

void MainWindow::show_save_dialog() {
  QString file_filter{QString::fromStdString(mesh_data_.get_persist_file_formats())};
  QString fn{QFileDialog::getSaveFileName(this, kSaveDialogCaption,
                                          QDir::home().absolutePath(), file_filter)};

  if (!fn.isNull()) {
    const std::string filename = fn.toStdString();
    if (!mesh_data_.persist(filename)) {
      QString msg = "Failed to save to file " + fn;
      QMessageBox::warning(this, "Error", msg);
    }
  }
}

MainWindow::~MainWindow() {
  delete ui_;
}

} // namespace subvis
