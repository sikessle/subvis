#include "GL/gl.h"
#include <QtDebug>

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

GLRenderer::~GLRenderer() {
}

void GLRenderer::mesh_updated(const Surface_mesh& mesh) {
  mesh_.reset(new Surface_mesh{mesh});
}

void GLRenderer::render_mesh_opengl() {
  if (mesh_) {
    render();
  }
}

void GLRenderer::init_opengl() {
  qDebug() << "Initializing open gl.";
}

} // namespace subdivision
