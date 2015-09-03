#include "GL/gl.h"

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

GLRenderer::~GLRenderer() {

}

void GLRenderer::before_render() {
  glBegin(GL_QUAD_STRIP);
}

void GLRenderer::render_mesh_opengl(const surface_mesh::Surface_mesh& mesh) {
  before_render();
  render(mesh);
  after_render();
}

void GLRenderer::after_render() {
  glEnd();
}

} // namespace subdivision
