#include <QtDebug>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QActionGroup>

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
  // Set the plugins on the viewers.
  viewer_plugin0_->set_drawing_plugin(it->second.plugin.get());
  viewer_plugin1_->set_drawing_plugin(it->second.plugin.get());
  // Update the tab text for the plugin tab
  ui_->tabs_viewer->setTabText(1,
                               it->second.name + ": " + it->second.plugin->viewer_tab_text());
}

void MainWindow::setup_menus() {
  connect(ui_->action_load, &QAction::triggered, this,
          &MainWindow::show_load_dialog);
  connect(ui_->action_save_left, &QAction::triggered, this,
          &MainWindow::show_save_dialog0);
  connect(ui_->action_save_right, &QAction::triggered, this,
          &MainWindow::show_save_dialog1);
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
  connect(ui_->action_edit,  &QAction::toggled, viewer_mesh0_,
          &ViewerMeshWidget::set_edit);
  connect(ui_->action_draw_edges, &QAction::triggered, this,
          &MainWindow::set_drawing_type_edges);
  connect(ui_->action_draw_vertices, &QAction::triggered, this,
          &MainWindow::set_drawing_type_vertices);
  connect(ui_->action_draw_faces, &QAction::triggered, this,
          &MainWindow::set_drawing_type_faces);
  connect(ui_->action_shading_flat, &QAction::triggered, this,
          &MainWindow::set_shading_type_flat);
  connect(ui_->action_shading_smooth, &QAction::triggered, this,
          &MainWindow::set_shading_type_smooth);
  connect(ui_->action_coloring_on, &QAction::triggered, this,
          &MainWindow::set_coloring_active);
  connect(ui_->action_coloring_off, &QAction::triggered, this,
          &MainWindow::set_coloring_not_active);
  connect(ui_->action_lighting_on, &QAction::triggered, this,
          &MainWindow::set_lighting_active);
  connect(ui_->action_lighting_off, &QAction::triggered, this,
          &MainWindow::set_lighting_not_active);

  // On startup redo/undo is not available
  ui_->action_redo->setEnabled(false);
  ui_->action_undo->setEnabled(false);

  // Group drawing actions (mutually exclusive selection)
  QActionGroup *drawing_group = new QActionGroup(this);
  drawing_group->addAction(ui_->action_draw_edges);
  drawing_group->addAction(ui_->action_draw_vertices);
  drawing_group->addAction(ui_->action_draw_faces);

  // Group shading actions (mutually exclusive selection)
  QActionGroup *shading_group = new QActionGroup(this);
  shading_group->addAction(ui_->action_shading_flat);
  shading_group->addAction(ui_->action_shading_smooth);

  // Group coloring actions (mutually exclusive selection)
  QActionGroup *coloring_group = new QActionGroup(this);
  coloring_group->addAction(ui_->action_coloring_on);
  coloring_group->addAction(ui_->action_coloring_off);

  // Group lighting actions (mutually exclusive selection)
  QActionGroup *lighting_group = new QActionGroup(this);
  lighting_group->addAction(ui_->action_lighting_on);
  lighting_group->addAction(ui_->action_lighting_off);
}

void MainWindow::save_snapshot0() {
  viewer_mesh0_->saveSnapshot(false);
}

void MainWindow::save_snapshot1() {
  viewer_mesh1_->saveSnapshot(false);
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
    // Disable the plugin viewer tab
    ui_->tabs_viewer->setCurrentIndex(0);
    ui_->tabs_viewer->setTabEnabled(1, false);
  } else {
    // Enable the plugin viewer tab
    ui_->tabs_viewer->setTabEnabled(1, true);
  }
  // Hide or unhide some controls if we are in edit mode to have more space and
  // let the user focus on the mesh edits.
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
      qWarning() << qPrintable(msg);
    }
  }
}

void MainWindow::show_save_dialog0() {
  show_save_dialog(0);
}

void MainWindow::show_save_dialog1() {
  show_save_dialog(1);
}

void MainWindow::show_save_dialog(int idx) {
  QString file_filter{QString::fromStdString(mesh_data_.get_persist_file_formats())};
  QString fn{QFileDialog::getSaveFileName(this, kSaveDialogCaption, "", file_filter)};

  if (!fn.isNull()) {
    const std::string filename = fn.toStdString();
    if (!mesh_data_.persist(filename, idx)) {
      QString msg = "Failed to save to file " + fn;
      QMessageBox::warning(this, "Error", msg);
      qWarning() << qPrintable(msg);
    }
  }
}

void MainWindow::set_drawing_type_edges() {
  set_drawing_type(GL_LINES);
}

void MainWindow::set_drawing_type_vertices() {
  set_drawing_type(GL_POINTS);
}

void MainWindow::set_drawing_type_faces() {
  set_drawing_type(GL_TRIANGLES);
}

void MainWindow::set_drawing_type(GLenum type) {
  viewer_mesh0_->set_drawing_type(type);
  viewer_mesh1_->set_drawing_type(type);
}

void MainWindow::set_shading_type_flat() {
  set_shading_type(GL_FLAT);
}

void MainWindow::set_shading_type_smooth() {
  set_shading_type(GL_SMOOTH);
}

void MainWindow::set_shading_type(GLenum type) {
  viewer_mesh0_->set_shading_type(type);
  viewer_mesh1_->set_shading_type(type);
}

void MainWindow::set_coloring_active() {
  set_coloring(true);
}

void MainWindow::set_coloring_not_active() {
  set_coloring(false);
}

void MainWindow::set_coloring(bool active) {
  viewer_mesh0_->set_coloring(active);
  viewer_mesh1_->set_coloring(active);
}

void MainWindow::set_lighting_active() {
  set_lighting(true);
}

void MainWindow::set_lighting_not_active() {
  set_lighting(false);
}

void MainWindow::set_lighting(bool active) {
  viewer_mesh0_->set_lighting(active);
  viewer_mesh1_->set_lighting(active);
}

MainWindow::~MainWindow() {
  // It is important to unsync the views so they are not sharing the same camera
  // object. Otherwise a segfault will occur, as they try to free the same object
  // twice.
  toggle_sync_views(false);
  delete ui_;
}

} // namespace subvis
