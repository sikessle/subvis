#include "view/viewer_widget.h"

namespace subvis {

using MeshPairRef =
  std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>;

ViewerWidget::ViewerWidget(QWidget* parent, int mesh_id)
  : QGLViewer{parent},
    mesh_id_{mesh_id} {

  setMouseBinding(Qt::KeyboardModifier::NoModifier, Qt::MouseButton::LeftButton,
                  ClickAction::NO_CLICK_ACTION, true);
}

void ViewerWidget::draw() {
  // To keep layout in full size
  updateGeometry();
  // Call subclasses draw method
  draw_gl();
}

void ViewerWidget::init() {
  // Call subclasses draw method
  init_gl();
}

void ViewerWidget::set_model(const MeshData& mesh_data) {
  mesh_data_ = &mesh_data;

  // listen to mesh updates
  connect(mesh_data_, &MeshData::updated, this, &ViewerWidget::mesh_updated_self);
}

void ViewerWidget::mesh_updated_self(MeshPairRef meshes) {
  if (mesh_id_ == 0) {
    mesh_updated(meshes.first);
  } else {
    mesh_updated(meshes.second);
  }
}

} // namespace subvis
