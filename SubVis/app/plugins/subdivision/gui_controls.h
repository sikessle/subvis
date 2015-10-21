#ifndef SUBVIS_PLUGINS_SUBDIVISION_GUI_CONTROLS_H
#define SUBVIS_PLUGINS_SUBDIVISION_GUI_CONTROLS_H

#include <QObject>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QStandardItemModel>

#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/algorithm_renderer.h"

namespace subdivision {

class GuiControls : public QObject {
  Q_OBJECT

 public:
  GuiControls();

  void set_model(subvis::MeshData& mesh_data);
  void create(QWidget* parent,
              std::map<const QString, AlgorithmRenderer>& algorithms);
  void mesh_updated(const surface_mesh::Surface_mesh& mesh);
  void init_opengl();
  void draw_opengl();

 private:
  subvis::MeshData* mesh_data_{nullptr};
  std::map<const QString, AlgorithmRenderer>* algorithms_{nullptr};
  // memory managed by Qt's parent-relationship
  QPushButton* subdivide_{nullptr};
  QPushButton* stop_{nullptr};
  QSpinBox* steps_{nullptr};
  QComboBox* dropdown_{nullptr};

  AlgorithmRenderer& current_algo_render_pair();
  void update_valid_dropdown_items(const surface_mesh::Surface_mesh& mesh);
  /// Enables dropdown items (algorithms) if they can subdivide the current mesh.
  /// Returns the first enabled item index after enabling/disabling the items.
  int enable_applicable_algorithms_dropdown(const surface_mesh::Surface_mesh&
      mesh, QStandardItemModel* model);
  void ensure_current_dropdown_item_is_enabled(QStandardItemModel* model,
      int first_enabled_item);

 private slots:
  void subdivide_clicked(bool);
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GUI_CONTROLS_H
