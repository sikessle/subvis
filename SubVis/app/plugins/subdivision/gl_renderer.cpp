#include "GL/gl.h"

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

GLRenderer::~GLRenderer() {

}

void GLRenderer::before_rendering() {
  glBegin(GL_QUAD_STRIP);
}

void GLRenderer::render_mesh_opengl(const surface_mesh::Surface_mesh& mesh) {
  before_rendering();
  render(mesh);
  after_rendering();
}

void GLRenderer::after_rendering() {
  glEnd();
}

} // namespace subdivision
