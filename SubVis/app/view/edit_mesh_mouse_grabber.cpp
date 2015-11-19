#include <iostream>
#include <GL/gl.h>

#include "view/edit_mesh_mouse_grabber.h"

namespace subvis {

EditMeshMouseGrabber::EditMeshMouseGrabber() {

}

void EditMeshMouseGrabber::set_enabled(bool enable) {
  enabled_ = enable;
  if (enabled_) {
    addInMouseGrabberPool();
  } else {
    removeFromMouseGrabberPool();
  }
}

void EditMeshMouseGrabber::checkIfGrabsMouse(int /*x*/, int /*y*/,
    const qglviewer::Camera* const /*camera*/) {
  setGrabsMouse(enabled_);
}

void EditMeshMouseGrabber::index_to_rgba(const int index,
    unsigned char rgba[4]) const {
  const unsigned char* index_ptr = (const unsigned char*) &index;

  for (int i = 0; i < kPixelsBytes; i++) {
    rgba[i] = index_ptr[i];
  }
}

int EditMeshMouseGrabber::rgba_to_index(const unsigned char rgba[4])
const {
  // rebuild index from r, g and b values.
  // inverse function of index_to_rgb.
  int index = 0;
  unsigned char* index_ptr = (unsigned char*) &index;

  for (int i = 0; i < kPixelsBytes; i++) {
    index_ptr[i] = rgba[i];
  }

  return index;
}

void EditMeshMouseGrabber::mesh_updated(const surface_mesh::Surface_mesh&
                                        mesh) {
  mesh_ = &mesh;
}

void EditMeshMouseGrabber::draw_gl() {
  using Point = surface_mesh::Point;

  if (!mesh_ || !unhandled_click_) {
    return;
  }

  glClearColor(255, 255, 255, 255);
  glClear(GL_COLOR_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  unsigned char rgba[4];
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  const int viewport_height = viewport[3];

  // Limit drawing area to the clicked area around the mouse click coordinates.
  glScissor(click_x_ - kClickBoxLength / 2,
            viewport_height - click_y_ - kClickBoxLength / 2,
            kClickBoxLength,
            kClickBoxLength);

  // Start rendering the mesh with unique colors per vertex
  glEnable(GL_SCISSOR_TEST);

  glPointSize(kClickBoxLength);

  glBegin(GL_POINTS);
  for (const auto& vertex : mesh_->vertices()) {
    // Set color based on idx
    index_to_rgba(vertex.idx(), rgba);
    const Point& p = mesh_->get_vertex_property<Point>("v:point")[vertex];
    glColor4f(rgba[0] / 255.0f, rgba[1] / 255.0f, rgba[2] / 255.0f, 1.0f);
    glVertex3f(p[0], p[1], p[2]);
  }
  glEnd();

  glDisable(GL_SCISSOR_TEST);

  // Get pixel color of mouse click coordinates (1 pixel)
  unsigned char pixels[4];
  glReadPixels(click_x_, viewport_height - click_y_, 1, 1, GL_RGBA,
               GL_UNSIGNED_BYTE, pixels);
  // Convert color to vertex idx
  const unsigned int vertex_idx = rgba_to_index(pixels);
  std::cerr << "Calculated vertex_idx: " << vertex_idx << std::endl;
  // Get vertex by id
  // TODO query mesh by id

  unhandled_click_ = false;
}

void EditMeshMouseGrabber::mousePressEvent(QMouseEvent* const event,
    qglviewer::Camera* const /*camera*/) {
  if (!mesh_) {
    return;
  }

  unhandled_click_ = true;

  click_x_ = event->x();
  click_y_ = event->y();
}

} // namespace subvis
