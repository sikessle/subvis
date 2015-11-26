#ifndef SUBVIS_VIEW_EDIT_MESH_MOUSE_HANDLER_H
#define SUBVIS_VIEW_EDIT_MESH_MOUSE_HANDLER_H

#include <map>
#include <QMouseEvent>
#include "QGLViewer/mouseGrabber.h"
#include "surface_mesh/Surface_mesh.h"

namespace subvis {

class EditMeshMouseHandler : public qglviewer::MouseGrabber {
 public:
  EditMeshMouseHandler();

  void checkIfGrabsMouse(int x, int y,
                         const qglviewer::Camera* const camera) override;
  void set_enabled(bool enable);
  void mesh_updated(const surface_mesh::Surface_mesh& mesh);
  void draw_gl();

 protected:
  void mousePressEvent(QMouseEvent* const event,
                       qglviewer::Camera* const camera) override;

 private:
  bool enabled_{false};
  std::map<int, const surface_mesh::Surface_mesh::Vertex> id_to_vertex_;
  const surface_mesh::Surface_mesh* mesh_{nullptr};
  bool unhandled_click_ {false};
  int click_x_ {0};
  int click_y_ {0};
  const int kClickBoxLength {6};
  const int kPixelsBytes {3};

  const surface_mesh::Surface_mesh::Vertex* get_vertex_at_click() const;
  /// RGBA color values will be stored in rgba array
  void index_to_rgba(const int index, unsigned char rgba[4]) const;
  /// Allows rgba values
  int rgba_to_index(const unsigned char rgba[4]) const;
};

} // namespace subvis
#endif // SUBVIS_VIEW_EDIT_MESH_MOUSE_HANDLER_H
