#include <iostream>
#include <GL/gl.h>

#include "view/edit_mesh_mouse_grabber.h"

namespace subvis {

EditMeshMouseGrabber::EditMeshMouseGrabber() {

}

void EditMeshMouseGrabber::set_enabled(bool enable) {
  enabled_ = enable;
}

void EditMeshMouseGrabber::mesh_updated(const surface_mesh::Surface_mesh&
                                        mesh) {
  using Point = surface_mesh::Point;
  points_.clear();
  int index = 1;
  for (const auto& vertex : mesh.vertices()) {
    points_[index] = &mesh.get_vertex_property<Point>("v:point")[vertex];
    index++;
  }
}

void EditMeshMouseGrabber::checkIfGrabsMouse(int /*x*/, int /*y*/,
    const qglviewer::Camera* const /*camera*/) {
  setGrabsMouse(enabled_);
}

void EditMeshMouseGrabber::index_to_rgba(const unsigned int index,
    unsigned char rgba[4]) const {
  const unsigned char* index_ptr = (const unsigned char*) &index;

  for (int i = 0; i < kPixelsBytes; i++) {
    rgba[i] = index_ptr[i];
  }
}

unsigned int EditMeshMouseGrabber::rgba_to_index(const unsigned char rgba[4])
const {
  // rebuild index from r, g and b values.
  // inverse function of index_to_rgb.
  unsigned int index = 0;
  unsigned char* index_ptr = (unsigned char*) &index;

  for (int i = 0; i < kPixelsBytes; i++) {
    index_ptr[i] = rgba[i];
  }

  return index;
}

void EditMeshMouseGrabber::render_mesh_colored() {
  using Point = surface_mesh::Point;
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  unsigned char rgba[4];

  for (const auto& entry : points_) {
    index_to_rgba(entry.first, rgba);
    glColor4f(rgba[0] / 255.0f, rgba[1] / 255.0f, rgba[2] / 255.0f, 1.0f);
    const Point p = *entry.second;

    glBegin(GL_POLYGON);

    // TODO vertex must be drawn in size kClickBoxLength!
    // Calculate correct start and end points (normalized -1 to 1?)
    glVertex3d(p[0] - 0.1, p[1], p[2]);
    glVertex3d(p[0] + 0.1, p[1], p[2]);

    glVertex3d(p[0], p[1] + 0.1, p[2]);
    glVertex3d(p[0], p[1] - 0.1, p[2]);

    glVertex3d(p[0], p[1], p[2] + 0.1);
    glVertex3d(p[0], p[1], p[2] - 0.1);

    glEnd();
  }
}

void EditMeshMouseGrabber::mousePressEvent(QMouseEvent* const event,
    qglviewer::Camera* const /*camera*/) {
  using Point = surface_mesh::Point;
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
  glFlush();
  glFinish();
  glReadPixels(x, viewport_height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  // Convert color to vertex id
  const unsigned int vertex_id = rgba_to_index(pixels);
  // Get vertex by id
  if (points_.find(vertex_id) != points_.end()) {
    const Point& point = *points_[vertex_id];
    // TODO remove debug
    std::printf("Vertex found at: %f, %f, %f\n", point[0], point[1], point[2]);
  }

}

} // namespace subvis
