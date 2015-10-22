#include <exception>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "plugins/subdivision/gui_controls.h"

namespace subdivision {

using Surface_mesh = ::surface_mesh::Surface_mesh;
using MeshPairUniquePtrs =
  std::pair<std::unique_ptr<Surface_mesh>, std::unique_ptr<Surface_mesh>>;

GuiControls::GuiControls() {

}

void GuiControls::create(QWidget* parent,
                         std::map<std::pair<int, const QString>, AlgorithmRenderer>& algorithms) {
  algorithms_ = &algorithms;
  // Gui creation
  QVBoxLayout* layout = new QVBoxLayout(parent);
  layout->setAlignment(Qt::AlignTop);

  layout->addWidget(new QLabel("Algorithms:"));

  QHBoxLayout* layout_dropdown0 = new QHBoxLayout;
  layout_dropdown0->setAlignment(Qt::AlignTop);
  QHBoxLayout* layout_dropdown1 = new QHBoxLayout;
  layout_dropdown1->setAlignment(Qt::AlignTop);

  layout_dropdown0->addWidget(new QLabel("[0]"));
  layout_dropdown1->addWidget(new QLabel("[1]"));

  dropdown0_ = new QComboBox(parent);
  dropdown1_ = new QComboBox(parent);
  for (const auto& it : *algorithms_) {
    // using name from map's entries as label
    if (it.first.first == 0) {
      dropdown0_->addItem(it.first.second);
    } else {
      dropdown1_->addItem(it.first.second);
    }
  }
  layout_dropdown0->addWidget(dropdown0_, 1);
  layout_dropdown1->addWidget(dropdown1_, 1);

  layout->addLayout(layout_dropdown0);
  layout->addLayout(layout_dropdown1);

  connect(dropdown0_, &QComboBox::currentTextChanged, this,
          &GuiControls::dropdown_changed);
  connect(dropdown1_, &QComboBox::currentTextChanged, this,
          &GuiControls::dropdown_changed);

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

  connect(subdivide_, &QPushButton::clicked, this,
          &GuiControls::subdivide_clicked);
  connect(stop_, &QPushButton::clicked, this, &GuiControls::stop_clicked);

  progress_ = new QProgressBar(parent);
  // busy indicator with max=min=0
  progress_->setMinimum(0);
  progress_->setMaximum(0);
  layout->addWidget(progress_);

  set_progress_controls_visible(false);
}

void GuiControls::collect_selected_algorithms() {
  active_algo0_ = current_algo_render_pair(0).algorithm.get();
  active_algo1_ = current_algo_render_pair(1).algorithm.get();
}

void GuiControls::dropdown_changed() {
  collect_selected_algorithms();

  emit needs_redraw();
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
    return selected_algo_renderer(dropdown0_, mesh_id);
  } else {
    return selected_algo_renderer(dropdown1_, mesh_id);
  }
}

void GuiControls::set_model(subvis::MeshData& mesh_data) {
  mesh_data_ = &mesh_data;
}

void GuiControls::mesh_updated(const Surface_mesh& mesh, int mesh_id) {
  current_algo_render_pair(mesh_id).renderer->mesh_updated(mesh);
  update_valid_dropdown_items(mesh, mesh_id);
}

void GuiControls::init_opengl(int mesh_id) {
  current_algo_render_pair(mesh_id).renderer->init_opengl();
}

void GuiControls::draw_opengl(int mesh_id) {
  current_algo_render_pair(mesh_id).renderer->render_mesh_opengl();
}

void GuiControls::subdivide_clicked(bool) {
  const int steps = steps_->value();
  collect_selected_algorithms();

  auto callback1 = [this] (std::unique_ptr<Surface_mesh> mesh) {
    subdivide_finished(std::move(mesh), result0_);
  };
  auto callback2 = [this] (std::unique_ptr<Surface_mesh> mesh) {
    subdivide_finished(std::move(mesh), result1_);
  };

  set_progress_controls_visible(true);

  result0_ = nullptr;
  result1_ = nullptr;

  active_algo0_->subdivide_threaded(mesh_data_->get_mesh(0), callback1, steps);
  active_algo1_->subdivide_threaded(mesh_data_->get_mesh(1), callback2, steps);
}

void GuiControls::subdivide_finished(std::unique_ptr<Surface_mesh> mesh,
                                     std::unique_ptr<Surface_mesh>& result_target) {
  result_target = std::move(mesh);

  if (result0_ && result1_) {
    mesh_data_->load(MeshPairUniquePtrs{std::move(result0_), std::move(result1_)});
    set_progress_controls_visible(false);
  }
}

void GuiControls::stop_clicked(bool) {
  active_algo0_->stop_subdivide_threaded();
  active_algo1_->stop_subdivide_threaded();
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
    update_valid_items(dropdown0_, mesh_id, mesh);
  } else {
    update_valid_items(dropdown1_, mesh_id, mesh);
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

