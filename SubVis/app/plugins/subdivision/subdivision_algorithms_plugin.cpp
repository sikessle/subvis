#include "surface_mesh/Surface_mesh.h"

#include "plugins/subdivision/gl_bspline_renderer.h"

#include "plugins/subdivision/sd_catmull.h"
#include "plugins/subdivision/sd_doosabin.h"
#include "plugins/subdivision/sd_loop.h"
#include "plugins/subdivision/sd_butterfly.h"
#include "plugins/subdivision/sd_modbutterfly.h"

#include "plugins/subdivision/subdivision_algorithms_plugin.h"

namespace subdivision {

SubdivisionAlgorithmsPlugin::SubdivisionAlgorithmsPlugin() {
  // Add all special renderers here to share them among the algorithms
  std::shared_ptr<GLRenderer> bspline_renderer {new GLBSplineRenderer};

  // Add here all the algorithms and their special renderer
  algorithms_["Catmull-Clark"] = {
    std::unique_ptr<SdAlgorithm>{new SdCatmull},
    bspline_renderer
  };
  algorithms_["Doo-Sabin"] = {
    std::unique_ptr<SdAlgorithm>{new SdDooSabin},
    bspline_renderer
  };
  algorithms_["Loop"] = {
    std::unique_ptr<SdAlgorithm>{new SdLoop},
    bspline_renderer
  };
  algorithms_["Butterfly"] = {
    std::unique_ptr<SdAlgorithm>{new SdButterfly},
    bspline_renderer
  };
  algorithms_["Modified Butterfly"] = {
    std::unique_ptr<SdAlgorithm>{new SdModButterfly},
    bspline_renderer
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
