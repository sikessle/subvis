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

void SubdivisionAlgorithmsPlugin::set_draw_controller(subvis::DrawController&
    draw_controller) {
  draw_controller_ = &draw_controller;
}

void SubdivisionAlgorithmsPlugin::mesh_updated(const surface_mesh::Surface_mesh&
    mesh) {
  auto& renderer = active_algorithm_renderer_pair().renderer;
  renderer->mesh_updated(mesh);
  update_valid_dropdown_items(mesh);
}

void SubdivisionAlgorithmsPlugin::init_opengl() {
  auto& renderer = active_algorithm_renderer_pair().renderer;
  renderer->init_opengl();
}

void SubdivisionAlgorithmsPlugin::draw_opengl() {
  auto& renderer = active_algorithm_renderer_pair().renderer;
  renderer->render_mesh_opengl();
}

AlgorithmRenderer&
SubdivisionAlgorithmsPlugin::active_algorithm_renderer_pair() {
  if (dropdown_->count() == 0) {
    throw std::logic_error{"no algorithms loaded. ensure that at least one is loaded in the constructor."};
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

void SubdivisionAlgorithmsPlugin::update_valid_dropdown_items(
  const surface_mesh::Surface_mesh& mesh) {


  QStandardItemModel* model = qobject_cast<QStandardItemModel*>
                              (dropdown_->model());

  int first_enabled_item = enable_applicable_algorithms_dropdown(mesh, model);
  ensure_current_dropdown_item_is_enabled(model, first_enabled_item);
}

int SubdivisionAlgorithmsPlugin::enable_applicable_algorithms_dropdown(
  const surface_mesh::Surface_mesh& mesh, QStandardItemModel* model) {

  QStandardItem* item {nullptr};
  int first_enabled_item_index { -1};

  for (int i = 0; i < dropdown_->count(); i++) {
    QModelIndex index = model->index(i, dropdown_->modelColumn(),
                                     dropdown_->rootModelIndex());
    item = model->itemFromIndex(index);

    if (algorithms_.at(item->text()).algorithm->is_subdividable(mesh)) {
      item->setEnabled(true);
      if (first_enabled_item_index == -1) {
        first_enabled_item_index = i;
      }
    } else {
      item->setEnabled(false);
    }
  }

  return first_enabled_item_index;
}


void SubdivisionAlgorithmsPlugin::ensure_current_dropdown_item_is_enabled(
  QStandardItemModel* model, int first_enabled_item_index) {
  QModelIndex current_index = model->index(dropdown_->currentIndex(),
                              dropdown_->modelColumn(), dropdown_->rootModelIndex());
  if (!model->itemFromIndex(current_index)->isEnabled()) {
    dropdown_->setCurrentIndex(first_enabled_item_index);
  }
}

} // namespace subdivision
