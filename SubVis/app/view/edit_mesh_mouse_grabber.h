#ifndef EDIT_MESH_MOUSE_GRABBER_H
#define EDIT_MESH_MOUSE_GRABBER_H

#include <QMouseEvent>
#include "QGLViewer/mouseGrabber.h"

namespace subvis {

class EditMeshMouseGrabber : public qglviewer::MouseGrabber {
 public:
  EditMeshMouseGrabber();

  void checkIfGrabsMouse(int x, int y, const qglviewer::Camera* const camera) override;
  void set_enabled(bool enable);

 protected:
  void mousePressEvent(QMouseEvent* const event,
                       qglviewer::Camera* const camera) override;

 private:
  bool enabled_{false};
  const int kClickBoxLength = 4;
  const int kRgbaBytes = 4;

  /// RGBA color values will be stored in rgba array
  void index_to_rgba(const unsigned int index, unsigned char rgba[4]) const;
  /// Allows rgba values
  unsigned int rgba_to_index(const unsigned char rgba[4]) const;
  void render_mesh_colored();
};

} // namespace subvis
#endif // EDIT_MESH_MOUSE_GRABBER_H
