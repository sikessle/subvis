#ifndef EDIT_MESH_MOUSE_GRABBER_H
#define EDIT_MESH_MOUSE_GRABBER_H

#include <QMouseEvent>
#include "QGLViewer/mouseGrabber.h"
#include "surface_mesh/Surface_mesh.h"

namespace subvis {

class EditMeshMouseGrabber : public qglviewer::MouseGrabber {
 public:
  EditMeshMouseGrabber();

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
  const surface_mesh::Surface_mesh* mesh_ {nullptr};
  bool unhandled_click_ {false};
  int click_x_ {0};
  int click_y_ {0};
  const int kClickBoxLength {4};
  const int kPixelsBytes {3};

  /// RGBA color values will be stored in rgba array
  void index_to_rgba(const int index, unsigned char rgba[4]) const;
  /// Allows rgba values
  int rgba_to_index(const unsigned char rgba[4]) const;
};

} // namespace subvis
#endif // EDIT_MESH_MOUSE_GRABBER_H
