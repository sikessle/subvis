#include <GL/glut.h>
#include <QtDebug>
#include "QGLViewer/manipulatedFrame.h"
#include <math.h>

#include "view/mesh_edit_handler.h"

namespace subvis {

using Point = surface_mesh::Point;
using Vertex = surface_mesh::Surface_mesh::Vertex;

MeshEditHandler::MeshEditHandler(int mesh_id) : mesh_id_(mesh_id) {
}

bool MeshEditHandler::callback_handle_previous_click(QGLViewer* viewer) {
  if (!unhandled_click_) {
    return false;
  }
  qDebug() << "Handling last click";
  unhandled_click_ = false;

  const Vertex* vertex = get_vertex_at_click();

  if (vertex == nullptr) {
    qDebug("No vertex found @ click (%d, %d)", click_x_, click_y_);
    return false;
  } else {
    editing_point_ =
      &editable_mesh_->get_vertex_property<Point>("v:point")[*vertex];
    Point& handle = *editing_point_;
    qDebug("Found vertex @ click (%d, %d): v%d with coordinates: %f %f %f",
           click_x_, click_y_, vertex->idx(), handle[0], handle[1], handle[2]);

    edit_in_progress_ = true;
    viewer->setManipulatedFrame(&manipulated_frame_);
    // Set to correct position
    manipulated_frame_.setPosition(handle[0], handle[1], handle[2]);
    // Constrain translations etc.
    auto normal = editable_mesh_->compute_vertex_normal(*vertex);
    vertex_normal_ = qglviewer::Vec(normal[0], normal[1], normal[2]);
    edit_constraint_.set_vertex_normal(vertex_normal_);
    manipulated_frame_.setConstraint(&edit_constraint_);
    qDebug() << "Manipulated Frame created";

    return true;
  }
}

bool MeshEditHandler::keyPressEvent(QKeyEvent* e) {
  if (e->key() != Qt::Key_S) {
    return false;
  }

  if (translation_type_ == VERTEX_NORMAL_PLANE) {
    translation_type_ = VERTEX_NORMAL_ORTHOGONAL_PLANE;
    edit_constraint_.set_plane_orthogonal(true);
  } else {
    translation_type_ = VERTEX_NORMAL_PLANE;
    edit_constraint_.set_plane_orthogonal(false);
  }

  return true;
}

bool MeshEditHandler::mouseDoubleClickEvent(QMouseEvent* const event,
    MeshData& mesh_data, QGLViewer* viewer) {
  // Delegate roations etc. to default behaviour, if we are not in edit mode.
  if (!is_edit_event(event)) {
    qDebug() << "Using default mouse behavior.";
    return false;
  }
  qDebug() << "Using edit mode mouse behavior.";

  // Save previous modification
  if (edit_in_progress_) {
    qDebug() << "Saving previous modifications to a mesh point";
    qreal x, y, z;
    manipulated_frame_.getPosition(x, y, z);

    if (!isnan(x) && !isnan(y) && !isnan(z)) {
      Point& p = *editing_point_;
      p[0] = x;
      p[1] = y;
      p[2] = z;
      qDebug("New Position: %f %f %f", p[0], p[1], p[2]);
      qDebug() << "Saving modified mesh";

      mesh_data.load_and_duplicate(std::move(editable_mesh_), mesh_id_);
      viewer->setManipulatedFrame(nullptr);
      edit_in_progress_ = false;
    }
  } else {
    qDebug() << "Handling a new click on a mesh point";
    click_x_ = event->x();
    click_y_ = event->y();
    unhandled_click_ = true;
    // During the next redraw we will handle the click
  }

  return true;
}

void MeshEditHandler::set_mouse_binding(QGLViewer* viewer) {
  viewer->setMouseBinding(Qt::ControlModifier, Qt::LeftButton, QGLViewer::FRAME,
                          QGLViewer::TRANSLATE);
}

void MeshEditHandler::set_enabled(bool enabled) {
  enabled_ = enabled;
  if (enabled_) {
    extract_vertices();
  }
}

void MeshEditHandler::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  mesh_ = &mesh;
  if (enabled_) {
    extract_vertices();
  }
}

void MeshEditHandler::extract_vertices() {
  id_to_vertex_.clear();

  if (!mesh_) {
    return;
  }

  qDebug() << "Copying mesh to allow for non-destructive editing";
  editable_mesh_ = std::unique_ptr<surface_mesh::Surface_mesh>
                   (new surface_mesh::Surface_mesh(*mesh_));

  qDebug() << "Extracting vertices and mapping to ids";

  for (Vertex vertex : editable_mesh_->vertices()) {
    id_to_vertex_.insert(std::pair<int, Vertex>(vertex.idx(), vertex));
  }
}

bool MeshEditHandler::is_edit_event(QMouseEvent* const event) const {
  return enabled_
         && event->button() == Qt::LeftButton
         && mesh_;
}

const surface_mesh::Surface_mesh::Vertex*
MeshEditHandler::get_vertex_at_click() const {

  glClearColor(1.f, 1.f, 1.f, 1.f);
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
      editable_mesh_->get_vertex_property<Point>("v:point")[id_vertex.second];
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
    return &id_to_vertex_.at(vertex_idx);
  }

  return nullptr;
}

void MeshEditHandler::index_to_rgba(const int index,
                                    unsigned char rgba[]) const {
  const unsigned char* index_ptr = (const unsigned char*) &index;

  for (int i = 0; i < kPixelsBytes; i++) {
    rgba[i] = index_ptr[i];
  }
  qDebug("Index %d to rgba: %d %d %d", index, rgba[0], rgba[1], rgba[2]);
}

int MeshEditHandler::rgba_to_index(const unsigned char rgba[]) const {
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

// Draws the edit handle centered at 0,0,0 in the manipulated frame.
void MeshEditHandler::draw_edit_handle(QGLViewer* viewer) {

  if (!edit_in_progress_) {
    return;
  }

  // Vertex normal in world coordinate system
  glLineWidth(2.0);
  glBegin(GL_LINES);
  glColor3f(.9f, .9f, .9f);
  Point& start = *editing_point_;
  qglviewer::Vec normal = translation_type_ == VERTEX_NORMAL_PLANE ?
                          vertex_normal_  : vertex_normal_.orthogonalVec();
  glVertex3f(start[0] - normal[0], start[1] - normal[1], start[2] - normal[2]);
  glVertex3f(start[0] + normal[0], start[1] + normal[1], start[2] + normal[2]);
  glEnd();

  // Line between vertex origin and new position
  glBegin(GL_LINES);
  glColor3f(102 / 255.f, 0.f, 0.f);
  qglviewer::Vec mf_pos = manipulated_frame_.position();
  glVertex3f(start[0], start[1], start[2]);
  glVertex3f(mf_pos[0], mf_pos[1], mf_pos[2]);
  glEnd();

  // Save the current model view matrix
  glPushMatrix();

  // Multiply matrix to get in the frame coordinate system.
  glMultMatrixd(manipulated_frame_.matrix());

  // Draw the vertex edit handle
  glDisable(GL_DEPTH_TEST);
  glPointSize(kEditHandleSize);
  glBegin(GL_POINTS);
  glColor3f(1.f, .0f, .0f);
  glVertex3f(0.f, 0.f, 0.f);
  glEnd();
  glEnable(GL_DEPTH_TEST);

  // Restore the original (world) coordinate system
  glPopMatrix();

  // Draw information about current constraint
  viewer->qglColor(viewer->foregroundColor());
  glDisable(GL_LIGHTING);
  QString text_type = translation_type_ == VERTEX_NORMAL_PLANE ?
                      "Plane of vertex normal" : "Orthogonal plane of vertex normal";
  viewer->drawText(10, viewer->height() - 10,
                   "Translation type: " + text_type + " (S)");
}

} // namespace subvis

