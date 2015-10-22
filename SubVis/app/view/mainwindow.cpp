#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>

#include "ui_mainwindow.h"
#include "view/mainwindow.h"

namespace subvis {

using MeshPairRef =
  std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>;

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

  connect(&mesh_data_, &MeshData::updated, this, &MainWindow::mesh_updated);
}

void MainWindow::mesh_updated(MeshPairRef meshes) {
  QString info = "[0]: ";
  info += get_stats(meshes.first);
  info += "       [1]: ";
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
  viewer_mesh0_ = new ViewerMeshWidget(ui_->tab_viewer_mesh, 0);
  viewer_mesh1_ = new ViewerMeshWidget(ui_->tab_viewer_mesh, 1);
  setup_viewer_tab(ui_->tab_viewer_mesh, viewer_mesh0_, viewer_mesh1_);

  viewer_plugin0_ = new ViewerPluginWidget(ui_->tab_viewer_plugin, 0);
  viewer_plugin1_ = new ViewerPluginWidget(ui_->tab_viewer_plugin, 1);
  setup_viewer_tab(ui_->tab_viewer_plugin, viewer_plugin0_, viewer_plugin1_);

  toggle_sync_views(true);
}

void MainWindow::setup_viewer_tab(QWidget* tab, ViewerWidget* viewer0,
                                  ViewerWidget* viewer1) {
  auto layout = new QHBoxLayout;

  viewer0->set_model(mesh_data_);
  viewer1->set_model(mesh_data_);

  layout->addWidget(viewer0);
  layout->addWidget(viewer1);

  tab->setLayout(layout);
}

void MainWindow::sync_viewers(QGLViewer* viewer0, QGLViewer* viewer1) {
  // Warning: This connection must be disconnected before the objects are garbage collected.
  auto old_cam = viewer1->camera();
  viewer1->setCamera(viewer0->camera());
  delete old_cam;
  viewer1->updateGL();
}

void MainWindow::unsync_viewers(QGLViewer* viewer0, QGLViewer* viewer1) {
  auto old_cam0 = viewer0->camera();
  auto old_cam1 = viewer1->camera();
  viewer0->setCamera(new qglviewer::Camera(*old_cam0));
  viewer1->setCamera(new qglviewer::Camera(*old_cam1));
  delete old_cam0;
  if (old_cam0 != old_cam1) {
    delete old_cam1;
  }
}

void MainWindow::toggle_splitscreen(bool show) {
  viewer_mesh1_->setVisible(show);
  viewer_plugin1_->setVisible(show);
}

void MainWindow::setup_plugin_guis() {
  auto& plugins = plugin_manager_.get_plugins();
  for (const auto& it : plugins) {
    const auto& wrapper = it.second;
    QWidget* plugin_container = new QWidget(this);
    ui_->tabs_plugins->addTab(plugin_container, wrapper.name);
    wrapper.plugin->create_gui(plugin_container);
    connect(ui_->tabs_plugins, &QTabWidget::currentChanged,
            this, &MainWindow::plugin_tab_changed);
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
  viewer_plugin0_->set_drawing_plugin(it->second.plugin.get());
  viewer_plugin1_->set_drawing_plugin(it->second.plugin.get());
  ui_->tabs_viewer->setTabText(1,
                               it->second.name + ": " + it->second.plugin->viewer_tab_text());
}

void MainWindow::setup_menus() {
  connect(ui_->action_load, &QAction::triggered, this,
          &MainWindow::show_load_dialog);
  connect(ui_->action_save, &QAction::triggered, this,
          &MainWindow::show_save_dialog);
  connect(ui_->action_snapshot_left, &QAction::triggered, this,
          &MainWindow::save_snapshot0);
  connect(ui_->action_snapshot_right, &QAction::triggered, this,
          &MainWindow::save_snapshot1);
  connect(ui_->action_quit, &QAction::triggered, QApplication::instance(),
          &QApplication::quit);
  connect(ui_->action_triangulate_left, &QAction::triggered, this,
          &MainWindow::triangulate_mesh0);
  connect(ui_->action_triangulate_right, &QAction::triggered, this,
          &MainWindow::triangulate_mesh1);
  connect(ui_->action_sync_views, &QAction::toggled, this,
          &MainWindow::toggle_sync_views);
  connect(ui_->action_toggle_splitscreen, &QAction::toggled, this,
          &MainWindow::toggle_splitscreen);
  connect(ui_->action_undo,  &QAction::triggered, this, &MainWindow::undo);
  connect(ui_->action_redo,  &QAction::triggered, this, &MainWindow::redo);
  connect(ui_->action_edit,  &QAction::toggled, this, &MainWindow::toggle_edit);

  // On startup redo/undo is not available
  ui_->action_redo->setEnabled(false);
  ui_->action_undo->setEnabled(false);
}

void MainWindow::save_snapshot0() {
  viewer_mesh0_->saveSnapshot();
}

void MainWindow::save_snapshot1() {
  viewer_mesh1_->saveSnapshot();
}

void MainWindow::toggle_sync_views(bool sync) {
  if (sync) {
    sync_viewers(viewer_mesh0_, viewer_mesh1_);
    sync_viewers(viewer_plugin0_, viewer_plugin1_);
  } else {
    unsync_viewers(viewer_mesh0_, viewer_mesh1_);
    unsync_viewers(viewer_plugin0_, viewer_plugin1_);
  }
}

void MainWindow::toggle_edit(bool edit) {
  if (edit) {
    ui_->tabs_viewer->setCurrentIndex(0);
    ui_->tabs_viewer->setTabEnabled(1, false);
  } else {
    ui_->tabs_viewer->setTabEnabled(1, true);
  }
  ui_->tabs_plugins->setHidden(edit);
  ui_->action_sync_views->setChecked(!edit);
  ui_->action_sync_views->setEnabled(!edit);
  ui_->action_toggle_splitscreen->setChecked(!edit);
  ui_->action_toggle_splitscreen->setEnabled(!edit);
}

void MainWindow::undo() {
  mesh_data_.history_step_back();
}

void MainWindow::redo() {
  mesh_data_.history_step_forward();
}

void MainWindow::triangulate_mesh0() {
  mesh_data_.triangulate(0);
}

void MainWindow::triangulate_mesh1() {
  mesh_data_.triangulate(1);
}

void MainWindow::show_load_dialog() {
  QString file_filter{QString::fromStdString(mesh_data_.get_load_file_formats())};
  QString fn{QFileDialog::getOpenFileName(this, kLoadDialogCaption, "", file_filter)};

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
  QString fn{QFileDialog::getSaveFileName(this, kSaveDialogCaption, "", file_filter)};

  if (!fn.isNull()) {
    const std::string filename = fn.toStdString();
    if (!mesh_data_.persist(filename)) {
      QString msg = "Failed to save to file " + fn;
      QMessageBox::warning(this, "Error", msg);
    }
  }
}

MainWindow::~MainWindow() {
  toggle_sync_views(false);
  delete ui_;
}

} // namespace subvis
