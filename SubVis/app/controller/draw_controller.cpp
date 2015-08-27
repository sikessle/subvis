#include "draw_controller.h"

namespace SubVis {

DrawController::DrawController(MeshData& mesh_data) : Controller{mesh_data}
{
}

MeshData& DrawController::mesh_data()
{
    return mesh_data_;
}

} // namespace SubVis

