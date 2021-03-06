#include <QtDebug>

#include "mesh_data.h"

namespace subvis {

using Surface_mesh = ::surface_mesh::Surface_mesh;
using MeshPairRef = std::pair<const Surface_mesh&, const Surface_mesh&>;
using MeshPairUniquePtrs =
  std::pair<std::unique_ptr<Surface_mesh>, std::unique_ptr<Surface_mesh>>;

MeshData::MeshData() {
  auto mesh0 = std::unique_ptr<Surface_mesh> {new Surface_mesh};
  auto mesh1 = std::unique_ptr<Surface_mesh> {new Surface_mesh};
  history_.push_back({std::move(mesh0), std::move(mesh1)});
}

const Surface_mesh& MeshData::get_mesh(int idx) const {
  qDebug() << "Returning mesh id" << idx;
  if (idx == 0) {
    return *(history_.at(history_index_).first.get());
  } else {
    return *(history_.at(history_index_).second.get());
  }
}

const Surface_mesh& MeshData::get_original_mesh(int idx) const {
  qDebug() << "Returning original mesh id" << idx;
  return *(original_mesh_.get());
}

void MeshData::load_and_duplicate(std::unique_ptr<Surface_mesh> mesh, int idx) {
  qDebug() << "Loading single mesh and duplicating";
  auto copy = std::unique_ptr<Surface_mesh> {new Surface_mesh(*mesh)};

  if (idx == 0) {
    load(MeshPairUniquePtrs{std::move(mesh), std::move(copy)});
  } else {
    load(MeshPairUniquePtrs{std::move(copy), std::move(mesh)});
  }
}

void MeshData::load(MeshPairUniquePtrs meshes) {
  qDebug() << "Mesh pair loaded";
  history_push({std::move(meshes.first), std::move(meshes.second)});
  emit_updated_signal();
}

bool MeshData::load(const std::string& filename) {
  if (filename.empty()) {
    qWarning() << "File name empty. Aborting loading.";
    return false;
  }

  qDebug() << "Loading mesh from file:" << QString::fromStdString(filename);
  // create new mesh where the file will be loaded into
  auto mesh_from_file = std::unique_ptr<Surface_mesh> {new Surface_mesh};
  bool success = mesh_from_file->read(filename);
  auto copy = std::unique_ptr<Surface_mesh> {new Surface_mesh(*mesh_from_file.get())};
  original_mesh_ = std::unique_ptr<Surface_mesh> {new Surface_mesh(*mesh_from_file.get())};
  history_push({std::move(mesh_from_file), std::move(copy)});
  emit_updated_signal();

  return success;
}

void MeshData::triangulate(int idx) {
  auto copy0 = std::unique_ptr<Surface_mesh> {new Surface_mesh(get_mesh(0))};
  auto copy1 = std::unique_ptr<Surface_mesh> {new Surface_mesh(get_mesh(1))};

  qDebug() << "Triangulating mesh" << idx;

  if (idx == 0) {
    copy0->triangulate();
  } else {
    copy1->triangulate();
  }
  history_push({std::move(copy0), std::move(copy1)});

  emit_updated_signal();
}

void MeshData::history_step_back() {
  if (history_can_step_back()) {
    qDebug() << "Stepping history back";
    history_index_--;
  }
  emit_updated_signal();
}

void MeshData::history_step_forward() {
  if (history_can_step_forward()) {
    qDebug() << "Stepping history forward";
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
  qDebug() << "History purged.";
  // Keep the first empty mesh
  history_.erase(history_.begin() + 1, history_.end());
}

void MeshData::history_push(MeshPairUniquePtrs meshes) {
  // Case 1: history is pointing to an entry before the last element
  if (history_index_ < history_.size() - 1) {
    // Erase everything from the history_index on to the end.
    // This also ensures, that the history has now at least one free bucket.
    history_.erase(history_.begin() + history_index_ + 1, history_.end());
  }
  // Case 2: history is full
  else if (history_.size() == kHistorySize) {
    // Purge the 3rd entry (3rd oldest) as we want to keep the first entry (empty mesh)
    // and the second entry (first loaded mesh)
    history_.erase(history_.begin() + 2);
    history_index_--;
  }

  qDebug() << "Added item to history.";

  // Store entry at next position
  meshes.first->garbage_collection();
  meshes.second->garbage_collection();
  history_.push_back({std::move(meshes.first), std::move(meshes.second)});
  history_index_++;
}

void MeshData::emit_updated_signal() {
  emit updated({get_mesh(0), get_mesh(1)});
}

bool MeshData::persist(const std::string& filename, int idx) const {
  if (filename.empty()) {
    qWarning() << "File name empty. Aborting persisting.";
    return false;
  }
  qDebug() << "Persisted mesh.";

  return get_mesh(idx).write(filename);
}

const std::string& MeshData::get_load_file_formats() const {
  return kLoadFileFormats;
}

const std::string& MeshData::get_persist_file_formats() const {
  return kPersistFileFormats;
}

} // namespace SubVis

