#include "surface_mesh/Surface_mesh.h"

#include "plugins/subdivision/gl_none_renderer.h"
#include "plugins/subdivision/gl_bspline_renderer.h"

#include "plugins/subdivision/sd_none.h"
#include "plugins/subdivision/sd_catmull.h"
#include "plugins/subdivision/sd_doosabin.h"
#include "plugins/subdivision/sd_loop.h"
#include "plugins/subdivision/sd_butterfly.h"
#include "plugins/subdivision/sd_modbutterfly.h"

#include "plugins/subdivision/subdivision_algorithms_plugin.h"

namespace subdivision {

SubdivisionAlgorithmsPlugin::SubdivisionAlgorithmsPlugin() {
  // Add here all the algorithms and their special renderer
  // Add them twice for each mesh

  init_algorithm(0, "-- None --", new SdNone, new GLNoneRenderer);
  init_algorithm(1, "-- None --", new SdNone, new GLNoneRenderer);

  init_algorithm(0, "Catmull-Clark", new SdCatmull, new GLBSplineRenderer);
  init_algorithm(1, "Catmull-Clark", new SdCatmull, new GLBSplineRenderer);

  init_algorithm(0, "Doo-Sabin", new SdDooSabin, new GLBSplineRenderer);
  init_algorithm(1, "Doo-Sabin", new SdDooSabin, new GLBSplineRenderer);

  init_algorithm(0, "Loop", new SdLoop, new GLBSplineRenderer);
  init_algorithm(1, "Loop", new SdLoop, new GLBSplineRenderer);

  init_algorithm(0, "Butterfly", new SdButterfly, new GLBSplineRenderer);
  init_algorithm(1, "Butterfly", new SdButterfly, new GLBSplineRenderer);

  init_algorithm(0, "Modified Butterfly", new SdModButterfly,
                 new GLBSplineRenderer);
  init_algorithm(1, "Modified Butterfly", new SdModButterfly,
                 new GLBSplineRenderer);
}

void SubdivisionAlgorithmsPlugin::init_algorithm(int mesh_id,
    const QString name, SdAlgorithm* algorithm, GLRenderer* renderer) {
  std::pair<int, QString> key = {mesh_id, name};
  algorithms_[key] = {
    std::unique_ptr<SdAlgorithm>{algorithm},
    std::shared_ptr<GLRenderer> {renderer}
  };
}

const QString SubdivisionAlgorithmsPlugin::id() const {
  return "de.htwg.ios.subvis.plugin.subdivision";
}

const QString SubdivisionAlgorithmsPlugin::name() const {
  return "Subdivision";
}

void SubdivisionAlgorithmsPlugin::set_model(subvis::MeshData&
    mesh_data) {
  gui_.set_model(mesh_data);
}

void SubdivisionAlgorithmsPlugin::mesh_updated(const surface_mesh::Surface_mesh&
    mesh, int mesh_id) {
  gui_.mesh_updated(mesh, mesh_id);
}

void SubdivisionAlgorithmsPlugin::init_opengl(int mesh_id) {
  gui_.init_opengl(mesh_id);
}

void SubdivisionAlgorithmsPlugin::draw_opengl(int mesh_id) {
  gui_.draw_opengl(mesh_id);
}

void SubdivisionAlgorithmsPlugin::create_gui(QWidget* parent) {
  gui_.create(parent, algorithms_);
}

} // namespace subdivision
