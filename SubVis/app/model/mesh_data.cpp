#include "mesh_data.h"

namespace subvis {

MeshData::MeshData() {
}

const surface_mesh::Surface_mesh& MeshData::get_mesh() const {
  return *mesh_object_.get();
}

void MeshData::load(std::unique_ptr<surface_mesh::Surface_mesh> mesh) {
  mesh_object_ = std::move(mesh);
  emit updated(*mesh_object_.get());
}

bool MeshData::load(const std::string& filename) {
  if (filename.empty()) {
    return false;
  }

  // create new mesh
  mesh_object_.reset(new surface_mesh::Surface_mesh);
  bool success = mesh_object_->read(filename);
  emit updated(*mesh_object_.get());

  return success;
}

bool MeshData::persist(const std::string& filename) const {
  if (filename.empty()) {
    return false;
  }

  mesh_object_->garbage_collection();

  return mesh_object_->write(filename);
}

void MeshData::triangulate() {
  mesh_object_->triangulate();

  emit updated(*mesh_object_.get());
}

const std::string& MeshData::get_load_file_formats() const {
  return kLoadFileFormats;
}

const std::string& MeshData::get_persist_file_formats() const {
  return kPersistFileFormats;
}

} // namespace SubVis

