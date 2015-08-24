#include "io_controller.h"

namespace SubVis {

IOController::IOController(SubVis::MeshData mesh) : mesh_model(mesh)
{

}

bool IOController::load_mesh(std::string &filename)
{
    return mesh_model.load(filename);
}

bool IOController::persist_mesh(std::string &filename) const
{
    return mesh_model.persist(filename);
}

} // namespace SubVis
