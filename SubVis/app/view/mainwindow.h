#ifndef SUBVIS_MAINWINDOW_H
#define SUBVIS_MAINWINDOW_H

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

 private:
  Ui::MainWindow* ui_;
  MeshData& mesh_data_;
  PluginManager& plugin_manager_;
  // memory managed by Qt's parent-relationship (ui)
  QLabel* status_label_;

  const QString kStatusText {"Build: " __DATE__ " " __TIME__};
  const QString kSaveDialogCaption {"Save model file"};
  const QString kLoadDialogCaption {"Load model file"};

  void setup_status_bar();
  void setup_viewer_tabs();
  void setup_toolbar();
  void setup_plugin_guis();

 public slots:
  void show_load_dialog();
  void show_save_dialog();
  void triangulate_mesh();
  void plugin_tab_changed(int current);
};


} // namespace subvis
#endif // SUBVIS_MAINWINDOW_H
