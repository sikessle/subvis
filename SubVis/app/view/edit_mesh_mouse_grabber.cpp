#include <iostream>

#include "view/edit_mesh_mouse_grabber.h"

namespace subvis {

EditMeshMouseGrabber::EditMeshMouseGrabber() {

}

void EditMeshMouseGrabber::set_enabled(bool enable) {
  enabled_ = enable;
}

void EditMeshMouseGrabber::checkIfGrabsMouse(int /*x*/, int /*y*/,
    const qglviewer::Camera* const /*camera*/) {
  setGrabsMouse(enabled_);
}

void EditMeshMouseGrabber::mousePressEvent(QMouseEvent* const /*event*/,
    qglviewer::Camera* const /*camera*/) {
  std::cerr << "mouse click. " << std::endl;
}

} // namespace subvis
