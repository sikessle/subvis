#include "draw_controller.h"

namespace SubVis {

DrawController::DrawController(MeshData &mesh) : Controller{mesh}
{
    QObject::connect(&mesh, SIGNAL(updated()), this, SLOT(mesh_updated()));
}

void DrawController::mesh_updated()
{
    emit redraw_required();
}

Surface_mesh &DrawController::mesh()
{
    return mesh_data.mesh();
}

} // namespace SubVis

