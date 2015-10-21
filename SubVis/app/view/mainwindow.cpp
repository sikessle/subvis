#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>

#include "ui_mainwindow.h"
#include "view/mainwindow.h"

namespace subvis {

MainWindow::MainWindow(MeshData& mesh_data, PluginManager& plugin_manager)
  : QMainWindow{0},
    ui_{new Ui::MainWindow},
mesh_data_(mesh_data),
plugin_manager_(plugin_manager) {

  ui_->setupUi(this);

  setup_status_bar();
  setup_viewer_tabs();
  setup_menus();
  setup_plugin_guis();

  QObject::connect(&mesh_data_,
                   SIGNAL(updated(
                            std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>)),
                   this,
                   SLOT(mesh_updated(
                          std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>)));
}

void MainWindow::mesh_updated(
  std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>
  meshes) {
  QString info = "[0]: ";
  info += get_stats(meshes.first);
  info += " ---- [1]: ";
  info += get_stats(meshes.second);

  mesh_information_label_->setText(info);

  ui_->action_undo->setEnabled(mesh_data_.history_can_step_back());
  ui_->action_redo->setEnabled(mesh_data_.history_can_step_forward());
}

QString MainWindow::get_stats(const surface_mesh::Surface_mesh& mesh) {
  QString info;

  info += "Vertices: ";
  info += QString::number(mesh.n_vertices());
  info += " | Edges: ";
  info += QString::number(mesh.n_edges());
  info += " | Faces: ";
  info += QString::number(mesh.n_faces());

  return info;
}

void MainWindow::setup_status_bar() {
  mesh_information_label_ = new QLabel;
  build_date_label_ = new QLabel;
  build_date_label_->setText(kStatusText);

  auto icon = new QLabel;
  icon->setTextFormat(Qt::RichText);
  icon->setText("<img src=\"image://theme/dialog-information\" />");
  ui_->statusbar->addWidget(icon);
  ui_->statusbar->addWidget(mesh_information_label_);
  ui_->statusbar->addPermanentWidget(build_date_label_);

  // Remove border around label
  ui_->statusbar->setStyleSheet("QStatusBar::item { border: 0; }");
}

void MainWindow::setup_viewer_tabs() {
  setup_viewer_tab(ui_->tab_viewer_mesh,
                   new ViewerMeshWidget(ui_->tab_viewer_mesh, 0),
                   new ViewerMeshWidget(ui_->tab_viewer_mesh, 1));

  viewer_plugin1_ = new ViewerPluginWidget(ui_->tab_viewer_plugin, 0);
  viewer_plugin2_ = new ViewerPluginWidget(ui_->tab_viewer_plugin, 1);
  setup_viewer_tab(ui_->tab_viewer_plugin, viewer_plugin1_, viewer_plugin2_);
}

void MainWindow::setup_viewer_tab(QWidget* tab, ViewerWidget* viewer1,
                                  ViewerWidget* viewer2) {
  auto layout = new QHBoxLayout(tab);
  layout->addWidget(viewer1);
  layout->addWidget(viewer2);
  viewer1->set_model(mesh_data_);
  viewer2->set_model(mesh_data_);
}

void MainWindow::setup_plugin_guis() {
  auto& plugins = plugin_manager_.get_plugins();
  for (const auto& it : plugins) {
    const auto& wrapper = it.second;
    QWidget* plugin_container = new QWidget(this);
    ui_->tabs_plugins->addTab(plugin_container, wrapper.name);
    wrapper.plugin->create_gui(plugin_container);
    QObject::connect(ui_->tabs_plugins, SIGNAL(currentChanged(int)),
                     this, SLOT(plugin_tab_changed(int)));
  }

  if (plugins.size() > 0) {
    plugin_tab_changed(0);
  }
}

void MainWindow::plugin_tab_changed(int current) {
  auto it = plugin_manager_.get_plugins().begin();

  for (int i = 0; i < current; i++) {
    it++;
  }
  viewer_plugin1_->set_drawing_plugin(it->second.plugin.get());
  viewer_plugin2_->set_drawing_plugin(it->second.plugin.get());
  ui_->tabs_viewer->setTabText(1, it->second.name);
}

void MainWindow::setup_menus() {
  QObject::connect(ui_->action_load, SIGNAL(triggered(bool)), this,
                   SLOT(show_load_dialog()));
  QObject::connect(ui_->action_save, SIGNAL(triggered(bool)), this,
                   SLOT(show_save_dialog()));
  QObject::connect(ui_->action_snapshot, SIGNAL(triggered(bool)),
                   ui_->tab_viewer_mesh, SLOT(saveSnapshot(bool)));
  QObject::connect(ui_->action_quit, SIGNAL(triggered(bool)),
                   QApplication::instance(), SLOT(quit()));
  QObject::connect(ui_->action_triangulate, SIGNAL(triggered(bool)),
                   this, SLOT(triangulate_mesh(void)));
  QObject::connect(ui_->action_undo, SIGNAL(triggered(bool)),
                   this, SLOT(undo(void)));
  QObject::connect(ui_->action_redo, SIGNAL(triggered(bool)),
                   this, SLOT(redo(void)));

  // On startup redo/undo is not available
  ui_->action_redo->setEnabled(false);
  ui_->action_undo->setEnabled(false);
}

void MainWindow::undo() {
  mesh_data_.history_step_back();
}

void MainWindow::redo() {
  mesh_data_.history_step_forward();
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
    mesh_data_.history_purge();
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
