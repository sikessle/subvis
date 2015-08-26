#include "draw_controller.h"

namespace SubVis {

DrawController::DrawController(MeshData& mesh_data) : Controller{mesh_data}
{
}

Surface_mesh& DrawController::mesh() const
{
    return mesh_data_.mesh();
}

} // namespace SubVis

