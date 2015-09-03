#include "GL/gl.h"

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

GLRenderer::~GLRenderer() {

}

void GLRenderer::init_opengl() {

}


void GLRenderer::render_mesh_opengl(const surface_mesh::Surface_mesh& mesh) {
  render(mesh);
}

} // namespace subdivision
