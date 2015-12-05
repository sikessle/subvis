#include <QtDebug>
#include <QtOpenGL>
#include "plugins/subdivision/gl_interpolating_renderer.h"

namespace subdivision {

void GLInterpolatingRenderer::mesh_updated(const Surface_mesh& mesh) {
  GLRenderer::mesh_updated(mesh);
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
}

void GLInterpolatingRenderer::render() {
  if (mesh_->n_faces() < 1) {
    return;
  }

  glClearColor(0, 0, 0, 0);
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, points_.data());
  glNormalPointer(GL_FLOAT, 0, vnormals_.data());

  if (triangles_.size()) {
    glDrawElements(GL_TRIANGLES, (GLsizei) triangles_.size(), GL_UNSIGNED_INT,
                   &triangles_[0]);
  }
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

}
