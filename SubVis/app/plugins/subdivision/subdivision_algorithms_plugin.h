#ifndef SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H
#define SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H

#include <QObject>
#include <string>

#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/algorithm_renderer.h"
#include "plugins/subdivision/gui_controls.h"

namespace subdivision {

class SubdivisionAlgorithmsPlugin : public QObject,
  public subvis::SubVisPlugin {
  Q_OBJECT

 public:
  SubdivisionAlgorithmsPlugin();

  const QString id() const override;
  const QString name() const override;
  void set_model(subvis::MeshData& mesh_data) override;
  void mesh_updated(const surface_mesh::Surface_mesh& mesh) override;
  void init_opengl() override;
  void draw_opengl() override;
  void create_gui(QWidget* parent) override;

 private:
  /// name->(algorithm, renderer)
  std::map<const QString, AlgorithmRenderer> algorithms_;
  GuiControls gui_;

};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H
