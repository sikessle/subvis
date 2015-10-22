#ifndef SUBVIS_VIEW_MAINWINDOW_H
#define SUBVIS_VIEW_MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>

#include "view/viewer_mesh_widget.h"
#include "view/viewer_plugin_widget.h"
#include "model/mesh_data.h"
#include "plugins/plugin_manager.h"

namespace subvis {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(MeshData& mesh_data,
             PluginManager& plugin_manager);
  ~MainWindow();

  void show_load_dialog();
  void show_save_dialog();
  void save_snapshot0();
  void save_snapshot1();
  void triangulate_mesh0();
  void triangulate_mesh1();
  void undo();
  void redo();
  void toggle_sync_views(bool sync);
  void toggle_splitscreen(bool show);
  void plugin_tab_changed(int current);
  void mesh_updated(
    std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>
    meshes);

 private:
  Ui::MainWindow* ui_;
  MeshData& mesh_data_;
  PluginManager& plugin_manager_;
  // memory managed by Qt's parent-relationship (ui)
  QLabel* build_date_label_{nullptr};
  QLabel* mesh_information_label_{nullptr};
  ViewerMeshWidget* viewer_mesh0_{nullptr};
  ViewerMeshWidget* viewer_mesh1_{nullptr};
  ViewerPluginWidget* viewer_plugin0_{nullptr};
  ViewerPluginWidget* viewer_plugin1_{nullptr};

  const QString kStatusText {"Build: " __DATE__ " " __TIME__};
  const QString kSaveDialogCaption {"Save model file"};
  const QString kLoadDialogCaption {"Load model file"};

  void setup_status_bar();
  void setup_viewer_tabs();
  void setup_viewer_tab(QWidget* tab, ViewerWidget* viewer0,
                        ViewerWidget* viewer1);
  void sync_viewers(QGLViewer* viewer0, QGLViewer* viewer1);
  void unsync_viewers(QGLViewer* viewer0, QGLViewer* viewer1);
  void setup_menus();
  void setup_plugin_guis();
  QString get_stats(const surface_mesh::Surface_mesh& mesh);
};


} // namespace subvis
#endif // SUBVIS_VIEW_MAINWINDOW_H
