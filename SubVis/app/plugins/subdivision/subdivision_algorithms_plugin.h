#ifndef SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H
#define SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H

#include <QObject>
#include <string>

#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/algorithm_renderer.h"
#include "plugins/subdivision/gui_controls.h"

namespace subdivision {

class SubdivisionAlgorithmsPlugin : public subvis::SubVisPlugin {
  Q_OBJECT

 public:
  SubdivisionAlgorithmsPlugin();

  const QString id() const override;
  const QString name() const override;
  void set_model(subvis::MeshData& mesh_data) override;
  void mesh_updated(const surface_mesh::Surface_mesh& mesh, int mesh_id) override;
  void init_opengl(int mesh_id) override;
  void draw_opengl(int mesh_id) override;
  void create_gui(QWidget* parent) override;
  void request_redraw();

 private:
  /// (mesh_id, name)->(algorithm, renderer)
  std::map<std::pair<int, const QString>, AlgorithmRenderer> algorithms_;
  GuiControls gui_;

  void init_algorithm(int mesh_id, const QString name, SdAlgorithm* algorithm,
                      GLRenderer* renderer);
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H
