#include <iostream>
#include <utility>
#include <GL/gl.h>
#include <QtDebug>

#include "view/edit_mesh_mouse_handler.h"

namespace subvis {

EditMeshMouseHandler::EditMeshMouseHandler() {

}

void EditMeshMouseHandler::set_enabled(bool enable) {
  enabled_ = enable;
  if (enabled_) {
    addInMouseGrabberPool();
  } else {
    removeFromMouseGrabberPool();
  }
  qDebug() << "Enabled:" << enable;
}

void EditMeshMouseHandler::checkIfGrabsMouse(int /*x*/, int /*y*/,
    const qglviewer::Camera* const /*camera*/) {
  setGrabsMouse(enabled_);
}

void EditMeshMouseHandler::index_to_rgba(const int index,
    unsigned char rgba[4]) const {
  const unsigned char* index_ptr = (const unsigned char*) &index;

  for (int i = 0; i < kPixelsBytes; i++) {
    rgba[i] = index_ptr[i];
  }
  qDebug("Index %d to rgba: %d %d %d", index, rgba[0], rgba[1], rgba[2]);
}

int EditMeshMouseHandler::rgba_to_index(const unsigned char rgba[4])
const {
  // rebuild index from r, g and b values.
  // inverse function of index_to_rgb.
  int index = 0;
  unsigned char* index_ptr = (unsigned char*) &index;

  for (int i = 0; i < kPixelsBytes; i++) {
    index_ptr[i] = rgba[i];
  }

  qDebug("Rgba %d %d %d to index: %d", rgba[0], rgba[1], rgba[2], index);

  return index;
}

void EditMeshMouseHandler::mesh_updated(const surface_mesh::Surface_mesh&
                                        mesh) {
  using Vertex = surface_mesh::Surface_mesh::Vertex;
  mesh_ = &mesh;
  id_to_vertex_.clear();

  for (Vertex vertex : mesh_->vertices()) {
    id_to_vertex_.insert(std::pair<int, Vertex>(vertex.idx(), vertex));
  }
}

const surface_mesh::Surface_mesh::Vertex* EditMeshMouseHandler::get_vertex_at_click() const {
  using Point = surface_mesh::Point;
  using Vertex = surface_mesh::Surface_mesh::Vertex;

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
  for (const auto& id_vertex : id_to_vertex_) {
    // Set color based on idx
    index_to_rgba(id_vertex.first, rgba);
    const Point& p =
      mesh_->get_vertex_property<Point>("v:point")[id_vertex.second];
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

  // Get vertex by id
  if (id_to_vertex_.count(vertex_idx) > 0) {
    const Vertex& vertex = id_to_vertex_.at(vertex_idx);
    return &vertex;
  }

  return nullptr;
}

void EditMeshMouseHandler::draw_gl() {
  using Point = surface_mesh::Point;
  using Vertex = surface_mesh::Surface_mesh::Vertex;

  if (id_to_vertex_.empty() || !unhandled_click_) {
    return;
  }

  const Vertex* vertex = get_vertex_at_click();

  if (vertex != nullptr) {
    Point& point = mesh_->get_vertex_property<Point>("v:point")[*vertex];
    qDebug("Found vertex @ click (%d, %d): v%d with coordinates: %f %f %f",
           click_x_, click_y_, vertex->idx(), point[0], point[1], point[2]);
    mf_.setPosition(point[0], point[1], point[2]);
    // TODO use mf_ to move vertex
    // TODO MANIPULATED FRAME INSTEAD OF MOUSE GRABBER. TEST WITH 1 Fixed Point.

    // TODO get faces around: mesh_->faces(vertex)
  } else {
    qDebug("No vertex found @ click (%d, %d)", click_x_, click_y_);
  }

  unhandled_click_ = false;
}

void EditMeshMouseHandler::mousePressEvent(QMouseEvent* const event,
    qglviewer::Camera* const /*camera*/) {
  qDebug() << "MousePressEvent received.";
  if (!mesh_) {
    qDebug() << "No mesh via mesh_updated received. Aborting mouse event handling.";
    return;
  }

  unhandled_click_ = true;

  // TODO does this trigger the redraw???

  click_x_ = event->x();
  click_y_ = event->y();
}

} // namespace subvis
