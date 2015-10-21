#ifndef SUBVIS_PLUGINS_SUBDIVISION_GUI_CONTROLS_H
#define SUBVIS_PLUGINS_SUBDIVISION_GUI_CONTROLS_H

#include <QObject>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QProgressBar>

#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/algorithm_renderer.h"

namespace subdivision {

class GuiControls : public QObject {
  Q_OBJECT

 public:
  GuiControls();

  void set_model(subvis::MeshData& mesh_data);
  void create(QWidget* parent,
              std::map<std::pair<const int, const QString>, AlgorithmRenderer>& algorithms);
  void mesh_updated(const surface_mesh::Surface_mesh& mesh, int mesh_id);
  void init_opengl(int mesh_id);
  void draw_opengl(int mesh_id);

 private:
  subvis::MeshData* mesh_data_{nullptr};
  std::map<int, SdAlgorithm*> mesh_id_active_algorithm_;
  std::map<std::pair<const int, const QString>, AlgorithmRenderer>* algorithms_{nullptr};
  // memory managed by Qt's parent-relationship
  QPushButton* subdivide_{nullptr};
  QPushButton* stop_{nullptr};
  QSpinBox* steps_{nullptr};
  QComboBox* dropdown1_{nullptr};
  QComboBox* dropdown2_{nullptr};
  QProgressBar* progress_{nullptr};

  AlgorithmRenderer& selected_algo_renderer(QComboBox* dropdown, int mesh_id);
  AlgorithmRenderer& current_algo_render_pair(int mesh_id);
  void update_valid_items(QComboBox* dropdown, int mesh_id,
                          const surface_mesh::Surface_mesh& mesh);
  void update_valid_dropdown_items(const surface_mesh::Surface_mesh& mesh,
                                   int mesh_id);
  /// Enables dropdown items (algorithms) if they can subdivide the current mesh.
  /// Returns the first enabled item index after enabling/disabling the items.
  int enable_applicable_algorithms_dropdown(QComboBox* dropdown, const surface_mesh::Surface_mesh&
      mesh, int mesh_id, QStandardItemModel* model);
  void ensure_current_dropdown_item_is_enabled(QComboBox* dropdown, QStandardItemModel* model,
      int first_enabled_item);
  void set_progress_controls_visible(bool visible);

 private slots:
  void subdivide_clicked(bool);
  void stop_clicked(bool);
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GUI_CONTROLS_H
