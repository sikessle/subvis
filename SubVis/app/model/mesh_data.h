#ifndef SUBVIS_MODEL_MESH_DATA_H
#define SUBVIS_MODEL_MESH_DATA_H

#include <string>
#include <memory>
#include <vector>
#include <QObject>
#include "surface_mesh/Surface_mesh.h"

namespace subvis {

///
/// \brief The model layer. Offers some convenience functions.
/// Can manage up to 2 different meshes at the same time.
/// Uses the surface_mesh data structure. Ownership is made explicit by unqiue_ptr
///
class MeshData : public QObject {
  Q_OBJECT

  using Surface_mesh = ::surface_mesh::Surface_mesh;

 public:
  MeshData();

  ///
  /// \brief Returns the mesh for the index.
  /// \param idx The mesh index to retrieve.
  /// \return The the first mesh if idx is 0, else the second mesh.
  ///
  const Surface_mesh& get_mesh(int idx) const;
  ///
  /// \brief Tries to load a mesh from the filename (path) and replaces the current
  /// meshes with the loaded one. Creates a new history entry.
  /// \param filename The path to load the file from.
  /// \return True if the mesh could be loaded, else false.
  ///
  bool load(const std::string& filename);
  ///
  /// \brief Loads the single mesh at the index idx. The mesh will be duplicated
  /// and placed in the other index as well. Creates a new history entry.
  /// \param mesh The mesh to load
  /// \param idx The index where the mesh will be placed
  ///
  void load_and_duplicate(std::unique_ptr<Surface_mesh> mesh, int idx);
  ///
  /// \brief Loads a pair of meshes.
  /// \param meshes The meshes to load
  ///
  void load(
    std::pair<std::unique_ptr<Surface_mesh>, std::unique_ptr<Surface_mesh>> meshes);
  ///
  /// \brief Step back in history. All subsequent calls to get_mesh() will
  /// result in returning the one step older meshes
  ///
  void history_step_back();
  ///
  /// \brief Step forward in history. All subsequent calls to get_mesh() will
  /// result in returning the one step newer meshes
  ///
  void history_step_forward();
  ///
  /// \brief Purge the history. All subsequent calls to get_mesh() will return
  /// an empty mesh.
  ///
  void history_purge();
  ///
  /// \brief Checks if the history can be stepped backwards. May be handy to
  /// display the state of an undo button
  /// \return True if a call to history_step_back() will be successful.
  ///
  bool history_can_step_back();
  ///
  /// \brief Checks if the history can be stepped forwards. May be handy to
  /// display the state of a redo button
  /// \return True if a call to history_step_forward() will be successful.
  ///
  bool history_can_step_forward();
  ///
  /// \brief Persist the mesh with index idx to a file.
  /// \param filename
  /// \param idx
  /// \return
  ///
  bool persist(const std::string& filename, int idx) const;
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
