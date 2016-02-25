#include <QtOpenGL>
#include "plugins/subdivision/gl_bspline_renderer.h"

namespace subdivision {

void GLBSplineRenderer::mesh_updated(const Surface_mesh& mesh) {

  GLRenderer::mesh_updated(mesh);

  mesh_.reset(new surface_mesh::Surface_mesh{mesh});
  mesh_->triangulate();
  mesh_->update_face_normals();
  mesh_->update_vertex_normals();

  triangles_.clear();

  for (const auto& face : mesh_->faces()) {

    for (const auto& vertex : mesh_->vertices(face)) {
      triangles_.push_back(vertex.idx());
    }
  }

  points_ = mesh_->vertex_property<Point>("v:point");
  vnormals_ = mesh_->vertex_property<Point>("v:normal");

  create_color_values();
}

void GLBSplineRenderer::render() {
  create_mock();
}

void GLBSplineRenderer::create_mock() {
  draw_mesh();
}

void GLBSplineRenderer::create_color_values() {
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

void GLBSplineRenderer::draw_mesh() {

  drawing_type_ = GL_TRIANGLES;
  shading_type_ = GL_SMOOTH;
  coloring_active_ = false;
  lighting_active_ = true;

  if (!mesh_ || (mesh_->n_faces() < 1)) {
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

  if (triangles_.size()) {
    glDrawElements(drawing_type_, (GLsizei) triangles_.size(), GL_UNSIGNED_INT, &triangles_[0]);
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

} // namespace subdivision
