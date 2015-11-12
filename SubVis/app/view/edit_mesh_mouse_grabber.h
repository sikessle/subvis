#ifndef EDIT_MESH_MOUSE_GRABBER_H
#define EDIT_MESH_MOUSE_GRABBER_H

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

};

} // namespace subvis
#endif // EDIT_MESH_MOUSE_GRABBER_H
