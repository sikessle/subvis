#include "io_controller.h"

namespace SubVis {

IOController::IOController(MeshData &mesh) : Controller{mesh}
{

}

bool IOController::load_mesh(const string &filename)
{
    return mesh_data.load(filename);
}

bool IOController::persist_mesh(const string &filename) const
{
    return mesh_data.persist(filename);
}

const string &IOController::load_file_formats() const
{
    return mesh_data.load_file_formats();
}

const string &IOController::persist_file_formats() const
{
    return mesh_data.persist_file_formats();
}

} // namespace SubVis
