#include "mesh_data.h"

namespace subvis {

  using Surface_mesh = surface_mesh::Surface_mesh;
  using UniqueMeshPtr = std::unique_ptr<Surface_mesh>;
  using MeshPair = std::pair<Surface_mesh&, Surface_mesh&>;
  using MeshPairUniquePtrs = std::pair<UniqueMeshPtr, UniqueMeshPtr>;

MeshData::MeshData() {
  auto mesh = UniqueMeshPtr {new surface_mesh::Surface_mesh};
  history_.push_back({std::move(mesh)});
}

const Surface_mesh& MeshData::get_mesh() const {
  return *(history_.at(history_index_).get());
}

void MeshData::load(UniqueMeshPtr mesh) {
  history_push(std::move(mesh));
  emit_updated_signal();
}

bool MeshData::load(const std::string& filename) {
  if (filename.empty()) {
    return false;
  }

  // create new mesh
  auto mesh = UniqueMeshPtr {new Surface_mesh};
  bool success = mesh->read(filename);
  history_push(std::move(mesh));
  emit_updated_signal();

  return success;
}

void MeshData::triangulate() {
  auto copy = UniqueMeshPtr {new Surface_mesh(get_mesh())};
  copy->triangulate();
  history_push(std::move(copy));

  emit_updated_signal();
}

void MeshData::history_step_back() {
  if (history_can_step_back()) {
    history_index_--;
  }
  emit_updated_signal();
}

void MeshData::history_step_forward() {
  if (history_can_step_forward()) {
    history_index_++;
  }
  emit_updated_signal();
}

bool MeshData::history_can_step_back() {
  return history_index_ > 0;
}

bool MeshData::history_can_step_forward() {
  return history_index_ < history_.size() - 1;
}

void MeshData::history_purge() {
  history_index_ = 0;
  // Keep the first empty mesh
  history_.erase(history_.begin() + 1, history_.end());
}

void MeshData::history_push(UniqueMeshPtr mesh) {
  // Case 1: history is pointing to an entry before the last element
  if (history_index_ < history_.size() - 1) {
    // Erase everything from the history_index on to the end.
    // This also ensures, that the history has now at least one free bucket.
    history_.erase(history_.begin() + history_index_ + 1, history_.end());
  }
  // Case 2: history is full
  else if (history_.size() == kHistorySize) {
    // Purge the 3rd entry (3rd oldest)
    history_.erase(history_.begin() + 2);
    history_index_--;
  }

  // Store entry at next position
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

