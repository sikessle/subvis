#include <exception>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "plugins/subdivision/gui_controls.h"

namespace subdivision {

GuiControls::GuiControls() {

}

AlgorithmRenderer&
GuiControls::current_algo_render_pair() {
  if (dropdown_->count() == 0) {
    throw std::logic_error{"no algorithms loaded. ensure that at least one is loaded in the constructor."};
  }
  const QString name = dropdown_->currentText();

  return algorithms_->at(name);
}

void GuiControls::set_model(subvis::MeshData& mesh_data) {
  mesh_data_ = &mesh_data;
}

void GuiControls::mesh_updated(const surface_mesh::Surface_mesh& mesh, int mesh_id) {
  // TODO select
  //current_algo_render_pair().renderer->mesh_updated(meshes);
  update_valid_dropdown_items(mesh);
}

void GuiControls::init_opengl(int mesh_id) {
  current_algo_render_pair().renderer->init_opengl();
}

void GuiControls::draw_opengl(int mesh_id) {
  current_algo_render_pair().renderer->render_mesh_opengl();
}

void GuiControls::create(QWidget* parent,
                         std::map<const QString, AlgorithmRenderer>& algorithms) {
  algorithms_ = &algorithms;
  // Gui creation
  QVBoxLayout* layout = new QVBoxLayout(parent);
  layout->setAlignment(Qt::AlignTop);

  layout->addWidget(new QLabel("Algorithm:"));

  dropdown_ = new QComboBox(parent);
  for (const auto& it : *algorithms_) {
    // using name from map's entries as label
    dropdown_->addItem(it.first);
  }
  layout->addWidget(dropdown_);

  QHBoxLayout* layout_steps = new QHBoxLayout;
  layout_steps->setAlignment(Qt::AlignTop);

  layout_steps->addWidget(new QLabel("Steps:"));
  steps_ = new QSpinBox(parent);
  steps_->setRange(1, 20);
  layout_steps->addWidget(steps_);

  layout->addLayout(layout_steps);

  QHBoxLayout* layout_start_stop = new QHBoxLayout;

  subdivide_ = new QPushButton("subdivide", parent);
  layout_start_stop->addWidget(subdivide_);

  stop_ = new QPushButton("stop", parent);
  layout_start_stop->addWidget(stop_);

  layout->addLayout(layout_start_stop);

  QObject::connect(subdivide_, SIGNAL(clicked(bool)),
                   this, SLOT(subdivide_clicked(bool)));

  QObject::connect(stop_, SIGNAL(clicked(bool)),
                   this, SLOT(stop_clicked(bool)));

  progress_ = new QProgressBar(parent);
  // busy indicator with max=min=0
  progress_->setMinimum(0);
  progress_->setMaximum(0);
  layout->addWidget(progress_);

  set_progress_controls_visible(false);
}

void GuiControls::subdivide_clicked(bool) {
  const int steps = steps_->value();
  active_algorithm_ = current_algo_render_pair().algorithm.get();
  auto callback = [this] (std::unique_ptr<surface_mesh::Surface_mesh> mesh) {
    // TODO pair!: mesh_data_->load(std::move(mesh));
    set_progress_controls_visible(false);
  };

  set_progress_controls_visible(true);
  active_algorithm_->subdivide_threaded(mesh_data_->get_mesh(), callback, steps);
}

void GuiControls::stop_clicked(bool) {
  active_algorithm_->stop_subdivide_threaded();
}

void GuiControls::set_progress_controls_visible(bool visible) {
  stop_->setEnabled(visible);
  progress_->setVisible(visible);
}

void GuiControls::update_valid_dropdown_items(
  const surface_mesh::Surface_mesh& mesh) {


  QStandardItemModel* model = qobject_cast<QStandardItemModel*>
                              (dropdown_->model());

  int first_enabled_item = enable_applicable_algorithms_dropdown(mesh, model);
  ensure_current_dropdown_item_is_enabled(model, first_enabled_item);
}

int GuiControls::enable_applicable_algorithms_dropdown(
  const surface_mesh::Surface_mesh& mesh, QStandardItemModel* model) {

  QStandardItem* item {nullptr};
  int first_enabled_item_index { -1};

  for (int i = 0; i < dropdown_->count(); i++) {
    QModelIndex index = model->index(i, dropdown_->modelColumn(),
                                     dropdown_->rootModelIndex());
    item = model->itemFromIndex(index);

    if (algorithms_->at(item->text()).algorithm->is_subdividable(mesh)) {
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


void GuiControls::ensure_current_dropdown_item_is_enabled(
  QStandardItemModel* model, int first_enabled_item_index) {
  QModelIndex current_index = model->index(dropdown_->currentIndex(),
                              dropdown_->modelColumn(), dropdown_->rootModelIndex());
  if (!model->itemFromIndex(current_index)->isEnabled()) {
    dropdown_->setCurrentIndex(first_enabled_item_index);
  }
}

} // namespace subdivision

