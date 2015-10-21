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
  using UniqueMeshPtr = std::unique_ptr<Surface_mesh>;
  using MeshPair = std::pair<Surface_mesh&, Surface_mesh&>;
  using MeshPairUniquePtrs = std::pair<UniqueMeshPtr, UniqueMeshPtr>;

 public:
  MeshData();

  const Surface_mesh& get_mesh() const;
  bool load(const std::string& filename);
  void load(UniqueMeshPtr mesh);
  void history_step_back();
  void history_step_forward();
  void history_purge();
  bool history_can_step_back();
  bool history_can_step_forward();
  bool persist(const std::string& filename) const;
  const std::string& get_load_file_formats() const;
  const std::string& get_persist_file_formats() const;
  void triangulate();

 private:
  std::vector<UniqueMeshPtr> history_;
  unsigned int history_index_{0};
  const std::string kLoadFileFormats {"*.obj *.off *.stl"};
  const std::string kPersistFileFormats {"*.off"};
  const unsigned int kHistorySize {15};
  void emit_updated_signal();

  void history_push(UniqueMeshPtr mesh);

 signals:
  void updated(const Surface_mesh& meshes);
};

} // namespace subvis
#endif // SUBVIS_MODEL_MESH_DATA_H
