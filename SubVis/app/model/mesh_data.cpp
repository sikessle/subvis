#include "mesh_data.h"

namespace subvis {

MeshData::MeshData() {
  auto mesh = std::unique_ptr<surface_mesh::Surface_mesh> {new surface_mesh::Surface_mesh};
  history_.push_back(std::move(mesh));
}

const surface_mesh::Surface_mesh& MeshData::get_mesh() const {
  return *(history_.at(history_index_).get());
}

void MeshData::load(std::unique_ptr<surface_mesh::Surface_mesh> mesh) {
  push_history(std::move(mesh));
  emit_updated_signal();
}

bool MeshData::load(const std::string& filename) {
  if (filename.empty()) {
    return false;
  }

  // create new mesh
  auto mesh = std::unique_ptr<surface_mesh::Surface_mesh> {new surface_mesh::Surface_mesh};
  bool success = mesh->read(filename);
  push_history(std::move(mesh));
  emit_updated_signal();

  return success;
}

void MeshData::triangulate() {
  auto copy = std::unique_ptr<surface_mesh::Surface_mesh> {new surface_mesh::Surface_mesh(get_mesh())};
  copy->triangulate();
  push_history(std::move(copy));

  emit_updated_signal();
}

void MeshData::history_step_back() {
  if (history_index_ > 0) {
    history_index_--;
  }
  emit_updated_signal();
}

void MeshData::history_step_forward() {
  if (history_index_ < history_.size() - 1) {
    history_index_++;
  }
  emit_updated_signal();
}

void MeshData::push_history(std::unique_ptr<surface_mesh::Surface_mesh> mesh) {
  if (history_.size() == kHistorySize) {
    history_.erase(history_.begin() + 1);
  }
  // Throw away everyhting which is newer than current index
  if (history_.size() > history_index_) {
    history_.erase(history_.begin() + history_index_ + 1, history_.end());
  }
  mesh->garbage_collection();
  history_.push_back(std::move(mesh));
  history_index_++;
}

void MeshData::emit_updated_signal() {
  emit updated(get_mesh());
}

bool MeshData::persist(const std::string& filename) const {
  if (filename.empty()) {
    return false;
  }

  return get_mesh().write(filename);
}

const std::string& MeshData::get_load_file_formats() const {
  return kLoadFileFormats;
}

const std::string& MeshData::get_persist_file_formats() const {
  return kPersistFileFormats;
}

} // namespace SubVis

