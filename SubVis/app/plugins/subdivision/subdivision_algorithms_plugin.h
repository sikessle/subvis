#ifndef SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H
#define SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H

#include <QObject>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <string>

#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/sd_algorithm.h"
#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

struct AlgorithmRenderer {
  std::unique_ptr<SdAlgorithm> algorithm;
  /**
   * @brief shared_ptr to allow multiple algorithms to use the same renderer
   */
  std::shared_ptr<GLRenderer> renderer;
};

class SubdivisionAlgorithmsPlugin : public QObject,
  public subvis::SubVisPlugin {
  Q_OBJECT

 public:
  SubdivisionAlgorithmsPlugin();

  const QString id() const override;
  const QString name() const override;
  void set_draw_controller(subvis::DrawController& draw_controller)
  override;
  void mesh_updated(const surface_mesh::Surface_mesh& mesh) override;
  void init_opengl() override;
  void draw_opengl() override;
  void create_gui(QWidget* parent) override;

 private:
  subvis::DrawController* draw_controller_{nullptr};
  /**
   * @brief name->(algorithm, renderer)
   */
  std::map<const QString, AlgorithmRenderer> algorithms_;

  // memory managed by Qt's parent-relationship
  QPushButton* subdivide_{nullptr};
  QSpinBox* steps_{nullptr};
  QComboBox* dropdown_{nullptr};

  AlgorithmRenderer& active_algorithm_renderer_pair();
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
#endif // SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H
