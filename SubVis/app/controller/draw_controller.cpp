#include "draw_controller.h"

namespace subvis {

DrawController::DrawController(MeshData& mesh_data) : Controller{mesh_data} {
}

MeshData& DrawController::get_mesh_data() {
  return mesh_data_;
}

void DrawController::triangulate_mesh() {
  mesh_data_.triangulate();
}

} // namespace subvis

