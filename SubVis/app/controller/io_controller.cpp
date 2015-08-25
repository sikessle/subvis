#include "io_controller.h"

namespace SubVis {

IOController::IOController(MeshData &mesh) : Controller{mesh}
{

}

bool IOController::load_mesh(string &filename)
{
    return mesh_data.load(filename);
}

bool IOController::persist_mesh(string &filename) const
{
    return mesh_data.persist(filename);
}

} // namespace SubVis
