#include <iostream>
#include <GL/gl.h>

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

void EditMeshMouseGrabber::index_to_rgba(const unsigned int index,
    unsigned char rgba[4]) const {
  const unsigned char* index_ptr = (const unsigned char*) &index;

  for (int i = 0; i < kRgbaBytes; i++) {
    rgba[i] = index_ptr[i];
  }
}

unsigned int EditMeshMouseGrabber::rgba_to_index(const unsigned char rgba[4]) const {
  // rebuild index from r, g and b values.
  // inverse function of index_to_rgb.
  unsigned int index = 0;
  unsigned char* index_ptr = (unsigned char*) &index;

  for (int i = 0; i < kRgbaBytes; i++) {
    index_ptr[i] = rgba[i];
  }

  return index;
}

void EditMeshMouseGrabber::render_mesh_colored() {
  // TODO: glDraw.. -> render mesh with vertex. Vertex must be size of kClickBoxLength!!
}

void EditMeshMouseGrabber::mousePressEvent(QMouseEvent* const event,
    qglviewer::Camera* const /*camera*/) {
  const int x = event->x();
  const int y = event->y();
  GLint viewport[4];
  unsigned char pixels[4];

  glGetIntegerv(GL_VIEWPORT, viewport);
  const int viewport_height = viewport[3];

  // Limit drawing area to the clicked area around the mouse click coordinates.
  glScissor(x - kClickBoxLength / 2, viewport_height - y - kClickBoxLength / 2,
            kClickBoxLength,
            kClickBoxLength);

  // Start rendering the mesh with unique colors per vertex
  glEnable(GL_SCISSOR_TEST);
  render_mesh_colored();
  glDisable(GL_SCISSOR_TEST);

  // Get pixel color of mouse click coordinates (1 pixel)
  glReadPixels(x, viewport_height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  // Convert color to vertex id
  const unsigned int vertex_id = rgba_to_index(pixels);
  // TODO get vertex by vertex_id

  std::cerr << "Vertex id clicked: " << vertex_id << std::endl;

}

} // namespace subvis
