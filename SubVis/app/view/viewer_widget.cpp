#include "view/viewer_widget.h"

namespace subvis {

ViewerWidget::ViewerWidget(QWidget* parent) : QGLViewer{parent} {
}

void ViewerWidget::set_model(MeshData& mesh_data) {
  mesh_data_ = &mesh_data;

  // listen to mesh updates
  QObject::connect(mesh_data_,
                   SIGNAL(updated(const surface_mesh::Surface_mesh&)), this,
                   SLOT(mesh_updated(const surface_mesh::Surface_mesh&)));
}

void ViewerWidget::mesh_updated(const surface_mesh::Surface_mesh& /*mesh*/) {
  // do nothing on default
}

} // namespace subvis
