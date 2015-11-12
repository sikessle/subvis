#ifndef EDIT_MESH_MOUSE_GRABBER_H
#define EDIT_MESH_MOUSE_GRABBER_H

#include <map>
#include <QMouseEvent>
#include "QGLViewer/mouseGrabber.h"
#include "surface_mesh/Surface_mesh.h"

namespace subvis {

class EditMeshMouseGrabber : public qglviewer::MouseGrabber {
 public:
  EditMeshMouseGrabber();

  void checkIfGrabsMouse(int x, int y, const qglviewer::Camera* const camera) override;
  void set_enabled(bool enable);
  void mesh_updated(const surface_mesh::Surface_mesh& mesh);

 protected:
  void mousePressEvent(QMouseEvent* const event,
                       qglviewer::Camera* const camera) override;

 private:
  bool enabled_{false};
  std::map<unsigned int, const surface_mesh::Point*> points_;
  const int kClickBoxLength = 4;
  const int kPixelsBytes = 3;

  /// RGBA color values will be stored in rgba array
  void index_to_rgba(const unsigned int index, unsigned char rgba[4]) const;
  /// Allows rgba values
  unsigned int rgba_to_index(const unsigned char rgba[4]) const;
  void render_mesh_colored();
};

} // namespace subvis
#endif // EDIT_MESH_MOUSE_GRABBER_H
