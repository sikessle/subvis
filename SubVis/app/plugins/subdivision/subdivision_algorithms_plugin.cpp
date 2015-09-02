#include <exception>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "surface_mesh/Surface_mesh.h"

#include "plugins/subdivision/gl_bspline_renderer.h"

#include "plugins/subdivision/sd_catmull.h"
#include "plugins/subdivision/sd_doosabin.h"
#include "plugins/subdivision/sd_loop.h"
#include "plugins/subdivision/sd_butterfly.h"

#include "plugins/subdivision/subdivision_algorithms_plugin.h"

namespace subdivision {

SubdivisionAlgorithmsPlugin::SubdivisionAlgorithmsPlugin() {
  // Add all special renderers here to share them among the algorithms
  std::shared_ptr<GLRenderer> bspline_renderer {new BSplineGLRenderer};

  // Add here all the algorithms and their special renderer

  algorithms_["Catmull-Clark"] = {
    std::unique_ptr<SubdivAlgorithm>{new SubdivCatmull},
    bspline_renderer
  };
  algorithms_["Doo-Sabin"] = {
    std::unique_ptr<SubdivAlgorithm>{new SubdivDooSabin},
    bspline_renderer
  };
  algorithms_["Loop"] = {
    std::unique_ptr<SubdivAlgorithm>{new SubdivLoop},
    bspline_renderer
  };
  algorithms_["Butterfly"] = {
    std::unique_ptr<SubdivAlgorithm>{new SubdivButterfly},
    bspline_renderer
  };
}

const QString SubdivisionAlgorithmsPlugin::id() const {
  return "de.htwg.ios.subvis.plugin.subdivision";
}

const QString SubdivisionAlgorithmsPlugin::name() const {
  return "Subdivision";
}

void SubdivisionAlgorithmsPlugin::set_draw_controller(subvis::DrawController*
    draw_controller) {
  draw_controller_ = draw_controller;
}

void SubdivisionAlgorithmsPlugin::draw_opengl() {
  auto& renderer = active_algorithm_renderer_pair().renderer;
  renderer->render_mesh_opengl(draw_controller_->get_mesh_data().get_mesh());
}

AlgorithmRenderer&
SubdivisionAlgorithmsPlugin::active_algorithm_renderer_pair() {
  if (dropdown_->count() == 0) {
    throw new std::logic_error("no algorithms loaded. ensure that at least one is loaded in the constructor.");
  }
  const QString name = dropdown_->currentText();

  return algorithms_.at(name);
}

void SubdivisionAlgorithmsPlugin::create_gui(QWidget* parent) {
  // Gui creation
  QVBoxLayout* layout = new QVBoxLayout(parent);
  layout->setAlignment(Qt::AlignTop);

  layout->addWidget(new QLabel("Algorithm:"));

  dropdown_ = new QComboBox(parent);
  for (const auto& it : algorithms_) {
    // using name from map's entries as label
    dropdown_->addItem(it.first);
  }
  layout->addWidget(dropdown_);

  QHBoxLayout* layout_steps = new QHBoxLayout;
  layout_steps->setAlignment(Qt::AlignTop);

  layout_steps->addWidget(new QLabel("Steps:"));
  steps_ = new QSpinBox(parent);
  steps_->setRange(1, 100);
  layout_steps->addWidget(steps_);

  layout->addLayout(layout_steps);

  subdivide_ = new QPushButton("subdivide", parent);
  layout->addWidget(subdivide_);

  QObject::connect(subdivide_, SIGNAL(clicked(bool)),
                   this, SLOT(subdivide_clicked(bool)));
}

void SubdivisionAlgorithmsPlugin::subdivide_clicked(bool) {
  const int steps = steps_->value();
  auto& mesh_data = draw_controller_->get_mesh_data();
  auto& algorithm = active_algorithm_renderer_pair().algorithm;

  auto result = algorithm->subdivide(mesh_data.get_mesh(), steps);

  mesh_data.load(std::move(result));
}

} // namespace subdivision
