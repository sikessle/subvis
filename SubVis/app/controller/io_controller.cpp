#include "io_controller.h"

namespace SubVis {

IOController::IOController(MeshData& mesh_data) : Controller{mesh_data}
{

}

bool IOController::load_mesh(const string& filename)
{
    return mesh_data_.load(filename);
}

bool IOController::persist_mesh(const string& filename) const
{
    return mesh_data_.persist(filename);
}

const string& IOController::load_file_formats() const
{
    return mesh_data_.load_file_formats();
}

const string& IOController::persist_file_formats() const
{
    return mesh_data_.persist_file_formats();
}

} // namespace SubVis
