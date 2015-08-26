#include "draw_controller.h"

namespace SubVis {

DrawController::DrawController(MeshData &mesh) : Controller{mesh}
{
}

Surface_mesh &DrawController::mesh()
{
    return mesh_data.mesh();
}

} // namespace SubVis

