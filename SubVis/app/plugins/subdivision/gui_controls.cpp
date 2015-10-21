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

void GuiControls::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  current_algo_render_pair().renderer->mesh_updated(mesh);
  update_valid_dropdown_items(mesh);
}

void GuiControls::init_opengl() {
  current_algo_render_pair().renderer->init_opengl();
}

void GuiControls::draw_opengl() {
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
  steps_->setRange(1, 100);
  layout_steps->addWidget(steps_);

  layout->addLayout(layout_steps);

  subdivide_ = new QPushButton("subdivide", parent);
  layout->addWidget(subdivide_);

  QObject::connect(subdivide_, SIGNAL(clicked(bool)),
                   this, SLOT(subdivide_clicked(bool)));

  stop_ = new QPushButton("stop", parent);
  layout->addWidget(stop_);
}

void GuiControls::subdivide_clicked(bool) {
  const int steps = steps_->value();
  auto& algorithm = current_algo_render_pair().algorithm;

  // TODO start in new thread, copy mesh before
  auto result = algorithm->subdivide(mesh_data_->get_mesh(), steps);

  mesh_data_->load(std::move(result));
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

