#include "io_controller.h"

namespace subvis {

IOController::IOController(MeshData& mesh_data) : Controller{mesh_data} {

}

bool IOController::load_mesh(const std::string& filename) {
  return mesh_data_.load(filename);
}

bool IOController::persist_mesh(const std::string& filename) const {
  return mesh_data_.persist(filename);
}

const std::string& IOController::get_load_file_formats() const {
  return mesh_data_.get_load_file_formats();
}

const std::string& IOController::get_persist_file_formats() const {
  return mesh_data_.get_persist_file_formats();
}

} // namespace subvis
