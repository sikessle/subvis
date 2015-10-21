#include <exception>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "plugins/subdivision/gui_controls.h"

namespace subdivision {

GuiControls::GuiControls() {

}

AlgorithmRenderer&
GuiControls::selected_algo_renderer(QComboBox* dropdown, int mesh_id) {
  if (dropdown->count() == 0) {
    throw std::logic_error{"no algorithms loaded. ensure that at least one is loaded in the constructor."};
  }
  const QString name = dropdown->currentText();

  return algorithms_->at({mesh_id, name});
}

AlgorithmRenderer&
GuiControls::current_algo_render_pair(int mesh_id) {
  if (mesh_id == 0) {
    return selected_algo_renderer(dropdown1_, mesh_id);
  } else {
    return selected_algo_renderer(dropdown2_, mesh_id);
  }
}

void GuiControls::set_model(subvis::MeshData& mesh_data) {
  mesh_data_ = &mesh_data;
}

void GuiControls::mesh_updated(const surface_mesh::Surface_mesh& mesh,
                               int mesh_id) {
  current_algo_render_pair(mesh_id).renderer->mesh_updated(mesh);
  update_valid_dropdown_items(mesh, mesh_id);
}

void GuiControls::init_opengl(int mesh_id) {
  current_algo_render_pair(mesh_id).renderer->init_opengl();
}

void GuiControls::draw_opengl(int mesh_id) {
  current_algo_render_pair(mesh_id).renderer->render_mesh_opengl();
}

void GuiControls::create(QWidget* parent,
                         std::map<std::pair<const int, const QString>, AlgorithmRenderer>& algorithms) {
  algorithms_ = &algorithms;
  // Gui creation
  QVBoxLayout* layout = new QVBoxLayout(parent);
  layout->setAlignment(Qt::AlignTop);

  layout->addWidget(new QLabel("Algorithms:"));

  QHBoxLayout* layout_dropdown1 = new QHBoxLayout;
  layout_dropdown1->setAlignment(Qt::AlignTop);
  QHBoxLayout* layout_dropdown2 = new QHBoxLayout;
  layout_dropdown2->setAlignment(Qt::AlignTop);

  layout_dropdown1->addWidget(new QLabel("[0]"));
  layout_dropdown2->addWidget(new QLabel("[1]"));

  dropdown1_ = new QComboBox(parent);
  dropdown2_ = new QComboBox(parent);
  for (const auto& it : *algorithms_) {
    // using name from map's entries as label
    if (it.first.first == 0) {
      dropdown1_->addItem(it.first.second);
    } else {
      dropdown2_->addItem(it.first.second);
    }
  }
  layout_dropdown1->addWidget(dropdown1_, 1);
  layout_dropdown2->addWidget(dropdown2_, 1);

  layout->addLayout(layout_dropdown1);
  layout->addLayout(layout_dropdown2);

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
  mesh_id_active_algorithm_[0] = current_algo_render_pair(0).algorithm.get();
  mesh_id_active_algorithm_[1] = current_algo_render_pair(1).algorithm.get();
  auto callback = [this] (std::unique_ptr<surface_mesh::Surface_mesh> mesh) {
    // TODO pair!: mesh_data_->load(std::move(mesh));
    set_progress_controls_visible(false);
  };

  set_progress_controls_visible(true);
  // TODO get both meshes
  mesh_id_active_algorithm_[0]->subdivide_threaded(mesh_data_->get_mesh(),
      callback, steps);
  //mesh_id_active_algorithm_[1]->subdivide_threaded(mesh_data_->get_mesh(),  callback, steps);
}

void GuiControls::stop_clicked(bool) {
  // TODO get all...
  mesh_id_active_algorithm_.at(0)->stop_subdivide_threaded();
}

void GuiControls::set_progress_controls_visible(bool visible) {
  stop_->setEnabled(visible);
  progress_->setVisible(visible);
}

void GuiControls::update_valid_items(QComboBox* dropdown, int mesh_id,
                                     const surface_mesh::Surface_mesh& mesh) {

  QStandardItemModel* model = qobject_cast<QStandardItemModel*>
                              (dropdown->model());

  int first_enabled_item = enable_applicable_algorithms_dropdown(dropdown, mesh,
                           mesh_id, model);
  ensure_current_dropdown_item_is_enabled(dropdown, model, first_enabled_item);
}

void GuiControls::update_valid_dropdown_items(
  const surface_mesh::Surface_mesh& mesh, int mesh_id) {

  if (mesh_id == 0) {
    update_valid_items(dropdown1_, mesh_id, mesh);
  } else {
    update_valid_items(dropdown2_, mesh_id, mesh);
  }
}

int GuiControls::enable_applicable_algorithms_dropdown(QComboBox* dropdown,
    const surface_mesh::Surface_mesh& mesh, int mesh_id,
    QStandardItemModel* model) {

  QStandardItem* item {nullptr};
  int first_enabled_item_index { -1};

  for (int i = 0; i < dropdown->count(); i++) {
    QModelIndex index = model->index(i, dropdown->modelColumn(),
                                     dropdown->rootModelIndex());
    item = model->itemFromIndex(index);

    if (algorithms_->at({mesh_id, item->text()}).algorithm->is_subdividable(mesh)) {
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


void GuiControls::ensure_current_dropdown_item_is_enabled(QComboBox* dropdown,
    QStandardItemModel* model, int first_enabled_item_index) {
  QModelIndex current_index = model->index(dropdown->currentIndex(),
                              dropdown->modelColumn(), dropdown->rootModelIndex());
  if (!model->itemFromIndex(current_index)->isEnabled()) {
    dropdown->setCurrentIndex(first_enabled_item_index);
  }
}

} // namespace subdivision

