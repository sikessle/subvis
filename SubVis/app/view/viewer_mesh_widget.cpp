#include <GL/glut.h>
#include <QtDebug>
#include <QMouseEvent>
#include <cmath>

#include "view/viewer_mesh_widget.h"

namespace subvis {

using Point = surface_mesh::Point;

ViewerMeshWidget::ViewerMeshWidget(QWidget* parent,
                                   int mesh_id) :
  ViewerWidget{parent, mesh_id} {
  // Initialize mouse bindings for the edit handling
  mesh_edit_handler_.set_mouse_binding(this);

  // Initialize drawing, shading, coloring and lighting values
  drawing_type_ = GL_LINES;
  shading_type_ = GL_FLAT;
  coloring_active_ = true;
  lighting_active_ = true;
}

void ViewerMeshWidget::set_edit(bool edit) {
  mesh_edit_handler_.set_enabled(edit);
  qDebug() << "Edit set to" << edit;
}

void ViewerMeshWidget::set_drawing_type(GLenum type) {
  drawing_type_ = type;

  // Update mesh (necessary because of triangulation when drawing faces)
  mesh_updated(*mesh_);

  // Trigger redrawing to update meshes
  updateGL();

  qDebug() << "Drawing type changed";
}

void ViewerMeshWidget::set_shading_type(GLenum type) {
  shading_type_ = type;

  // Draw mesh to update shading
  draw_mesh();

  // Trigger redrawing to update meshes
  updateGL();

  qDebug() << "Shading type changed";
}

void ViewerMeshWidget::set_coloring(bool active) {
  coloring_active_ = active;

  // Draw mesh to update shading
  draw_mesh();

  // Trigger redrawing to update meshes
  updateGL();

  qDebug() << "Coloring active: " << active;
}

void ViewerMeshWidget::set_lighting(bool active) {
  lighting_active_ = active;

  // Draw mesh to update shading
  draw_mesh();

  // Trigger redrawing to update meshes
  updateGL();

  qDebug() << "Lighting active: " << active;
}

void ViewerMeshWidget::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  mesh_ = &mesh;
  mesh_edit_handler_.mesh_updated(mesh);

  // Get new editable mesh instance
  editable_mesh_.reset(new surface_mesh::Surface_mesh{mesh});
  editable_mesh_->update_face_normals();
  editable_mesh_->update_vertex_normals();

  // Reset vertices vector
  vertex_indices_.clear();

  // Triangulate mesh if faces are drawn (draw OpenGL triangles)
  if (drawing_type_ == GL_TRIANGLES) {
    editable_mesh_->triangulate();
  }

  // Vertex index vector
  std::vector<unsigned int> indices_tmp;

  // Get indices of vertices per face
  for (const auto& face : editable_mesh_->faces()) {

    indices_tmp.clear();

    for (const auto& vertex : editable_mesh_->vertices(face)) {
      indices_tmp.push_back(vertex.idx());

      // Get all vertices two times to draw OpenGL lines
      if (drawing_type_ == GL_LINES) {
        indices_tmp.push_back(vertex.idx());
      }

    }

    // Move first vertex to last position (copy to end and delete first one)
    // Vertex order: 1-2, 2-3, 3-4, 4-1 to draw OpenGL lines
    if (drawing_type_ == GL_LINES) {
      indices_tmp.insert(indices_tmp.end(), indices_tmp.begin(), indices_tmp.begin() + 1);
      indices_tmp.erase(indices_tmp.begin(), indices_tmp.begin() + 1);
    }

    vertex_indices_.insert(vertex_indices_.end(), indices_tmp.begin(), indices_tmp.end());
  }

  points_ = editable_mesh_->vertex_property<Point>("v:point");
  vnormals_ = editable_mesh_->vertex_property<Point>("v:normal");

  create_color_values();

  // Trigger redraw
  updateGL();
}

void ViewerMeshWidget::create_color_values() {
  // Reset color value vector
  color_values_.clear();

  for (const auto& vertex : mesh_->vertices()) {

    // Create color value vector from absolute vertex coordinates (just as an example)
    const Point& p = mesh_->get_vertex_property<Point>("v:point")[vertex];
    color_values_.push_back(std::abs(p[0]));
    color_values_.push_back(std::abs(p[1]));
    color_values_.push_back(std::abs(p[2]));
    color_values_.push_back(1.0);
  }
}

void ViewerMeshWidget::mouseDoubleClickEvent(QMouseEvent* const event) {
  // Event was not handled by the edit handler.
  if (!mesh_edit_handler_.mouseDoubleClickEvent(event, *mesh_data_, this)) {;
    ViewerWidget::mouseDoubleClickEvent(event);
  }
}

void ViewerMeshWidget::keyPressEvent(QKeyEvent* e) {
  // Edit handler changed and display might need to update
  if (mesh_edit_handler_.keyPressEvent(e)) {
    // Trigger redraw.
    updateGL();
  } else {
    // Propagate to super class.
    ViewerWidget::keyPressEvent(e);
  }
}

void ViewerMeshWidget::init_gl() {
  qDebug() << "Initializing open gl.";

  setSceneRadius(10.0);
  camera()->setZNearCoefficient(0.0001);
  camera()->setZClippingCoefficient(10.0);

  qDebug() << "Finished initializing open gl.";
}

void ViewerMeshWidget::draw_gl() {
  qDebug() << "Drawing mesh.";

  // Edit handler had an unhandled click, redraw to clear any leftovers
  // from the color picking algorithm.
  if (mesh_edit_handler_.callback_handle_previous_click(this)) {
    updateGL();
    return;
  }

  draw_mesh();

  // Draw edit handle
  mesh_edit_handler_.draw_edit_handle(this);
}

void ViewerMeshWidget::draw_mesh() {

  if (!editable_mesh_ || (editable_mesh_->n_faces() < 1)) {
    return;
  }

  if (lighting_active_) {
    glEnable(GL_LIGHTING);
  }

  // Flat: without color gradient; Smooth: with color gradient
  glShadeModel(shading_type_);
  glLineWidth(1.0);

  // Optimized drawing (only highest z-axis elements drawn)
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // OpenGL settings
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Save the current model view matrix
  glPushMatrix();

  // Set vertex pointer
  glVertexPointer(3, GL_FLOAT, 0, points_.data());

  // Set normals pointer (for lighting)
  glNormalPointer(GL_FLOAT, 0, vnormals_.data());

  if (coloring_active_) {
    // Set color pointer (coordinates as RGB colors for testing purposes)
      glColorPointer(4, GL_FLOAT, 0, &color_values_[0]);
  } else {
      glColor3f(1.0, 1.0, 1.0);
  }

  // Set OpenGL state
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  if (coloring_active_) {
    glEnableClientState(GL_COLOR_ARRAY);
  }

  if (vertex_indices_.size()) {
    glDrawElements(drawing_type_, (GLsizei) vertex_indices_.size(), GL_UNSIGNED_INT, &vertex_indices_[0]);
  }

  // Reset OpenGL state to original state
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  if (coloring_active_) {
    glDisableClientState(GL_COLOR_ARRAY);
  }

  glDisable(GL_DEPTH_TEST);

  if (lighting_active_) {
    glDisable(GL_LIGHTING);
  }

  // Restore the original world coordinate system
  glPopMatrix();
}
} // namespace subvis
