#include "GL/gl.h"
#include <QtDebug>

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

GLRenderer::~GLRenderer() {
}

void GLRenderer::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  // placeholder for custom data structure
  mesh_ = &mesh;
  // TODO extract vertices in a custom data structure (member variable) to speed up drawing!
  qDebug() << "Extracted vertices to custo data structure.";
}

void GLRenderer::render_mesh_opengl() {
  // TODO use custom data structure here
  if (mesh_) {
    render(*mesh_);
  }
}

void GLRenderer::init_opengl() {
  // basic init
}

} // namespace subdivision
