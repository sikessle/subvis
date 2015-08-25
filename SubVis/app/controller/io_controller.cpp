#include "io_controller.h"

namespace SubVis {

IOController::IOController(MeshData &mesh) : mesh_model(mesh)
{

}

bool IOController::load_mesh(string &filename)
{
    return mesh_model.load(filename);
}

bool IOController::persist_mesh(string &filename) const
{
    return mesh_model.persist(filename);
}

void IOController::mesh_updated()
{

}

} // namespace SubVis
