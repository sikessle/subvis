#ifndef SUBVIS_MODEL_MESH_DATA_H
#define SUBVIS_MODEL_MESH_DATA_H

#include <string>
#include <memory>
#include <vector>
#include <QObject>
#include "surface_mesh/Surface_mesh.h"

namespace subvis {

class MeshData : public QObject {
  Q_OBJECT

  using Surface_mesh = ::surface_mesh::Surface_mesh;

 public:
  MeshData();

  const Surface_mesh& get_mesh(int idx) const;
  bool load(const std::string& filename);
  void load_and_duplicate(std::unique_ptr<Surface_mesh> mesh, int idx);
  void load(
    std::pair<std::unique_ptr<Surface_mesh>, std::unique_ptr<Surface_mesh>> meshes);
  void history_step_back();
  void history_step_forward();
  void history_purge();
  bool history_can_step_back();
  bool history_can_step_forward();
  bool persist(const std::string& filename) const;
  const std::string& get_load_file_formats() const;
  const std::string& get_persist_file_formats() const;
  void triangulate(int idx);

 private:
  std::vector<std::pair<std::unique_ptr<Surface_mesh>, std::unique_ptr<Surface_mesh>>>
  history_;
  unsigned int history_index_{0};
  const std::string kLoadFileFormats {"*.obj *.off *.stl"};
  const std::string kPersistFileFormats {"*.off"};
  const unsigned int kHistorySize {15};

  void emit_updated_signal();
  void history_push(
    std::pair<std::unique_ptr<Surface_mesh>, std::unique_ptr<Surface_mesh>> mesh);

 signals:
  void updated(
    std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>
    meshes);
};

} // namespace subvis
#endif // SUBVIS_MODEL_MESH_DATA_H
