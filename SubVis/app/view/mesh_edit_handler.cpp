#include <GL/glut.h>
#include <QtDebug>
#include "QGLViewer/manipulatedFrame.h"
#include <math.h>

#include "view/mesh_edit_handler.h"

namespace subvis {

using Point = surface_mesh::Point;
using Vertex = surface_mesh::Surface_mesh::Vertex;

MeshEditHandler::MeshEditHandler(int mesh_id) : mesh_id_(mesh_id) {
  manipulated_frame_.setConstraint(&constraint_);
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
    // A vertex was found. So start an edit operation.
    edit_in_progress_ = true;
    edited_point_ =
      &editable_mesh_->get_vertex_property<Point>("v:point")[*vertex];
    Point& handle = *edited_point_;
    qDebug("Found vertex @ click (%d, %d): v%d with coordinates: %f %f %f",
           click_x_, click_y_, vertex->idx(), handle[0], handle[1], handle[2]);
    // Set the frame which is used to move the vertex
    viewer->setManipulatedFrame(&manipulated_frame_);
    // Set to correct position
    manipulated_frame_.setPosition(handle[0], handle[1], handle[2]);
    // Constrain translations etc.
    auto normal = editable_mesh_->compute_vertex_normal(*vertex);
    edited_point_normal_ = qglviewer::Vec(normal[0], normal[1], normal[2]);
    constraint_.set_vertex_normal(edited_point_normal_);
    qDebug() << "Manipulated Frame set";

    return true;
  }
}

bool MeshEditHandler::keyPressEvent(QKeyEvent* e) {
  if (e->key() != Qt::Key_S || !enabled_) {
    qDebug() << "Pressed key is not handled by this class";
    return false;
  }
  // Rotate through the two constraint modes
  use_orthogonal_normal_ = !use_orthogonal_normal_;
  constraint_.set_plane_orthogonal(use_orthogonal_normal_);
  qDebug() <<
           "Constraint type switched. Using plane orthogonal to normal of vertex: "
           << use_orthogonal_normal_;
  return true;
}

bool MeshEditHandler::mouseDoubleClickEvent(QMouseEvent* const event,
    MeshData& mesh_data, QGLViewer* viewer) {
  // Delegate roations etc. to default behaviour, if we are not in edit mode.
  if (!doubleclick_is_edit_event(event)) {
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
      Point& p = *edited_point_;
      p[0] = x;
      p[1] = y;
      p[2] = z;
      qDebug("New Position: %f %f %f", p[0], p[1], p[2]);
      qDebug() << "Saving modified mesh";

      mesh_data.load_and_duplicate(std::move(editable_mesh_), mesh_id_);
      // Remove frame as the edit operation is completed.
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
  extract_vertices();
}

void MeshEditHandler::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  mesh_ = &mesh;
  extract_vertices();
}

void MeshEditHandler::extract_vertices() {
  if (!enabled_ || !mesh_) {
    return;
  }

  id_to_vertex_.clear();

  qDebug() << "Copying mesh to allow for non-destructive editing";
  editable_mesh_ = std::unique_ptr<surface_mesh::Surface_mesh>
                   (new surface_mesh::Surface_mesh(*mesh_));

  qDebug() << "Extracting vertices and mapping to ids";

  for (const Vertex vertex : editable_mesh_->vertices()) {
    id_to_vertex_.insert(std::pair<int, Vertex>(vertex.idx(), vertex));
  }
}

bool MeshEditHandler::doubleclick_is_edit_event(QMouseEvent* const event)
const {
  return enabled_ && event->button() == Qt::LeftButton && mesh_;
}

const surface_mesh::Surface_mesh::Vertex*
MeshEditHandler::get_vertex_at_click() const {
  // Use a white background to avoid generating valid ids of 0 by clicking
  // on the background
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
  draw_vertex_normal();
  draw_origin_line();
  draw_edit_vertex();
  draw_constraint_text(viewer);
}

void MeshEditHandler::draw_vertex_normal() {
  glLineWidth(2.0);
  glBegin(GL_LINES);
  glColor3f(.9f, .9f, .9f);
  Point& start = *edited_point_;
  qglviewer::Vec normal = use_orthogonal_normal_ ?
                          edited_point_normal_.orthogonalVec()
                          : edited_point_normal_;
  glVertex3f(start[0] - normal[0], start[1] - normal[1], start[2] - normal[2]);
  glVertex3f(start[0] + normal[0], start[1] + normal[1], start[2] + normal[2]);
  glEnd();
}

void MeshEditHandler::draw_origin_line() {
  // Line between vertex origin and new position
  glBegin(GL_LINES);
  glColor3f(102 / 255.f, 0.f, 0.f);
  qglviewer::Vec mf_pos = manipulated_frame_.position();
  Point& start = *edited_point_;
  glVertex3f(start[0], start[1], start[2]);
  glVertex3f(mf_pos[0], mf_pos[1], mf_pos[2]);
  glEnd();
}

void MeshEditHandler::draw_edit_vertex() {
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

  // Restore the original world coordinate system
  glPopMatrix();
}

void MeshEditHandler::draw_constraint_text(QGLViewer* viewer) {
  // Draw information about current constraint
  viewer->qglColor(viewer->foregroundColor());
  glDisable(GL_LIGHTING);
  QString text_type = use_orthogonal_normal_ ?
                      "Orthogonal plane of vertex normal"
                      : "Plane of vertex normal";
  viewer->drawText(10, viewer->height() - 10,
                   "Translation type: " + text_type + " (S)");
}

} // namespace subvis

