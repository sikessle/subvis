#include "view/viewer_widget.h"

namespace subvis {

ViewerWidget::ViewerWidget(QWidget* parent, int mesh_id)
  : QGLViewer{parent},
    mesh_id_{mesh_id} {
}

void ViewerWidget::set_mesh_id(int mesh_id) {
  mesh_id_ = mesh_id;
}

void ViewerWidget::set_model(MeshData& mesh_data) {
  mesh_data_ = &mesh_data;

  // listen to mesh updates
  QObject::connect(mesh_data_,
                   SIGNAL(updated(
                            std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>)),
                   this,
                   SLOT(mesh_updated(
                          std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>)));
}

void ViewerWidget::mesh_updated(
  std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>
  meshes) {

  if (mesh_id_ == 0) {
    mesh_updated(meshes.first);
  } else {
    mesh_updated(meshes.second);
  }
}

} // namespace subvis
