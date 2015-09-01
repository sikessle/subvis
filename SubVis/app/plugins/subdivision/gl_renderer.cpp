#include "GL/gl.h"

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

GLRenderer::~GLRenderer() {

}

void GLRenderer::start_rendering() {
  glBegin(GL_QUAD_STRIP);
}

void GLRenderer::render_mesh_opengl(const surface_mesh::Surface_mesh& mesh) {
  start_rendering();
  render(mesh);
  end_rendering();
}

void GLRenderer::end_rendering() {
  glEnd();
}

} // namespace subdivision
